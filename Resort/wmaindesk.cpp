#include "wmaindesk.h"
#include "ui_wmaindesk.h"
#include "wreservation.h"
#include "winvoice.h"
#include "dlgchartdaterange.h"
#include "wreservations.h"
#include "dlgtransferroom.h"
#include "dlgendofday.h"
#include "epushbutton.h"
#include "dayitemdelegate.h"
#include "monthitemdelegate.h"
#include "roomitemdelegate.h"
#include "reservewidget.h"
#include "roomstate.h"
#include "dlgpostingcharges.h"
#include "dlgpaymentsdetails.h"
#include "dlgcreategroupreservation.h"
#include "eqtablewidget.h"
#include "dlgtransferinvoiceamount.h"
#include "dlgreserveconfirmstatus.h"
#include "dlggroupparams.h"
#include "dlgreceiptvaucher.h"
#include "dlggroupreservationfuck.h"
#include <QScrollBar>
#include <QDesktopWidget>
#include <QInputDialog>

#define SEL_CARDEX 1

WMainDesk::WMainDesk(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WMainDesk)
{
    ui->setupUi(this);
    ui->g->verticalScrollBar()->blockSignals(true);
    ui->g->horizontalScrollBar()->blockSignals(true);
    //ui->btnEndOfDay->setEnabled(r__(cr__super_correction));
    fDb.select("select f_short from f_room_classes order by 1", fDbBind, fDbRows);
    ui->tblClasses->setRowCount(1);
    ui->tblClasses->setColumnCount(fDbRows.count());
    int c = 0;
    foreach_rows {
        ui->tblClasses->setItem(0, c++, new QTableWidgetItem(it->at(0).toString()));
    }
    fDb.select("select f_id from f_room_bed order by 1", fDbBind, fDbRows);
    ui->tblBed->setRowCount(1);
    ui->tblBed->setColumnCount(fDbRows.count());
    c = 0;
    foreach_rows {
        ui->tblBed->setItem(0, c++, new QTableWidgetItem(it->at(0).toString()));
    }
    ui->tblClasses->setMaximumWidth((ui->tblClasses->columnCount() * ui->tblClasses->horizontalHeader()->defaultSectionSize()) + 5);
    ui->tblBed->setMaximumWidth((ui->tblBed->columnCount() * ui->tblBed->horizontalHeader()->defaultSectionSize()) + 5);
    fDockHint = new DWMainDeskHint(this);
    fDockHint->hide();
    connect(fDockHint, SIGNAL(visibilityChanged(bool)), this, SLOT(dockHintVisibilityChanged(bool)));
    fDateStart = QDate::fromString("01/01/2018", def_date_format);
    fDateEnd = QDate::fromString("31/12/2020", def_date_format);
    ChartStartDate = fDateStart;
    int dayCount = fDateStart.daysTo(fDateEnd) + 1;
    ui->tblDay->setColumnCount(dayCount);
    ui->tblDay->horizontalHeader()->setDefaultSectionSize(COLUMN_WIDTH);
    ui->tblDay->setMinimumWidth(dayCount * COLUMN_WIDTH + 2);
    ui->g->setMinimumWidth(ui->tblDay->minimumWidth());
    ui->tblDay->setRowCount(1);
    ui->tblDay->setItemDelegate(new DayItemDelegate());
    for (int i = 0; i < dayCount; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, fDateStart.addDays(i));
        ui->tblDay->setItem(0, i, item);
    }
    ui->tblMonth->setItemDelegate(new MonthItemDelegate());
    ui->tblMonth->setRowCount(1);
    ui->tblMonth->setColumnCount(ui->tblDay->columnCount());
    ui->tblMonth->horizontalHeader()->setDefaultSectionSize(COLUMN_WIDTH);
    int col = 0;
    for (QDate d = fDateStart, end = fDateEnd.addDays(1); d < end; d = d.addDays(d.daysInMonth())) {
        ui->tblMonth->setSpan(0, col, 1, d.daysInMonth());
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, d);
        ui->tblMonth->setItem(0, col, item);
        col += d.daysInMonth();
    }
    ui->tblRoom->setItemDelegate(new RoomItemDelegate());
    ui->tblRoom->verticalHeader()->setDefaultSectionSize(ROW_HEIGHT);
    fReserveStatus = 0;
    connect(ui->saMain->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scMainHScroll(int)));
    connect(ui->g->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scMainHScroll(int)));
    connect(ui->saDate->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scDateHScroll(int)));
    connect(ui->saMain->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scMainVScroll(int)));
    connect(ui->g->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scMainVScroll(int)));
    connect(ui->saRoom->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scRoomVScroll(int)));
    connect(ui->tblRoom->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(roomSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->tblDay->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(daySelectionChanged(QModelIndex,QModelIndex)));
    connect(CacheReservation::instance(), SIGNAL(cacheUpdated(QString)), this, SLOT(reservationCacheUpdated(QString)));
    connect(CacheRoom::instance(), SIGNAL(cacheUpdated(QString)), this, SLOT(roomCacheUpdated(QString)));
    fDockCardex = new DWSelectorReservationCardex(this);
    fDockCardex->configure();
    fDockCardex->setDialog(this, SEL_CARDEX);
    connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    timeout();
    fTimer.start(30000);
}

WMainDesk::~WMainDesk()
{
    delete ui;
}

void WMainDesk::setup()
{
    fScene = new RoomScene(this);
    connect(fScene, SIGNAL(editReserve(CI_Reservation*)), this, SLOT(editReserveRoom(CI_Reservation*)));
    ui->g->setScene(fScene);
    loadReservationList();
    filterRoom();
    int x = xFromDate(WORKING_DATE);
    ui->saMain->horizontalScrollBar()->setValue(x);
}

void WMainDesk::setupTab()
{
    setupTabTextAndIcon(tr("Room chart"), ":/images/analytics.png");
}

void WMainDesk::handleBroadcast(const QMap<QString, QVariant> &data)
{
    int cmd = data["command"].toInt();
    switch (cmd) {
    case cmd_end_of_day:
        int x = xFromDate(WORKING_DATE);
        ui->saMain->horizontalScrollBar()->setValue(x);
    }
}

void WMainDesk::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_CARDEX: {
            CI_Reservation *c = value.value<CI_Reservation*>();
            if (c) {
                fFilterCardex = c->fCardex;
                filterRoom();
            }
        }
        break;
    }
}

bool WMainDesk::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::WindowActivate:
        break;
    case QEvent::WindowDeactivate:
        //fDockHint->hide();
        break;
    default:
        break;
    }
    return BaseWidget::event(e);
}

void WMainDesk::timeout()
{
    if (WORKING_DATE.daysTo(QDate::currentDate()) > 0) {
        ui->btnEndOfDay->setEnabled(true);
    }
}

void WMainDesk::scDateHScroll(int value)
{
    ui->saMain->horizontalScrollBar()->setValue(value);
}

void WMainDesk::scMainHScroll(int value)
{
    ui->saDate->horizontalScrollBar()->setValue(value);
}

void WMainDesk::scRoomVScroll(int value)
{
    ui->saMain->verticalScrollBar()->setValue(value);
}

void WMainDesk::scMainVScroll(int value)
{
    ui->saRoom->verticalScrollBar()->setValue(value);
}

void WMainDesk::roomSelectionChanged(const QModelIndex &m1, const QModelIndex &m2)
{
    Q_UNUSED(m1)
    Q_UNUSED(m2)
    roomsRowSelectionChanged();
}

void WMainDesk::editReserveRoom(CI_Reservation *r)
{
    WReservation::openReserveWindows(r);
}

void WMainDesk::daySelectionChanged(const QModelIndex &m1, const QModelIndex &m2)
{
    Q_UNUSED(m1)
    Q_UNUSED(m2)
    daysColumnSelectionChanged();
}

void WMainDesk::reservationCacheUpdated(const QString &id)
{
    for (int i = 0; i < fReservationHint.count(); i++) {
        if (fReservationHint[i][1].toString() == id) {
            fReservationHint[i][0] = -1;
            fReservationHint[i][1] = -1;
            break;
        }
    }
    EQTableWidget *t = static_cast<EQTableWidget*>(fDockHint->tableWidget());
    for (int i = 0; i < t->rowCount(); i++) {
        if (t->toString(i, 1) == id) {
            t->setValue(i, 0, -1);
            break;
        }
    }
    fDbBind[":state1"] = RESERVE_CHECKIN;
    fDbBind[":state2"] = RESERVE_RESERVE;
    fDbBind[":state3"] = RESERVE_SERVICE;
    fDbBind[":state4"] = RESERVE_OUTOFINVENTORY;
    fDbBind[":state5"] = RESERVE_OUTOFROOM;
    fDb.select("select r.f_state, r.f_id, r.f_room, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName), "
               "r.f_startDate, r.f_endDate, r.f_cardex, n.f_name, '', "
               "r.f_invoice, concat(u.f_firstName, ' ', u.f_lastName), r.f_booking "
               "from f_reservation r  "
               "left join f_guests g on g.f_id=r.f_guest "
               "left join f_nationality n on n.f_short=g.f_nation "
               "left join users u on u.f_id=r.f_author "
               "where (r.f_state=:state1 or r.f_state=:state2 or r.f_state=:state3 or r.f_state=:state4 or r.f_state=:state5) "
               "and r.f_id=" + ap(id) + " "
               "order by 1, 2 ", fDbBind, fDbRows);
    foreach_rows {
        QList<QVariant> v = *it;
        fReservationHint.append(v);
        int row = t->rowCount();
        t->setRowCount(row + 1);
        for (int i = 0; i < it->count(); i++) {
           t->setItem(row, i, new QTableWidgetItem(Utils::variantToString(it->at(i))));
        }
        QTableWidgetItem *item = t->item(row, 0);
        item->setData(Qt::UserRole, item->text());
        item->setText("");
        QString pixmapName;
        switch (it->at(0).toInt()) {
        case RESERVE_CHECKIN:
            pixmapName = ":/images/ball-blue.png";
            break;
        case RESERVE_RESERVE:
            pixmapName = ":/images/ball-red.png";
            break;
        default:
            pixmapName = ":/images/ball-gray.png";
            break;
        }
        item->setData(Qt::DecorationRole, QPixmap(pixmapName));
        EPushButton *b = new EPushButton(this);
        b->setMinimumSize(25, 25);
        b->setMaximumSize(25, 25);
        b->setText("");
        b->setIcon(QIcon(":/images/goto.png"));
        b->setTag(row);
        connect(b, SIGNAL(clickedWithTag(int)), this, SLOT(btnDockHintGoToClicked(int)));
        t->setCellWidget(row, 8, b);
    }

    CI_Reservation *r = CacheReservation::instance()->get(id);
    if (!r)  {
        qDebug() << "Something wrong in reservation cache. Contact to application developer";
        return;
    }
    if (fReserveStatus > 0) {
        if (r->fStatus != fReserveStatus) {
            return;
        }
    }
    if (!fRoomIdList.contains(r->fRoom.toInt()) && r->fRoom.toInt() > 0) {
        return;
    }

    fScene->removeInvalidReserveWidget(r);
    switch (r->fState) {
    case RESERVE_CHECKIN:
    case RESERVE_RESERVE: {
        fScene->addReserveInfo(r);
        break;
    }
    case RESERVE_SERVICE:
    case RESERVE_OUTOFINVENTORY:
    case RESERVE_OUTOFROOM: {
        fScene->addReserveWidget(r);
        break;
    }
    }
}

void WMainDesk::roomCacheUpdated(const QString &id)
{
    Q_UNUSED(id)
    ui->tblRoom->viewport()->update();
}

void WMainDesk::btnDockHintGoToClicked(int tag)
{
    int reserveState = fReservationHint.at(tag).at(0).toInt();
    switch (reserveState) {
    case RESERVE_RESERVE: {
        CI_Reservation *r = CacheReservation::instance()->get(fReservationHint.at(tag).at(1).toString());
        WReservation::openReserveWindows(r);
        break;
    }
    case RESERVE_CHECKIN:
        WInvoice::openInvoiceWindow(fReservationHint.at(tag).at(9).toString());
        break;
    case -1:
        break;
    default:
        message_info_tr("This reservation is not editable");
        break;
    }
}

void WMainDesk::dockHintVisibilityChanged(bool v)
{
    if (!v) {
        ui->leFilterGuest->clear();
    }
}

void WMainDesk::changeDate()
{
    DlgChartDateRange *d = new DlgChartDateRange(this);
    if (d->exec() == QDialog::Accepted) {
        QDate date;
        d->getResult(date);
    }
    delete d;
}

void WMainDesk::filterRoom()
{
    fRoomList.clear();
    fRoomIdList.clear();
    CacheRoom *c = CacheRoom::instance();
    QModelIndexList selCat = ui->tblClasses->selectionModel()->selectedIndexes();
    QString category = "";
    if (selCat.count() > 0) {
        category = selCat.at(0).data(Qt::DisplayRole).toString();
    }
    QModelIndexList selBed = ui->tblBed->selectionModel()->selectedIndexes();
    QString bed = "";
    QModelIndexList selSmoke = ui->tblSmoke->selectionModel()->selectedIndexes();
    if (selBed.count() > 0) {
        bed = selBed.at(0).data(Qt::DisplayRole).toString();
    }
    QMapIterator<QString, CI_Room*> it = c->it();
    while (it.hasNext()) {
        it.next();
        CI_Room *room = it.value();
        if (!ui->leJumpToRoom->text().isEmpty()) {
            QStringList jumpList = ui->leJumpToRoom->text().split(",", QString::SkipEmptyParts);
            if (jumpList.contains(room->fCode)) {
                fRoomList.append(room);
                fRoomIdList.insert(room->fCode.toInt());
                continue;
            } else {
                continue;
            }
        }
        if (category.length() > 0) {
            if (room->fCategoryShort != category) {
                continue;
            }
        }
        if (bed.length() > 0) {
            if (room->fBed != bed) {
                continue;
            }
        }
        if (selSmoke.count() > 0) {
            switch (selSmoke.at(0).column()) {
            case 0:
                break;
            case 1:
                if (room->fSmoke == 1) {
                    continue;
                }
                break;
            case 2:
                if (room->fSmoke == 0) {
                    continue;
                }
                break;
            }
        }
        fRoomList.append(room);
        fRoomIdList.insert(room->fCode.toInt());
    }
    fScene->initBackgroung(fDateStart.daysTo(fDateEnd) + 1, fRoomList);
    ui->tblRoom->setRowCount(fRoomList.count());
    ui->tblRoom->setColumnCount(1);
    std::sort(fRoomList.begin(), fRoomList.end(), CI_Romm_compare);
    for (int i = 0, count = fRoomList.count(); i < count; i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(fRoomList.at(i)));
        ui->tblRoom->setItem(i, 0, item);
    }
    int minimumHeight = ui->saRoom->height();
    int height = fRoomList.count() * ROW_HEIGHT;
    minimumHeight = minimumHeight < height ? height : minimumHeight;
    ui->tblRoom->setMinimumHeight(minimumHeight);
    ui->tblRoom->setMaximumHeight(minimumHeight);
    //ui->saMain->verticalScrollBar()->setValue(0);
    ui->g->setMinimumHeight(minimumHeight);
    ui->g->setMaximumHeight(minimumHeight);

    CacheReservation *cr = CacheReservation::instance();
    QMapIterator<QString, CI_Reservation*> itr = cr->it();
    while (itr.hasNext()) {
        itr.next();
        CI_Reservation *r = itr.value();
        if (fReserveStatus > 0) {
            if (r->fStatus != fReserveStatus) {
                continue;
            }
        }
        if (!fFilterCardex.isEmpty()) {
            if (!r->fCardex.contains(fFilterCardex, Qt::CaseInsensitive)) {
                continue;
            }
        }
        if (!fRoomIdList.contains(r->fRoom.toInt())) {
            continue;
        }
        switch (r->fState) {
        case RESERVE_CHECKIN:
        case RESERVE_RESERVE: {
            fScene->addReserveInfo(r);
            break;
        }
        case RESERVE_SERVICE:
        case RESERVE_OUTOFINVENTORY:
        case RESERVE_OUTOFROOM: {
            fScene->addReserveWidget(r);
            break;
        }
        }
    }
}

void WMainDesk::dockHint(const QString &filter)
{
    fDockHint->setWindowTitle(tr("Search for:") + " \"" + filter + "\"");
    fDockHint->commonFilter(filter);
    fDockHint->show();
    fDockHint->raise();
}

int WMainDesk::xFromDate(const QDate &date)
{
    int dayCount = fDateStart.daysTo(date);
    return dayCount * COLUMN_WIDTH;
}

void WMainDesk::roomsRowSelectionChanged()
{
    QModelIndexList indexes = ui->tblRoom->selectionModel()->selectedIndexes();
    QSet<int> selectedRows;
    for (QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); it++) {
        selectedRows.insert(it->row());
    }
    fScene->rowsSelect(selectedRows);
}

void WMainDesk::daysColumnSelectionChanged()
{
    QModelIndexList indexes = ui->tblDay->selectionModel()->selectedIndexes();
    QSet<int> selectedColumns;
    for (QModelIndexList::const_iterator it = indexes.begin(); it != indexes.end(); it++) {
        selectedColumns.insert(it->column());
    }
    fScene->columnsSelect(selectedColumns);
}

void WMainDesk::on_tblClasses_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    filterRoom();
}

void WMainDesk::on_tblBed_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    filterRoom();
}

void WMainDesk::on_btnClearFilter_clicked()
{
    ui->leJumpToRoom->clear();
    ui->tblBed->clearSelection();
    ui->tblClasses->clearSelection();
    ui->tblSmoke->clearSelection();
    ui->leFilterGuest->clear();
    ui->tblRoom->selectionModel()->clear();
    ui->tblDay->selectionModel()->clear();
    fDockHint->hide();
    fReserveStatus = 0;
    fFilterCardex = "";
    ui->btnStatus->setText(tr("Status"));
    filterRoom();
}

void WMainDesk::on_btnScrollRight_clicked()
{
    ui->saMain->horizontalScrollBar()->setValue(ui->saMain->horizontalScrollBar()->value() + 200);
}

void WMainDesk::on_btnScrollLeft_clicked()
{
    ui->saMain->horizontalScrollBar()->setValue(ui->saMain->horizontalScrollBar()->value() - 200);
}

void WMainDesk::on_btnCheckIn_clicked()
{
    WReservation *w = 0;
    QList<CI_Room*> rooms;
    rooms.append(0);
    w = addTab<WReservation>();
    w->setInitialParams(WORKING_DATE, WORKING_DATE, rooms);
}

void WMainDesk::on_btnClearSelection_clicked()
{
    if (message_confirm_tr("Are sure to delete draft reservations") == QDialog::Accepted) {
        fScene->clearReserveWidgets();
    }
}


void WMainDesk::on_btnGroupReservation_clicked()
{
    addTab<DlgGroupReservationFuck>();
}

void WMainDesk::on_btnScrollDown_clicked()
{
    ui->saMain->verticalScrollBar()->setValue(ui->saMain->verticalScrollBar()->value() + 200);
}

void WMainDesk::on_btnScrollUp_clicked()
{
    ui->saMain->verticalScrollBar()->setValue(ui->saMain->verticalScrollBar()->value() - 200);
}

void WMainDesk::on_btnEndOfDay_clicked()
{
    DlgEndOfDay *d = new DlgEndOfDay(this);
    if (d->exec() == QDialog::Accepted)  {
        loadReservationList();
    }
    delete d;
}

void WMainDesk::on_btnCheckin_clicked()
{
    fDockHint->checkInFilter();
    fDockHint->show();
    fDockHint->raise();
}

void WMainDesk::loadReservationList()
{
    fDbBind[":state1"] = RESERVE_CHECKIN;
    fDbBind[":state2"] = RESERVE_RESERVE;
    fDbBind[":state3"] = RESERVE_SERVICE;
    fReservationHint.clear();
    //"left join f_nationality n on n.f_short=g.f_nation "
    fDb.select("select r.f_state, r.f_id, r.f_room, group_concat(g1.gname separator ', '), "
               "r.f_startDate, r.f_endDate, concat(r.f_cardex, '-', cdx.f_name), '', '', r.f_invoice, "
               "concat(u.f_firstName, ' ', u.f_lastName), r.f_booking "
               "from f_reservation r "
               "left join (select f_reservation, concat(g.f_firstName, ' ' , g.f_lastName) as gname "
                    "from f_reservation_guests gr left join f_guests g on g.f_id=gr.f_guest) g1 on g1.f_reservation=r.f_id "

               "left join f_cardex cdx on cdx.f_cardex=r.f_cardex "
               "inner join users u on u.f_id=r.f_author "
               "where (r.f_state=:state1 or r.f_state=:state2 or r.f_state=:state3) "
               "group by r.f_id "
               "order by 1, 2 ", fDbBind, fDbRows);
    foreach_rows {
        QList<QVariant> v = *it;
        fReservationHint.append(v);
    }

    QTableWidget *t = fDockHint->tableWidget();
    t->setRowCount(fReservationHint.count());
    Utils::tableSetColumnWidths(t, 12,
                                30, 0, 60, 250, 120, 120, 70, 70, 30, 0, 200, 0);
    Utils::tableSetHeaderCaptions(t, 12,
        QString(), QString(), tr("Room"), tr("Guest"), tr("Arrival"), tr("Departure"), tr("Cardex"), tr("Nat."),
        QString(), QString(), tr("Author"), tr("Booking"));
    int row = 0;
    for (QList<QList<QVariant> >::const_iterator it = fReservationHint.begin(); it != fReservationHint.end(); it++) {
        for (int i = 0; i < it->count(); i++) {
           t->setItem(row, i, new QTableWidgetItem(Utils::variantToString(it->at(i))));
        }
        QTableWidgetItem *item = t->item(row, 0);
        item->setData(Qt::UserRole, item->text());
        item->setText("");
        QString pixmapName;
        switch (it->at(0).toInt()) {
        case RESERVE_CHECKIN:
            pixmapName = ":/images/ball-blue.png";
            break;
        case RESERVE_RESERVE:
            pixmapName = ":/images/ball-red.png";
            break;
        default:
            pixmapName = ":/images/ball-gray.png";
            break;
        }
        item->setData(Qt::DecorationRole, QPixmap(pixmapName));
        EPushButton *b = new EPushButton(this);
        b->setMinimumSize(25, 25);
        b->setMaximumSize(25, 25);
        b->setText("");
        b->setIcon(QIcon(":/images/goto.png"));
        b->setTag(row);
        connect(b, SIGNAL(clickedWithTag(int)), this, SLOT(btnDockHintGoToClicked(int)));
        t->setCellWidget(row, 8, b);
        row++;
    }
    ui->tblRoom->viewport()->update();
}

void WMainDesk::on_btnShowDockHint_clicked()
{
    if (!fDockHint->isVisible()) {
        dockHint("");
    }
}

void WMainDesk::on_tblSmoke_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    filterRoom();
}

void WMainDesk::on_btnJumpToDate_clicked()
{
    DlgChartDateRange *d = new DlgChartDateRange(this);
    if (d->exec() == QDialog::Accepted) {
        QDate date;
        d->getResult(date);
        int x = xFromDate(date);
        ui->saMain->horizontalScrollBar()->setValue(x);
    }
    delete d;
}

void WMainDesk::on_tblRoom_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    roomsRowSelectionChanged();
}

void WMainDesk::on_tblDay_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    daysColumnSelectionChanged();
}

void WMainDesk::on_btnCheckoutFilter_clicked()
{
    fDockHint->checkOutFilter();
    fDockHint->show();
    fDockHint->raise();
}

void WMainDesk::on_leJumpToRoom_editingFinished()
{
    filterRoom();
}

void WMainDesk::on_tblRoom_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    CI_Room *r = index.data(Qt::UserRole).value<CI_Room*>();
    if (r->fState == ROOM_STATE_CHECKIN) {
        message_info_tr("Changing the state of the room is not available in this case");
        return;
    }
    RoomState *rs = new RoomState(this);
    rs->setRoom(r);
    if (rs->exec() == QDialog::Accepted) {
        ui->tblRoom->viewport()->update();
    }
    delete rs;
}

void WMainDesk::on_btnInvoice_clicked()
{
    addTab<WInvoice>();
}

void WMainDesk::on_btnPostCharge_clicked()
{
    DlgPostingCharges *d = new DlgPostingCharges(this);
    d->exec();
    delete d;
}

void WMainDesk::on_btnReceipt_clicked()
{
    DlgReceiptVaucher *d = new DlgReceiptVaucher(this);
    d->exec();
    delete d;
}

void WMainDesk::on_btnTransferRoom_clicked()
{
    DlgTransferInvoiceAmount *d = new DlgTransferInvoiceAmount(this);
    d->exec();
    delete d;
}

void WMainDesk::on_btnFilterCardex_clicked()
{
    fDockCardex->show();
    fDockCardex->raise();
}

void WMainDesk::on_btnAirReserve_clicked()
{
    fDockHint->airFilter();
    fDockHint->show();
    fDockHint->raise();
}

void WMainDesk::on_leFilterGuest_textEdited(const QString &arg1)
{
    if (arg1.length() == 0) {
        fDockHint->hide();
    } else {
        dockHint(arg1);
    }
}

void WMainDesk::on_btnStatus_clicked()
{
    DlgReserveConfirmStatus *d = new DlgReserveConfirmStatus(this);
    int result = d->exec();
    delete d;
    if (result > 0) {
        fReserveStatus = result;
        switch (fReserveStatus) {
        case CONFIRM_BLOCK:
            ui->btnStatus->setText(tr("Block"));
            break;
        case CONFIRM_CONFIRM:
            ui->btnStatus->setText(tr("Confirm"));
            break;
        case CONFIRM_GUARANTEED:
            ui->btnStatus->setText(tr("Guaranted"));
            break;
        case CONFIRM_TENTATIVE:
            ui->btnStatus->setText(tr("Tentative"));
            break;
        case CONFIRM_WAITING:
            ui->btnStatus->setText(tr("Waiting"));
            break;
        default:
            ui->btnStatus->setText(tr("Status"));
            break;
        }

        filterRoom();
    }
}
