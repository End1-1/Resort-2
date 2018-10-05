#include "rdesk.h"
#include "ui_rdesk.h"
#include "rchangelanguage.h"
#include "rchangemenu.h"
#include "rmessage.h"
#include "rpaymenttype.h"
#include "cachecar.h"
#include "rselecttable.h"
#include "rdishcomment.h"
#include "dlgsmile.h"
#include "rtools.h"
#include "baseuid.h"
#include "rlogin.h"
#include "rnumbers.h"
#include "cacherights.h"
#include "rmodifiers.h"
#include "dlgreservation.h"
#include "pprintreceipt.h"
#include "ptextrect.h"
#include "pimage.h"
#include "pprintscene.h"
#include "dlgcomplexdish.h"
#include "cacheinvoiceitem.h"
#include "logging.h"
#include "cacherights.h"
#include "dlgpayment.h"
#include "cacheusers.h"
#include "dlggettext.h"
#include "reportprint.h"
#include "printtax.h"
#include "dlgselecttaxcashmode.h"
#include "cachecityledger.h"
#include "dlgvoidback.h"
#include "dlgcarselection.h"
#include "dlglist.h"
#include "rlogin.h"
#include "databaseresult.h"
#include "dlgdate.h"
#include "vauchers.h"
#include "dlgorders.h"
#include <QItemDelegate>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QScrollBar>

QMap<int, DishStruct*> RDesk::fQuickDish;

class PartItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, QBrush());
        if (!index.isValid()) {
            return;
        }
        DishPartStruct *p = index.data(Qt::UserRole).value<DishPartStruct*>();
        if (!p) {
            return;
        }
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(190, 240, 254, 255));
        }
        QFont f(qApp->font());
        f.setBold(true);
        painter->setFont(f);
        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QRect textRect = option.rect;
        textRect.adjust(3, 3, -3, -3);
        painter->drawText(textRect, p->fName[def_lang], o);
    }
};

class TypeItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, Qt::white);
        TypeStruct *t = index.data(Qt::UserRole).value<TypeStruct*>();
        if (!t) {
            return;
        }
        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        o.setWrapMode(QTextOption::WordWrap);
        painter->fillRect(option.rect, QColor::fromRgb(t->fBgColor));
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(42,42, 42));
            painter->setPen(Qt::white);
        } else {
            painter->setPen(QColor::fromRgb(t->fTextColor));
        }
        QRect textRect = option.rect;
        textRect.adjust(3, 3, -3, -3);
        QFont f(qApp->font());
        f.setPointSize(14);
        painter->setFont(f);
        painter->drawText(textRect, t->fName[def_lang], o);
    }
};

class DishItemDelegate : public QItemDelegate {
public:
    DishItemDelegate(RDesk *desk) {
        fDesk = desk;
    }
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->fillRect(option.rect, Qt::white);
        if (!index.isValid()) {
            return;
        }
        DishStruct *d = index.data(Qt::UserRole).value<DishStruct*>();
        if (!d) {
            return;
        }
        QTextOption o;
        o.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        o.setWrapMode(QTextOption::WordWrap);
        painter->fillRect(option.rect, QColor::fromRgb(d->fBgColor));
        QRect textRect = option.rect;
        textRect.adjust(3, 3, -3, -3);
        painter->setPen(QColor::fromRgb(d->fTextColor));
        QFont f(qApp->font());
        f.setPointSize(12);
        f.setBold(true);
        painter->setFont(f);
        QString text = QString("%1 [%2]")
                .arg(d->fName[def_lang])
                .arg(QString::number(d->fPrice, 'f', 0));
        painter->drawText(textRect, text, o);
    }
private:
    RDesk *fDesk;
};

class OrderDishDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->save();
        painter->fillRect(option.rect, Qt::white);
        if (!index.isValid()) {
            return;
        }
        OrderDishStruct *od = index.data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            return;
        }
        QFont f(qApp->font());
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor::fromRgb(42, 42, 42));
            painter->setPen(Qt::white);
            f.setBold(true);
            painter->setFont(f);
        } else {
            painter->setPen(QColor::fromRgb(42, 42, 42));
        }
        if (od->fState != DISH_STATE_READY) {
            f.setStrikeOut(true);
            painter->setFont(f);
        }
        QTextOption o;
        o.setWrapMode(QTextOption::WordWrap);
        switch (index.column()) {
        case 0: {
            QRect textRect = option.rect;
            textRect.adjust(2, 2, -2, -2);
            painter->drawText(textRect, od->fName[def_lang], o);
            f.setPointSize(8);
            painter->setFont(f);
            int h = QFontMetrics(f).height();
            QRect commentRect = textRect;
            commentRect.adjust(0, commentRect.height() - h - 1, 0, 0);
            painter->drawText(commentRect, od->fComment, o);
            if (od->fComplex > 0) {
                QRect imageRect = option.rect;
                imageRect.adjust(imageRect.width() - 20, imageRect.height() - 20, -5, -5);
                QImage img(":/images/dinner.png");
                painter->drawImage(imageRect, img);
            }
            break;
        }
        case 1: {
            o.setAlignment(Qt::AlignHCenter);
            QRect qtyRect = option.rect;
            qtyRect.adjust(2, 2, -2, (qtyRect.height() / 2) * -1);
            f.setBold(true);
            painter->setFont(f);
            painter->drawText(qtyRect, float_str(od->fQty, 1), o);

            QRect printRect = option.rect;
            printRect.adjust(2, (printRect.height() / 2), -2, -2);
            f.setBold(false);
            painter->setFont(f);
            painter->drawText(printRect, float_str(od->fQtyPrint, 1), o);

            painter->drawLine(qtyRect.left(), qtyRect.bottom(), qtyRect.right(), qtyRect.bottom());
            break;
        }
        case 2: {
            o.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            QRect totalRect = option.rect;
            totalRect.adjust(2, 2, -2, -2);
            painter->drawText(totalRect, float_str(od->fTotal, 0), o);
            break;
        }
        }
        painter->restore();
    }
};

class TotalItemDelegate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->setPen(Qt::white);
        QItemDelegate::paint(painter, option, index);

    }
};

class TablesItemDeletgate : public QItemDelegate {
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QItemDelegate::paint(painter, option, index);
        TableStruct *t = index.data(Qt::UserRole).value<TableStruct*>();
        if (!t) {
            return;
        }
        painter->fillRect(option.rect, Qt::black);
        painter->setPen(Qt::white);
        if (t->fAmount.toDouble() > 0.1) {
            painter->fillRect(option.rect, Qt::yellow);
            painter->setPen(Qt::black);
        }
        QTextOption o;
        o.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QRect r = option.rect;
        r.adjust(0, 0, 0, -20);
        painter->drawText(r, t->fName, o);
        r = option.rect;
        r.adjust(0, r.height() / 2, 0, 0);
        painter->drawText(r, t->fAmount, o);
    }
};

RDesk::RDesk(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RDesk)
{
    ui->setupUi(this);
    ui->tblComplex->setVisible(false);
    fCanClose = false;
    fShowRemoved = false;
    fTable  = 0;
    fCloseTimeout = 0;
    ui->tblType->setItemDelegate(new TypeItemDelegate());
    ui->tblDish->setItemDelegate(new DishItemDelegate(this));
    ui->tblOrder->setItemDelegate(new OrderDishDelegate());
    ui->tblTotal->setItemDelegate(new TotalItemDelegate());
    ui->tblTables->setItemDelegate(new TablesItemDeletgate());
    if (fQuickDish.count() == 0) {
//        fDbBind[":f_hall"] = def_default_hall
    }
    fTrackControl = new TrackControl(TRACK_REST_ORDER, 0, 0);
    fHall = Hall::getHallById(fPreferences.getDb(def_default_hall).toInt());
    /*
    fMenu = fHall->fDefaultMenu;
    setBtnMenuText();
    setupType(0);
    */
    ui->tblTables->setMaximumHeight((ui->tblTables->rowCount() * ui->tblTables->verticalHeader()->defaultSectionSize()) + 10);
    fPreferences.setDb(def_working_day, QDate::currentDate().toString(def_date_format));
    DatabaseResult dr;
    fDbBind[":f_comp"] = QHostInfo::localHostName();
    dr.select(fDb, "select f_key, f_value from r_config where f_comp=:f_comp", fDbBind);
    for (int i =0; i < dr.rowCount(); i++) {
        fPreferences.setDb(dr.value(i, "f_key").toString(), dr.value("f_value").toString());
    }
}

RDesk::~RDesk()
{
    delete ui;
    delete fStaff;
}

void RDesk::prepareToShow()
{
    showFullScreen();
    qApp->processEvents();
}

bool RDesk::setup(TableStruct *t)
{
    if (!t) {
        t = ui->tblTables->item(0, 0)->data(Qt::UserRole).value<TableStruct*>();
    }

    if (t) {
        fMenu = Hall::fHallMap[t->fHall]->fDefaultMenu;
        setBtnMenuText();
        setupType(0);
    }


    int colWidth = ui->tblDish->horizontalHeader()->defaultSectionSize();
    int colCount = ui->tblDish->width() / colWidth;
    int delta = ui->tblDish->width() - (colCount * colWidth);
    colWidth += (delta / colCount);
    ui->tblDish->horizontalHeader()->setDefaultSectionSize(colWidth);
    ui->tblDish->setColumnCount(colCount);

    ui->tblOrder->setColumnWidth(1, 40);
    ui->tblOrder->setColumnWidth(2, 60);
    ui->tblOrder->setColumnWidth(0, ui->tblOrder->width() - 104 - ui->tblOrder->verticalHeader()->width());

    ui->tblTotal->setColumnWidth(1, 70);
    ui->tblTotal->setColumnWidth(0, ui->tblTotal->width() - 72);

    bool result = false;
    if (t) {
        result = setTable(t);
    }
    connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    fTimer.start(10000);
    changeBtnState();
    return result;
}

void RDesk::setStaff(User *user)
{
    fStaff = user;
    ui->lbStaff->setText(user->fName);
}

void RDesk::showHideRemovedItems()
{
    fShowRemoved = !fShowRemoved;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        setOrderRowHidden(i, od);
    }
}

void RDesk::setOrderComment()
{
    QString comment;
    if (RDishComment::getComment(comment, this)) {
        fTable->fComment = comment;
        fDbBind[":f_comment"] = comment;
        fDb.update("o_header", fDbBind, QString("where f_id='%1'").arg(fTable->fOrder));
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, fStaff->fId, "Set order comment", comment, "");
    }
}

void RDesk::moveTable()
{
    int trackUser;
    if (!right(cr__o_movement, trackUser)) {
        return;
    }
    RSelectTable *t = new RSelectTable(this);
    t->setup(fTable->fHall);
    if (t->exec() == QDialog::Accepted) {
        TableStruct *table = t->table();
        if (table == fTable) {
            DlgSmile *ds = new DlgSmile(this);
            ds->exec();
            delete ds;
            return;
        }
        fDb.fDb.transaction();
        fDb.queryDirect(QString("select f_id from r_table where f_id in (%1, %2) for update")
                .arg(fTable->fId)
                .arg(table->fId));
        if (!table->fOrder.isEmpty()) {
            if (!message_question(tr("Destination table is not empty, continue with merge?"))) {
                return;
            }
            fDb.queryDirect(QString("update o_dish set f_header='%1' where f_header='%2'")
                            .arg(table->fOrder)
                            .arg(fTable->fOrder));
            fDb.queryDirect(QString("update o_header set f_state=%1 where f_id='%2'")
                            .arg(ORDER_STATE_MOVED)
                            .arg(fTable->fOrder));
            fTrackControl->addMessage(TRACK_REST_ORDER, table->fOrder, trackUser, "Table moved from",
                                      QString("%1/%2").arg(table->fName).arg(table->fOrder),
                                      QString("%1/%2").arg(fTable->fName).arg(fTable->fOrder));
        } else {
            fDbBind[":f_table"] = table->fId;
            fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
        }

        fDb.queryDirect(QString("update r_table set f_lockTime=0, f_lockHost='', f_order='' where f_id=%1")
                        .arg(fTable->fId));
        fDb.queryDirect(QString("update r_table set f_order='%1' where f_id=%2")
                        .arg(table->fOrder.isEmpty() ? fTable->fOrder : table->fOrder)
                        .arg(table->fId));
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Table moved to",
                                  QString("%1/%2").arg(fTable->fName).arg(fTable->fOrder),
                                  QString("%1/%2").arg(table->fName).arg(table->fOrder.isEmpty() ? fTable->fOrder : table->fOrder));
        fDb.fDb.commit();
        fTable->fOrder.clear();
        if (setTable(table)) {
            fHall = Hall::getHallById(fTable->fHall);
            if (fMenu != fHall->fDefaultMenu) {
                fMenu = fHall->fDefaultMenu;
                setBtnMenuText();
                setupType(0);
            }
        }
    }
    delete t;
    changeBtnState();
}

void RDesk::removeOrder()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_cancelation, trackUser)) {
        return;
    }
    if (message_question(tr("Confirm remove whole order")) != QDialog::Accepted) {
        return;
    }
    QString name, sql = "select f_id, f_en from o_dish_state where f_id in (2, 3)";
    QVariant result;
    if (!DlgList::getValue(tr("STORE OPTION"), name, result, sql)) {
        return;
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (od->fState == DISH_STATE_READY && od->fQtyPrint > 0.1) {
            fDbBind[":f_state"] = result.toInt();
            fDbBind[":f_cancelUser"] = trackUser;
            fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
            fDb.update("o_dish", fDbBind, where_id(ap(od->fRecId)));
            printRemovedDish(od, od->fQty - od->fQtyPrint, trackUser);
        } else if (od->fState == DISH_STATE_READY) {
            fDbBind[":f_state"] = DISH_STATE_EMPTY;
            fDbBind[":f_cancelUser"] = trackUser;
            fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
            fDb.update("o_dish", fDbBind, where_id(ap(od->fRecId)));
        }
        if (od->fState == DISH_STATE_READY) {
            QString dish = QString("%1, %2/%3")
                    .arg(od->fName["en"])
                    .arg(od->fQty)
                    .arg(od->fQtyPrint);
            fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Dish removed with order", dish, "");
        }
    }
    fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Order canceled", "", "");
    closeOrder(ORDER_STATE_REMOVED);
}

void RDesk::showTableOrders()
{

}

void RDesk::showMyTotal()
{
    fDbBind[":f_dateCash"] = fPreferences.getLocalDate(def_working_day);
    fDbBind[":f_staff"] = fStaff->fId;
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDb.select("select i.f_" + def_lang + ", sum(d.f_total) "
               "from o_dish d "
               "inner join o_header h on h.f_id=d.f_header "
               "inner join f_invoice_item i on i.f_id=h.f_paymentMode "
               "where h.f_dateCash=:f_dateCash and h.f_staff=:f_staff and h.f_state=:f_state "
               "group by 1", fDbBind, fDbRows);
    QString msg;
    foreach_rows {
        msg += it->at(0).toString() + " - " + it->at(1).toString() + "<br> ";
    }
    message_info(msg);

}

void RDesk::recover()
{
    int trackUser;
    if (!right(cr__o_recover_order, trackUser)){
        message_error(tr("Access denied"));
        return;
    }
    QString ordNum;
    if (!DlgGetText::getText(ordNum, "PS-")) {
        return;
    }
    if (ordNum.trimmed().isEmpty()) {
        return;
    }

    fDbBind[":f_id"] = ordNum;
    fDb.select("select f_staff, f_dateCash, f_total, f_table from o_header where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error(tr("Invalid order id"));
        return;
    }
    if (WORKING_DATE > fDbRows.at(0).at(1).toDate()) {
        message_error(tr("Order is not in current working date"));
        return;
    }
    int table = fDbRows.at(0).at(3).toInt();
    int staff = fDbRows.at(0).at(0).toInt();
    double amount = fDbRows.at(0).at(2).toDouble();
    fDbBind[":f_id"] = fDbRows.at(0).at(3).toInt();
    fDb.select("select f_order from r_table where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error(tr("Invalid order id"));
        return;
    }
    int orderId = fDbRows.at(0).at(0).toInt();
    if (orderId > 0) {
        message_error(tr("Table is busy, try again later"));
        return;
    }
    QString q = QString("Table %1<br>Staff %2<br>Amount %3")
            .arg(Hall::fTablesMap[table]->fName)
            .arg(CacheUsers::instance()->get(staff)->fFull)
            .arg(amount);
    if (message_question(q) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_order"] = ordNum;
    fDb.update("r_table", fDbBind, where_id(table));
    fDbBind[":f_state"] = ORDER_STATE_OPENED;
    fDb.update("o_header", fDbBind, where_id(ap(ordNum)));
    QString rrId = uuid(VAUCHER_RECOVER_N, fDb);
    fDbBind[":f_id1"] = rrId;
    fDbBind[":f_source1"] = VAUCHER_RECOVER_N;
    fDbBind[":f_itemCode"] = 41;
    fDbBind[":f_finalName"] = tr("RECOVER FOR ") + ordNum;
    fDbBind[":f_finance"] = 0;
    fDbBind[":f_id"] = ordNum;
    fDb.select("update m_register set f_id=:f_id1, f_source=:f_source1, f_finance=:f_finance, f_itemCode=:f_itemCode "
               "where f_id=:f_id", fDbBind, fDbRows);
    if (fTable) {
        if (table == fTable->fId) {
            fTable = 0;
            TableStruct *t = Hall::fTablesMap[table];
            setTable(t);
        }
    }
    message_info(tr("Table was successful recovered"));

}

void RDesk::initialCash()
{
    float num;
    if (RNumbers::getNumber(num, 100000, this)) {
        fDb.fDb.transaction();
        fDbBind[":f_date"] = fPreferences.getLocalDate(def_working_day);
        fDbBind[":f_staff"] = fStaff->fId;
        fDb.select("delete from o_initial_cash where f_date=:f_date and f_staff=:f_staff", fDbBind, fDbRows);
        fDbBind[":f_date"] = fPreferences.getLocalDate(def_working_day);
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_amount"] = num;
        fDb.insert("o_initial_cash", fDbBind);
        fDb.fDb.commit();
    }
}

void RDesk::setComplexMode()
{
    DishComplexStruct *dc = DlgComplexDish::complex(this);
    if (dc) {
        checkOrderHeader(fTable);
        fDbBind[":f_header"] = fTable->fOrder;
        fDbBind[":f_state"] = DISH_STATE_READY;
        fDbBind[":f_dish"] = dc->fId;
        fDbBind[":f_qty"] = dc->fQty;
        fDbBind[":f_qtyPrint"] = dc->fQty;
        fDbBind[":f_price"] = dc->fPrice;
        fDbBind[":f_svcValue"] = 0;
        fDbBind[":f_svcAmount"] = 0;
        fDbBind[":f_dctValue"] = 0;
        fDbBind[":f_dctAmount"] = 0;
        fDbBind[":f_total"] = dc->fPrice;
        fDbBind[":f_print1"] = "";
        fDbBind[":f_print2"] = "";
        fDbBind[":f_store"] = 0;
        fDbBind[":f_comment"] = "";
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_complex"] = dc->fId;
        fDbBind[":f_complexId"] = dc->fId;
        fDbBind[":f_adgt"] = dc->fAdgt;
        dc->fRecId = uuid("DR", fDb);
        fDb.insertId("o_dish", dc->fRecId);
        if (dc->fRecId.isEmpty()) {
            message_error("Application will quit due an program error.");
            qApp->quit();
            return;
        }
        fDb.update("o_dish", fDbBind, where_id(ap(dc->fRecId)));
        fTrackControl->addMessage("New complex begin", dc->fName["en"], "----", true);
        for (int i = 0; i < dc->fDishes.count(); i++) {
            dc->fDishes[i]->fComplexRec = dc->fRecId;
            dc->fDishes[i]->fComplex = dc->fId;
            //dc->fDishes[i]->fPrice = 0;
            addDishToOrder(dc->fDishes[i]);
        }
        ui->tblComplex->setRowCount(ui->tblComplex->rowCount() + 1);
        ui->tblComplex->setItem(ui->tblComplex->rowCount() - 1, 0, new QTableWidgetItem());
        ui->tblComplex->item(ui->tblComplex->rowCount() - 1, 0)->setData(Qt::UserRole, qVariantFromValue(dc));
        countTotal();
        fTrackControl->addMessage("New complex end", dc->fName["en"], "----", true);
    }
}

void RDesk::closeOrder(int state)
{
    BaseOrder bo(fTable->fOrder);
    bo.calculateOutput();
    fDbBind[":f_state"] = state;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    fDbBind[":f_dateClose"] = QDateTime::currentDateTime();
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fDbBind[":f_order"] = "";
    fDb.update("r_table", fDbBind, where_id(ap(fTable->fId)));
    clearOrder();
}

void RDesk::printTotalToday()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__o_print_reports, trackUser)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotal(WORKING_DATE, userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalYesterday()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__o_print_reports, trackUser)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotal(WORKING_DATE.addDays(-1), userName, fHall->fReceiptPrinter);
}

void RDesk::printTotalAnyDay()
{
    int trackUser;
    QString userName = fStaff->fName;
    if (!right(cr__print_reports_any_day, trackUser)) {
        return;
    }
    QDate date;
    if (!DlgDate::getDate(date)) {
        return;
    }
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    }
    ReportPrint::printTotal(date, userName, fHall->fReceiptPrinter);
}

void RDesk::printTax(int cashMode)
{
    //Print tax
    if (fPreferences.getDb(def_tax_port).toInt() == 0) {
        message_error(tr("Setup tax printer first"));
        return;
    }
    PrintTax *pt = new PrintTax(this);
    double total = 0;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (od->fComplex > 0) {
            continue;
        }
        total += od->fTotal;
        pt->fRecList.append(QString::number(i));
        pt->fDept.append(fHall->fVatDept);
        pt->fAdgCode.append(od->fAdgt);
        pt->fCodeList.append(QString::number(od->fDishId));
        pt->fNameList.append(od->fName["am"]);
        pt->fQtyList.append(float_str(od->fQty, 1));
        pt->fPriceList.append(float_str(od->fPrice, 2));
        pt->fTaxNameList.append(od->fName["am"]);
    }
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc ==0, print tax");
            return;
        }
        total += dc->fPrice * dc->fQty;
        pt->fRecList.append(dc->fRecId);
        pt->fDept.append(fHall->fVatDept);
        pt->fAdgCode.append(dc->fAdgt);
        pt->fCodeList.append(QString::number(dc->fId));
        pt->fNameList.append(dc->fName["am"]);
        pt->fQtyList.append(float_str(dc->fQty, 1));
        pt->fPriceList.append(float_str(dc->fPrice, 2));
        pt->fTaxNameList.append(dc->fName["am"]);
    }
    fTable->fTaxPrint = 1;
    fDbBind[":f_tax"] = 1;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    pt->fInvoice = fTable->fOrder;
    pt->build();
    switch (cashMode) {
    case tax_mode_cash:
        pt->fAmountCash = float_str(total, 2);
        pt->fAmountCard = "0";
        break;
    case tax_mode_card:
        pt->fAmountCard = float_str(total, 2);
        pt->fAmountCash = "0";
        break;
    }
    pt->print();
    delete pt;

    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (od->fComplex > 0) {
            continue;
        }
        fDbBind[":f_vaucher"] = fTable->fOrder;
        fDbBind[":f_invoice"] = "";
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_name"] = od->fName["en"];
        fDbBind[":f_amountCash"] = cashMode == tax_mode_cash ? total : 0;
        fDbBind[":f_amountCard"] = cashMode == tax_mode_cash ? 0 : total;
        fDbBind[":f_amountPrepaid"] = 0;
        fDbBind[":f_user"] = fStaff->fId;
        fDb.insert("m_tax_history", fDbBind);
    }
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc ==0, print tax");
            return;
        }
        fDbBind[":f_vaucher"] = fTable->fOrder;
        fDbBind[":f_invoice"] = 0;
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_name"] = dc->fName["en"];
        fDbBind[":f_amountCash"] = cashMode == tax_mode_cash ? total : 0;
        fDbBind[":f_amountCard"] = cashMode == tax_mode_cash ? 0 : total;
        fDbBind[":f_amountPrepaid"] = 0;
        fDbBind[":f_user"] = fStaff->fId;
        fDb.insert("m_tax_history", fDbBind);
    }
}

void RDesk::printTaxDialog()
{
    int trackUser = fStaff->fId;
    if (!right(cr__print_any_tax, trackUser)) {
        return;
    }
    DlgOrders *d = new DlgOrders(fStaff->fId, this);
    d->exec();
    delete d;
}

void RDesk::printReceiptByNumber()
{
    int trackUser = fStaff->fId;
    if (!right(cr__print_receipt_by_umber, trackUser)) {
        return;
    }
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    } else {
        userName = "#Username Error";
    }
    QString ordNum;
    if (!DlgGetText::getText(ordNum, "PS-")) {
        return;
    }
    if (ordNum.trimmed().isEmpty()) {
        return;
    }
    PPrintReceipt::printOrder(fHall->fReceiptPrinter, ordNum, trackUser);

}

void RDesk::voidBack()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_cancelation, trackUser)) {
        return;
    }
    DlgVoidBack::recover();
    if (fTable) {
        loadOrder();
    }
}

void RDesk::printVoidReport()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_print_reports, trackUser)) {
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    dr.select(fDb, "select oh.f_id, h.f_name as hname, t.f_name as tname, concat(u1.f_firstName, ' ' , u1.f_lastName) as staff, \
              ds.f_en as state, d.f_en as dish, od.f_qty, od.f_total, \
              concat(u2.f_firstName, ' ', u2.f_lastName) as staffcancel \
              from o_dish od \
              left join o_header oh on oh.f_id=od.f_header \
              left join r_hall h on h.f_id=oh.f_hall \
              left join o_dish_state ds on ds.f_id=od.f_state \
              left join r_table t on t.f_id=oh.f_table \
              left join users u1 on u1.f_id=oh.f_staff \
              left join r_dish d on d.f_id=od.f_dish \
              left join users u2 on u2.f_id=od.f_cancelUser \
              where od.f_state in (2, 3) and oh.f_dateCash=:f_dateCash \
              order by 1", fDbBind);

    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 30);
    th.setTextAlignment(Qt::AlignHCenter);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(200, top, 400, 250));
    top += 250;
    top += ps->addTextRect(new PTextRect(10, top, 750, rowHeight, QString("%1").arg(tr("VOID REPORT")), &th, f))->textHeight();
    top += ps->addTextRect(10, top, 680, rowHeight, WORKING_DATE.toString(def_date_format), &th)->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Printed by ") + CacheUsers::instance()->get(trackUser)->fFull, &th)->textHeight();
    top += ps->addTextRect(0, top, 680, rowHeight, tr("Date") + ": " + WORKING_DATE.toString(def_date_format), &th)->textHeight();
    ps->addLine(10, top, 750, top);

    th.setTextAlignment(Qt::AlignLeft);

    QPen dotPen(Qt::DotLine);
    for (int i = 0; i < dr.rowCount(); i++) {
        QString row = QString("%1 / %2 / #%3")
                .arg(dr.value(i, "hname").toString())
                .arg(dr.value(i, "tname").toString())
                .arg(dr.value(i, "f_id").toString());
        top += ps->addTextRect(10, top, 680, rowHeight, row, &th)->textHeight();
        ps->addLine(10, top, 680, top, dotPen);
        row = QString ("%1 / %2 / %3")
                .arg(dr.value(i, "dish").toString())
                .arg(dr.value(i, "f_qty").toString())
                .arg(dr.value(i, "f_total").toString());
        top += ps->addTextRect(10, top, 680, rowHeight, row, &th)->textHeight();
        row = QString("%1").arg(dr.value(i, "staff").toString());
        top += ps->addTextRect(10, top, 680, rowHeight, row, &th)->textHeight();
        top += ps->addTextRect(10, top, 680, rowHeight, tr("Type: ") + dr.value(i, "state").toString(), &th)->textHeight();
        top += ps->addTextRect(10, top, 680, rowHeight, tr("Manager"), &th)->textHeight();
        top += ps->addTextRect(10, top, 680, rowHeight, dr.value(i, "staffcancel").toString(), &th)->textHeight();
        top ++;
        ps->addLine(10, top, 680, top, dotPen);
        top += 2;
        ps->addLine(10, top, 680, top, dotPen);
        top += rowHeight;
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }

    ps->addTextRect(10, top, 300, rowHeight, "_", &th);


    QPrinter printer;
    printer.setPrinterName(fHall->fReceiptPrinter);
    QMatrix m;
    m.scale(3, 3);
    QPainter painter(&printer);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }
    qDeleteAll(lps);
}

void RDesk::complimentary()
{
    int trackUser = fStaff->fId;
    if (!right(cr__o_complimentary, trackUser)) {
        return;
    }
    QString comment, sql = "select f_id, f_name from r_complimentary_comment";
    QVariant result;
    if (!DlgList::getValue(tr("Complimentary comment"), comment, result, sql)) {
        return;
    }
    fDbBind[":f_paymentMode"] = PAYMENT_COMPLIMENTARY;
    fDbBind[":f_paymentModeComment"] =  comment;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fTable->fPaymentComment = comment;
    fTable->fPaymentMode = PAYMENT_COMPLIMENTARY;
    printReceipt(true);
    closeOrder();
}

void RDesk::changePassword()
{
    QString login;
    if (RLogin::getLogin(login, tr("Login"), this)) {
        QString query = QString("update users set f_altPassword='' where f_altPassword=md5('%1')")
                .arg(login);
        fDb.queryDirect(query);
        query = QString("update users set f_altPassword=md5('%1') where f_id=%2")
                .arg(login)
                .arg(fStaff->fId);
        fDb.queryDirect(query);
        message_info(tr("Your password was changed"));
    }
}

void RDesk::openTools()
{
    RTools *t = new RTools(this);
    t->setNoTable();
    if (t->exec() == QDialog::Accepted) {

    }
    delete t;
}

void RDesk::closeDay()
{
    fDbBind[":f_date"] = WORKING_DATE;
    fDbBind[":f_docType"] = 2;
    fDb.select("delete from c_cash where f_date=:f_date and f_docType=:f_docType", fDbBind, fDbRows);
    fDbBind[":f_date"] = WORKING_DATE;
    fDbBind[":f_docType"] = 3;
    fDb.select("delete from c_cash where f_date=:f_date and f_docType=:f_docType", fDbBind, fDbRows);
    DatabaseResult dtotal;
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDbBind[":f_dateCash"] = WORKING_DATE;
    dtotal.select(fDb, "select sum(h.f_cash+h.f_card) as f_total from o_header_payment h "
                       "left join o_Header o on o.f_id=h.f_id "
                       "where o.f_state=:f_state and o.f_dateCash=:f_dateCash", fDbBind);
    if (dtotal.rowCount() > 0) {
        fDbBind[":f_date"] = WORKING_DATE;
        fDbBind[":f_docType"] = 2;
        fDbBind[":f_debit"] = 1;
        fDbBind[":f_credit"] = 1;
        fDbBind[":f_amount"] = dtotal.value("f_total");
        fDb.insert("c_cash", fDbBind);

        DatabaseResult drsal;
        fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
        fDbBind[":f_state2"] = DISH_STATE_READY;
        fDbBind[":f_dateCash"] = WORKING_DATE;
        fDbBind[":f_store"] = 2;
        drsal.select(fDb, "select sum(d.f_total) as f_total from o_dish d "
                     "left join o_header h on h.f_id=d.f_header "
                     "where h.f_state=:f_state1 and d.f_state=:f_state2 "
                     "and h.f_dateCash=:f_dateCash and d.f_store=:f_store", fDbBind);

        if (drsal.value("f_total").toDouble() > 0.1) {
            fDbBind[":f_date"] = WORKING_DATE;
            fDbBind[":f_docType"] = 3;
            fDbBind[":f_debit"] = 1;
            fDbBind[":f_credit"] = 2;
            fDbBind[":f_amount"] = drsal.value("f_total").toDouble() * -1 * 0.4;
            fDb.insert("c_cash", fDbBind);
        }
    }
    printTotalToday();
}

void RDesk::closeEvent(QCloseEvent *e)
{
    if (!fCanClose) {
        e->ignore();
        return;
    }
    checkEmpty();
    BaseExtendedDialog::closeEvent(e);
}

void RDesk::onBtnQtyClicked()
{
    fCloseTimeout = 0;
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    QPushButton *b = static_cast<QPushButton*>(sender());
    float qty;
    if (b->text() == "-0.5") {
        qty = -0.5;
    } else if (b->text() == "+0.5") {
        qty = 0.5;
    } else {
        qty = b->text().toFloat();
    }
    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
    if (!od) {
        return;
    }
    if (od->fState != DISH_STATE_READY) {
        message_error(tr("You cannot edit the quantity of selected item"));
        return;
    }
    if (od->fQty + qty < 0.1) {
        return;
    }
    if (od->fComplex == 0) {
        QString oldQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                .arg(od->fQty)
                .arg(od->fQtyPrint);
        if (od->fQtyPrint < 0.01) {
            if (qty > 0.9) {
                if (qty < 10.0) {
                    od->fQty = qty;
                } else {
                    od->fQty += qty;
                }
            } else {
                od->fQty += qty;
            }
        } else {
            od->fQty += qty;
        }
        QString newQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                .arg(od->fQty)
                .arg(od->fQtyPrint);
        countDish(od);
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, fStaff->fId, "Dish qty", oldQty, newQty);
        updateDish(od);
    } else {
        double complexQty = 0;
        DishComplexStruct *dc = 0;
        for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
            DishComplexStruct *dt = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
            if (dt->fRecId == od->fComplexRecId) {
                dc = dt;
                break;
            }
        }
        if (!dc) {
            message_error("Application error. Contact to developer, message: dc ==0, dish qty");
            return;
        }
        QString oldQty = QString("%1, %2").arg(dc->fName[def_lang])
                .arg(dc->fQty);
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (od->fComplexRecId != dc->fRecId) {
                continue;
            }
            QString odOldQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                    .arg(od->fQty)
                    .arg(od->fQtyPrint);
            if (od->fQtyPrint < 0.01) {
                if (qty > 0.9){
                    od->fQty = qty;
                } else {
                    od->fQty += qty;
                }
            } else {
                od->fQty += qty;
            }
            complexQty = od->fQty;
            QString odNewQty = QString("%1, %2 / %3").arg(od->fName[def_lang])
                    .arg(od->fQty)
                    .arg(od->fQtyPrint);
            fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, fStaff->fId, "Dish qty", odOldQty, odNewQty);
            countDish(od);
            updateDish(od);
        }
        dc->fQty = complexQty;
        fDbBind[":f_qty"] = dc->fQty;
        fDbBind[":f_total"] = dc->fPrice * dc->fQty;
        fDb.update("o_dish", fDbBind, where_id(ap(dc->fRecId)));
        QString newQty = QString("%1, %2").arg(dc->fName[def_lang])
                .arg(dc->fQty);
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, fStaff->fId, "Complex qty", oldQty, newQty);
    }
    resetPrintQty();
    ui->tblOrder->viewport()->update();
    countTotal();
    changeBtnState();
}

void RDesk::timeout()
{
    if (fTable) {
        QString query = QString("update r_table set f_lockTime=%1, f_lockHost='%2' where f_id=%3")
                .arg(QDateTime::currentDateTime().toTime_t())
                .arg(Utils::hostName())
                .arg(fTable->fId);
        fDb.queryDirect(query);
    }
    fCloseTimeout++;
    if (fCloseTimeout > 5) {
        if (defrest(dr_open_table_after_run).toInt() == 0) {
            on_btnExit_clicked();
        }
    }
}

void RDesk::on_btnExit_clicked()
{
    if (message_question(tr("Confirm to close application")) != QDialog::Accepted) {
        return;
    }
    if (fTable) {
        fDb.fDb.transaction();
        fDbBind[":f_id"] = fTable->fId;
        if (fDb.select("select f_id from r_table where f_id=:f_id for update", fDbBind, fDbRows) == -1) {
            message_error(tr("Cannot close current table, try later"));
            return;
        }
        fDbBind[":f_lockTime"] = 0;
        fDbBind[":f_lockHost"] = "";
        fDb.update("r_table", fDbBind, QString("where f_id=%1").arg(fTable->fId));
        fDb.fDb.commit();
    }
    fCanClose = true;
    close();
}

void RDesk::on_btnLanguage_clicked()
{
    if (RChangeLanguage::changeLanguage(this)) {
        ui->tblType->viewport()->update();
        ui->tblDish->viewport()->update();
        ui->tblOrder->viewport()->update();
    }
}

void RDesk::on_btnMenu_clicked()
{
    int newMenu;
    if (RChangeMenu::changeMenu(fMenu, newMenu, this)) {
        fMenu = newMenu;
        setBtnMenuText();
        setupType(0);
    }
}

void RDesk::setBtnMenuText()
{
    if (fMenu == 0) {
        message_error(tr("Default menu is not set"));
        return;
    }
}

void RDesk::setupType(int partId)
{
    writelog("RDesk::setupType start.");
    QMap<int, TypeStruct*> type;
    fDishTable.filterType(fMenu, partId, type);
    ui->tblDish->clear();
    ui->tblDish->setRowCount(0);
    ui->tblType->clear();
    ui->tblType->setColumnCount(2);
    ui->tblType->horizontalHeader()->setDefaultSectionSize((ui->tblType->width() - 10)  /2);
    int row = -1, col = 3;
    for (QMap<int, TypeStruct*>::const_iterator it = type.begin(); it != type.end(); it++) {
        if (col > ui->tblType->columnCount() - 1) {
            col = 0;
            row ++;
            ui->tblType->setRowCount(row + 1);
        }
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(it.value()));
        ui->tblType->setItem(row, col++, item);
    }
    writelog("RDesk::setupType end.");
}

void RDesk::setupDish(int typeId)
{
    ui->tblDish->clear();
    QMap<int, DishStruct*> dish;
    fDishTable.filterDish(fMenu, typeId, dish);
    int rowCount = (dish.count() / ui->tblDish->columnCount());
    if (dish.count() % ui->tblDish->columnCount() > 0) {
        rowCount++;
    }
    ui->tblDish->setRowCount(rowCount);
    int col = 0, row = 0;
    for (QMap<int, DishStruct*>::const_iterator it = dish.begin(); it != dish.end(); it++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(*it));
        ui->tblDish->setItem(row, col++, item);
        if (col == ui->tblDish->columnCount()) {
            row++;
            col = 0;
        }
    }
}

void RDesk::addDishToOrder(DishStruct *d)
{
    checkOrderHeader(fTable);
    OrderDishStruct *od = new OrderDishStruct();
    if (d->fMod.count() > 0) {
        QStringList mods;
        for (QList<QMap<QString, QString> >::const_iterator it = d->fMod.begin(); it != d->fMod.end(); it++) {
            mods.append(it->value(def_lang));
        }
        RModifiers *m = new RModifiers(this);
        m->setModifiers(mods);
        if (m->exec() == QDialog::Accepted) {
            od->fComment = m->mod();
        }
        delete m;
    }
    od->fDishId = d->fId;
    od->fState = DISH_STATE_READY;
    od->fPrint1 = d->fPrint1;
    od->fPrint2 = d->fPrint2;
    od->fStore = d->fStore;
    od->fName = d->fName;
    od->fPrice = d->fPrice;
    od->fSvcValue = fHall->fDefaultSvcValue;
    od->fSvcAmount = od->fSvcValue * od->fTotal;
    od->fDctValue = 0;
    od->fDctAmount = 0;
    od->fQty = 1;
    od->fQtyPrint = 0;
    od->fComplex = d->fComplex;
    od->fComplexRecId = d->fComplexRec;
    od->fAdgt = d->fAdgt;

    countDish(od);
    fDbBind[":f_header"] = fTable->fOrder;
    fDbBind[":f_state"] = DISH_STATE_READY;
    fDbBind[":f_dish"] = od->fDishId;
    fDbBind[":f_qty"] = od->fQty;
    fDbBind[":f_qtyPrint"] = od->fQtyPrint;
    fDbBind[":f_price"] = od->fPrice;
    fDbBind[":f_svcValue"] = od->fSvcValue;
    fDbBind[":f_svcAmount"] = od->fSvcAmount;
    fDbBind[":f_dctValue"] = od->fDctValue;
    fDbBind[":f_dctAmount"] = od->fDctAmount;
    fDbBind[":f_total"] = od->fTotal;
    fDbBind[":f_totalUSD"] = od->fTotal / def_usd;
    fDbBind[":f_print1"] = od->fPrint1;
    fDbBind[":f_print2"] = od->fPrint2;
    fDbBind[":f_store"] = od->fStore;
    fDbBind[":f_comment"] = od->fComment;
    fDbBind[":f_staff"] = fStaff->fId;
    fDbBind[":f_complex"] = od->fComplex;
    fDbBind[":f_complexId"] = 0;
    fDbBind[":f_adgt"] = od->fAdgt;
    fDbBind[":f_complexRec"] = od->fComplexRecId;
    od->fRecId = intid("DR", fDb);
    fDb.insertId("o_dish", od->fRecId);
    fDb.update("o_dish", fDbBind, where_id(ap(od->fRecId)));
    updateDishQtyHistory(od);
    addDishToTable(od);
    resetPrintQty();
    fTrackControl->addMessage("New dish", od->fName["en"], "", true);
}

void RDesk::addDishToTable(OrderDishStruct *od)
{
    int row = ui->tblOrder->rowCount();
    ui->tblOrder->setRowCount(row + 1);
    for (int i = 0; i < 3; i++) {
        ui->tblOrder->setItem(row, i, new QTableWidgetItem());
    }
    ui->tblOrder->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(od));
    ui->tblOrder->item(row, 1)->setData(Qt::UserRole, qVariantFromValue(od));
    ui->tblOrder->item(row, 2)->setData(Qt::UserRole, qVariantFromValue(od));
    ui->tblOrder->setCurrentCell(row, 0);
    setOrderRowHidden(row, od);
    countTotal();
    changeBtnState();
}

void RDesk::updateDish(OrderDishStruct *od)
{
    fDbBind[":f_state"] = od->fState;
    fDbBind[":f_qty"] = od->fQty;
    fDbBind[":f_qtyPrint"] = od->fQtyPrint;
    fDbBind[":f_price"] = od->fPrice;
    fDbBind[":f_svcValue"] = od->fSvcValue;
    fDbBind[":f_svcAmount"] = od->fSvcAmount;
    fDbBind[":f_dctValue"] = od->fDctValue;
    fDbBind[":f_dctAmount"] = od->fDctAmount;
    fDbBind[":f_total"] = od->fTotal;
    fDbBind[":f_totalUSD"] = od->fTotal / def_usd;
    fDbBind[":f_comment"] = od->fComment;
    fDbBind[":f_cancelUser"] = od->fCancelUser;
    fDbBind[":f_cancelDate"] = od->fCancelDate;
    fDb.update("o_dish", fDbBind, QString("where f_id='%1'").arg(od->fRecId));
    updateDishQtyHistory(od);
}

double RDesk::countTotal()
{
    double total = 0;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (!od->fComplexRecId.isEmpty() ) {
            continue;
        }
        total += od->fTotal;
    }
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc == 0, countTotal");
            return -1;
        }
        total += dc->fPrice * dc->fQty;
    }
    double grandTotal = total ;
    ui->tblTotal->item(1, 1)->setData(Qt::EditRole, float_str(grandTotal, 0));
    fDbBind[":f_total"] = grandTotal;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    fTable->fAmount = float_str(grandTotal, 0);
    updateTableInfo();
    return grandTotal;
}

void RDesk::countDish(OrderDishStruct *d)
{
    d->fTotal = d->fQty * d->fPrice;
    d->fSvcAmount += (d->fTotal * d->fSvcValue);
    d->fTotal += d->fSvcAmount;
    d->fDctAmount = d->fTotal * d->fDctValue;
    d->fTotal -= d->fDctAmount;
}

bool RDesk::setTable(TableStruct *t)
{
    if (fTable) {
        if (t == fTable) {
            DlgSmile *ds = new DlgSmile(this);
            ds->exec();
            delete ds;
            return true;
        }
    }
    if (t == 0) {
        clearOrder();
        return false;
    }
    fCarModel.clear();
    fCarGovNum.clear();
    ui->lbCar->clear();
    Splash s(this);
    s.show();
    if (fTable) {
        //unlock previous table
        checkEmpty();
        QString query = QString("update r_table set f_lockTime=0, f_lockHost='' where f_id=%1")
                .arg(fTable->fId);
        fDb.queryDirect(query);
    }
    fTable = t;
    s.setText(tr("Opening table ") + t->fName);
    ui->tblTotal->item(0, 1)->setText(t->fName);
    ui->tblTotal->item(1, 1)->setText("0");
    fDb.fDb.transaction();
    fDbBind[":f_id"] = t->fId;
    if (fDb.select("select f_lockTime, f_lockHost, f_order from r_table where f_id=:f_id for update", fDbBind, fDbRows) == -1) {
        if (fDb.fLastError.contains("Lock wait timeout exceeded")) {
            fDb.fDb.commit();
            s.close();
            message_error(tr("Table in editing mode"));
            fCanClose = true;
            close();
            return false;
        }
    }
    QString lockHost = fDbRows.at(0).at(1).toString();
    int lockTime = fDbRows.at(0).at(0).toInt();
    int currTime = QDateTime::currentDateTime().toTime_t();
    if (lockTime > 0) {
        if (currTime - lockTime < 1 * 60 * 3) {
            if (lockHost != Utils::hostName()) {
                s.close();
                message_error(tr("Table in editing mode"));
                fCanClose = true;
                close();
                return false;
            }
        }
    }
    clearOrder();
    fTable->fOrder = fDbRows.at(0).at(2).toString();
    QString query = QString("update r_table set f_lockTime=%1, f_lockHost='%2' where f_id=%3")
            .arg(QDateTime::currentDateTime().toTime_t())
            .arg(Utils::hostName())
            .arg(fTable->fId);
    fDb.queryDirect(query);
    fDb.fDb.commit();
    if (!fTable->fOrder.isEmpty()) {
        loadOrder();
    }
    fHall = Hall::getHallById(fTable->fHall);
    ui->lbCar->setText(fCarModel + " " + fCarGovNum);
    s.close();
    return true;
}

void RDesk::checkOrderHeader(TableStruct *t)
{
    if (t->fOrder.isEmpty()) {
        fDb.fDb.transaction();
        QString query = QString("select f_id from r_table where f_id='%1' for update")
                .arg(t->fId);
        fDb.queryDirect(query);
        t->fOrder = intid(VAUCHER_POINT_SALE_N, fDb);
        fDb.insertId("o_header", t->fOrder);
        fDbBind[":f_state"] = ORDER_STATE_OPENED;
        fDbBind[":f_table"] = t->fId;
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_dateOpen"] = QDateTime::currentDateTime();
        fDbBind[":f_dateCash"] = WORKING_DATE;
        fDbBind[":f_tax"] = 0;
        fDbBind[":f_paymentMode"] = PAYMENT_CASH;
        fDbBind[":f_hall"] = t->fHall;
        t->fOpened = fDbBind[":f_dateOpen"].toDateTime();
        fDb.update("o_header", fDbBind, where_id(ap(t->fOrder)));
        fDbBind[":f_order"] = t->fOrder;
        fDb.update("r_table", fDbBind, QString("where f_id=%1").arg(t->fId));
        fDb.fDb.commit();
        fTrackControl->resetChanges(t->fOrder);
        fTrackControl->fUser = fStaff->fId;
        fTrackControl->addMessage("Order started", "", "", true);
        ui->tblTables->viewport()->update();
        if (fTable->fHall == 1) {
            int model;
            QString govNum;
            DlgCarSelection::selectCar(model, govNum);
            fDbBind[":f_model"] = model;
            fDbBind[":f_order"] = t->fOrder;
            fDbBind[":f_govNumber"] = govNum;
            fDb.insert("o_car", fDbBind);
            fCarGovNum = govNum;
            fDbBind[":f_id"] = model;
            DatabaseResult dr;
            dr.select(fDb, "select concat(f_model, ' ', f_class) as car from d_car_model where f_id=:f_id", fDbBind);
            if (dr.rowCount() > 0) {
                fCarModel = dr.value("car").toString();
                ui->lbCar->setText(fCarModel + " " + fCarGovNum);
            }
        }
    }
}

void RDesk::clearOrder()
{
    if (!fTable) {
        return;
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        delete ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
    }
    fTable->fPrint = 0;
    ui->tblOrder->clear();
    ui->tblOrder->setRowCount(0);
    fTable->fOrder = "";
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
        for (int j = 0; j < dc->fDishes.count(); j++) {
            delete dc->fDishes[j];
        }
        delete dc;
    }
    ui->tblComplex->setRowCount(0);
    ui->lbCar->clear();
    countTotal();
}

void RDesk::loadOrder()
{
    ui->tblComplex->clear();
    ui->tblComplex->setRowCount(0);
    //ui->tblOrder->clear();
    ui->tblOrder->setRowCount(0);
    User u;
    QString query = QString("select h.f_staff, h.f_comment, concat(u.f_firstName, ' ', u.f_lastName), h.f_print, "
                            "h.f_paymentMode, h.f_paymentModeComment, h.f_cityLedger, h.f_roomComment, h.f_dateOpen, "
                            "h.f_tax "
                             "from o_header h "
                             "left join users u on u.f_id=h.f_staff "
                             "where h.f_id='%1'")
            .arg(fTable->fOrder);
    fDb.select(query, fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        return;
    }
    u.fId = fDbRows.at(0).at(0).toInt();
    u.fName = fDbRows.at(0).at(2).toString();
    fTable->fComment = fDbRows.at(0).at(1).toString();
    fTable->fPrint = fDbRows.at(0).at(3).toInt();
    fTable->fPaymentMode = fDbRows.at(0).at(4).toInt();
    fTable->fPaymentComment = fDbRows.at(0).at(5).toString();
    fTable->fCitiLedger = fDbRows.at(0).at(6).toInt();
    fTable->fRoomComment = fDbRows.at(0).at(7).toString();
    fTable->fOpened = fDbRows.at(0).at(8).toDateTime();
    fTable->fTaxPrint = fDbRows.at(0).at(9).toInt();
    if (u.fId != fStaff->fId) {
        message_info(QString("%1<br>%2")
                     .arg(tr("Table opened by"))
                     .arg(u.fName));
    }
    //Complex
    query = "select od.f_id, od.f_dish, dc.f_en, dc.f_ru, dc.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
            "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
            "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
            "dc.f_adgt "
            "from o_dish od "
            "left join r_dish_complex dc on dc.f_id=od.f_complexId "
            "where od.f_header=:f_header and f_complex>0 and f_complexId>0 and f_state=1 "
            "order by od.f_id ";
    fDbBind[":f_header"] = fTable->fOrder;
    fDb.select(query, fDbBind, fDbRows);
    foreach_rows {
        DishComplexStruct *dc = new DishComplexStruct();
        dc->fRecId = it->at(0).toString();
        dc->fName["en"] = it->at(2).toString();
        dc->fName["ru"] = it->at(3).toString();
        dc->fName["am"] = it->at(4).toString();
        dc->fAdgt = it->at(20).toString();
        dc->fId = it->at(19).toInt();
        dc->fQty = it->at(5).toDouble();
        dc->fPrice = it->at(7).toDouble();
        int row = ui->tblComplex->rowCount();
        ui->tblComplex->setRowCount(row + 1);
        ui->tblComplex->setItem(row, 0, new QTableWidgetItem());
        ui->tblComplex->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(dc));
    }


    query = "select od.f_id, od.f_dish, d.f_en, d.f_ru, d.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
            "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
            "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
            "od.f_adgt, od.f_complexRec "
            "from o_dish od "
            "left join r_dish d on d.f_id=od.f_dish "
            "where od.f_header=:f_header and (f_complex=0 or (f_complex>0 and f_complexId=0)) and f_state=1 "
            "order by od.f_id ";
    fDbBind[":f_header"] = fTable->fOrder;
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        OrderDishStruct *d = new OrderDishStruct();
        int c = 0;
        d->fRecId = it->at(c++).toString();
        d->fDishId = it->at(c++).toInt();
        d->fName["en"] = it->at(c++).toString();
        d->fName["ru"] = it->at(c++).toString();
        d->fName["am"] = it->at(c++).toString();
        d->fQty = it->at(c++).toFloat();
        d->fQtyPrint = it->at(c++).toFloat();
        d->fPrice = it->at(c++).toFloat();
        d->fSvcValue = it->at(c++).toFloat();
        d->fSvcAmount = it->at(c++).toFloat();
        d->fDctValue = it->at(c++).toFloat();
        d->fDctAmount = it->at(c++).toFloat();
        d->fTotal = it->at(c++).toFloat();
        d->fPrint1 = it->at(c++).toString();
        d->fPrint2 = it->at(c++).toString();
        d->fComment = it ->at(c++).toString();
        d->fStaff = it->at(c++).toInt();
        d->fState = it->at(c++).toInt();
        d->fComplex = it->at(c++).toInt();
        c++; //complexId
        d->fAdgt = it->at(c++).toString();
        d->fComplexRecId = it->at(c++).toString();
        if (!d->fComplexRecId.isEmpty()) {
            for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
                DishComplexStruct *ds = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
                if (ds) {
                    if (ds->fRecId == d->fComplexRecId) {
                        DishStruct *dss = new DishStruct();
                        dss->fId = d->fDishId;
                        dss->fName = d->fName;
                        dss->fPrice = d->fPrice;
                        ds->fDishes.append(dss);
                    }
                }
            }
        }
        countDish(d);
        addDishToTable(d);
        setOrderRowHidden(ui->tblOrder->rowCount() - 1, d);
    }
    changeBtnState();
    countTotal();
    DatabaseResult dr;
    fDbBind[":f_order"] = fTable->fOrder;
    dr.select(fDb, "select f_model, f_govNumber from o_car where f_order=:f_order", fDbBind);

    if (dr.rowCount() > 0) {
        CI_Car *car = CacheCar::instance()->get(dr.value("f_model").toString());
        if (car) {
            fCarModel = car->fName;
        }
        fCarGovNum = dr.value("f_govNumber").toString();
    }
    ui->lbCar->setText(fCarModel + " " + fCarGovNum);
}

void RDesk::setOrderRowHidden(int row, OrderDishStruct *od)
{
    switch(od->fState) {
    case DISH_STATE_READY:
        ui->tblOrder->setRowHidden(row, false);
        break;
    case DISH_STATE_REMOVED_NOSTORE:
    case DISH_STATE_REMOVED_STORE:
    case DISH_STATE_MOVED:
        ui->tblOrder->setRowHidden(row, !fShowRemoved);
        break;
    case DISH_STATE_EMPTY:
        ui->tblOrder->setRowHidden(row, true);
        break;
    }
}

void RDesk::printServiceCheck(const QString &prn, int side)
{
    QStringList printers = QPrinterInfo::availablePrinterNames();
    foreach (QString s, printers) {
        writelog("printer - " + s);
    }

    if (!printers.contains(prn, Qt::CaseInsensitive)) {
        message_error(tr("The printer with name not exists on the system") + "\r\n" + prn);
        return;
    }
    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 24);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    PTextRect *r = 0;
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    f.setPointSize(34);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, fHall->fName, &th)->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    r = ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("Service check, order #"))
                                     .arg(fTable->fOrder),
                                     &th, f));
    top += r->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Table")).arg(fTable->fName), &th, f))->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Time"))
                                        .arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th, f))->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Waiter"))
                                        .arg(fStaff->fName), &th, f))->textHeight();
    ps->addLine(10, top, 680, top);
    top += 2;
    ps->addTextRect(new PTextRect(10, top, 80, rowHeight, tr("Qty"), &th, f));
    top += ps->addTextRect(new PTextRect(90, top, 680, rowHeight, tr("Description"), &th, f))->textHeight();
    ps->addLine(10, top, 680, top);
    top += 2;
    th.setTextAlignment(Qt::AlignLeft);
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (side == 1) {
            if (od->fPrint1 != prn) {
                continue;
            }
        }
        if (side == 2) {
            if (od->fPrint2 != prn) {
                continue;
            }
        }
        float qty = od->fQty - od->fQtyPrint;
        if (qty < 0.1) {
            continue;
        }
        ps->addTextRect(new PTextRect(10, top, 80, rowHeight, float_str(qty, 1), &th, f));
        top += ps->addTextRect(new PTextRect(90, top, 680, rowHeight, od->fName[def_lang], &th, f))->textHeight();
        if (!od->fComment.isEmpty()) {
            f.setPointSize(18);
            f.setBold(true);
            th.setFont(f);
            top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, od->fComment, &th, f))->textHeight();
            f.setPointSize(24);
            f.setBold(false);
            th.setFont(f);
        }
        ps->addLine(10, top, 680, top);
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    top += 25;
    th.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(10, top, 680, rowHeight, tr("Printer: ") + prn, &th);
    top += 5;
    ps->addTextRect(10, top, 680, rowHeight, "_", &th);
    QPrinter printer;
    //writelog("before set " + prn.toUpper());
    printer.setPrinterName(prn.toUpper());
    //writelog("after set " + printer.printerName());
    int scale = 3; //600 / printer.resolution();
    //printer.setPrinterName("\\\\10.2.1.37\\HP LaserJet Professional P1102");
    QPainter painter(&printer);
    QPrintDialog pd(&printer, this);
    QMatrix m;
    m.scale(scale, scale);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }
}

void RDesk::printRemovedDish(OrderDishStruct *od, double removed, int user)
{
    CI_User *u = CacheUsers::instance()->get(user);
    QString userName = fStaff->fName;
    if (u) {
        userName =  u->fName;
    }
    PPrintScene ps(Portrait);
    PTextRect th;
    QFont f("Arial", 34);
    f.setBold(true);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    PTextRect *r = 0;
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    r = ps.addTextRect(10, top, 680, rowHeight, QString("%1").arg(tr("Cancelation check")), &th);
    top += r->textHeight();
    top += ps.addTextRect(10, top, 680, rowHeight, QString("Order #%1")
                                     .arg(fTable->fOrder),
                                     &th)->textHeight();
    f.setPointSize(25);
    th.setFont(f);
    top += r->textHeight();
    top += ps.addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Table")).arg(fTable->fName), &th, f))->textHeight();
    top += ps.addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Time"))
                                        .arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th, f))->textHeight();
    top += ps.addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("User"))
                                        .arg(userName), &th, f))->textHeight();
    ps.addLine(10, top, 680, top);
    th.setTextAlignment(Qt::AlignLeft);
    ps.addTextRect(new PTextRect(10, top, 80, rowHeight, float_str(removed, 1), &th, f));
    top += ps.addTextRect(new PTextRect(90, top, 680, rowHeight, od->fName[def_lang], &th, f))->textHeight();

    QPrinterInfo pi;
    if (!od->fPrint1.isEmpty()) {
        if (!pi.availablePrinterNames().contains(od->fPrint1, Qt::CaseInsensitive)) {
            message_error(QString("Printer %1 is not exists").arg(od->fPrint1));
        } else {
            QPrinter printer;
            QPrintDialog pd(&printer, this);
            printer.setPrinterName(od->fPrint1.toUpper());
            int scale = 3; //600 / printer.resolution();
            QMatrix m;
            m.scale(scale, scale);
            QPainter painter(&printer);
            painter.setMatrix(m);
            ps.render(&painter);
            //message_info("print on " + od->fPrint1);
        }
    }
    if (!od->fPrint2.isEmpty()) {
        if (!pi.availablePrinterNames().contains(od->fPrint2, Qt::CaseInsensitive)) {
            message_error(QString("Printer %1 is not exists").arg(od->fPrint2));
        } else {
            QPrinter printer;
            QPrintDialog pd(&printer, this);
            printer.setPrinterName(od->fPrint2.toUpper());
            int scale = 3;//600 / printer.resolution();
            QMatrix m;
            m.scale(scale, scale);
            QPainter painter(&printer);
            painter.setMatrix(m);
            ps.render(&painter);
            //message_info("print on " + od->fPrint2);
        }
    }
}

void RDesk::printReceipt(bool printModePayment)
{
    int trackUser = fStaff->fId;
    if (!printModePayment) {
        if (fTable->fPrint > 0) {
            if (!right(cr__o_print_unlimited_receipt, trackUser)) {
                return;
            }
        }
    }
    QString userName = fStaff->fName;
    CI_User *u = CacheUsers::instance()->get(trackUser);
    if (u) {
        userName = u->fFull;
    } else {
        userName = "#Username Error";
    }

    for (int i = 0, count = ui->tblOrder->rowCount(); i < count; i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (od->fComplex) {
            od->fPrice = 0;
            od->fTotal = 0;
            updateDish(od);
        }
    }
    ui->tblOrder->viewport()->update();

    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 30);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    PTextRect thdc;
    f.setPointSize(14);
    thdc.setFont(f);
    thdc.setBorders(false, false, false, false);
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(150, top, 400, 250));
    top += 250;
    f.setPointSize(36);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight + 10, fHall->fName, &th, f))->textHeight();
    top += 20;
    f.setPointSize(20);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("Receipt S/N "))
                                     .arg(fTable->fOrder),
                                     &th, f))->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Table"), &th, f));
    top += ps->addTextRect(new PTextRect(210, top, 200, rowHeight, fTable->fName, &th, f))->textHeight();
    if (!fCarModel.isEmpty()) {
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Car"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 400, rowHeight, fCarModel + ": " + fCarGovNum, &th, f))->textHeight();
    }
    ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Date"), &th, f));
    top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, WORKING_DATE.toString(def_date_format), &th, f))
            ->textHeight();
    if (!printModePayment) {
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Time"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 200, rowHeight, QTime::currentTime().toString(def_time_format), &th, f))->textHeight();
    }
    ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Waiter"), &th, f));
    top += ps->addTextRect(new PTextRect(210, top, 500, rowHeight, userName, &th, f))->textHeight();
    if (printModePayment) {
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Opened"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 350, rowHeight, fTable->fOpened.toString(def_date_time_format), &th, f))->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, tr("Closed"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 350, rowHeight, QDateTime::currentDateTime().toString(def_date_time_format), &th, f))->textHeight();
    }
    top += 2;
    ps->addLine(10, top, 680, top);
    top += 2;
    ps->addTextRect(new PTextRect(10, top, 100, rowHeight, tr("Qty"), &th, f));
    ps->addTextRect(new PTextRect(110, top, 390, rowHeight, tr("Description"), &th, f));
    top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, tr("Amount"), &th, f))->textHeight();
    top ++;
    ps->addLine(10, top, 680, top);
    top += 2;
    f.setPointSize(18);
    f.setBold(true);
    th.setFont(f);
    for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
        DishComplexStruct *dc = ui->tblComplex->item(i,0)->data(Qt::UserRole).value<DishComplexStruct*>();
        if (!dc) {
            message_error("Application error. Contact to developer. Message: dc==0, print receipt");
            return;
        }
        ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(dc->fQty, 1), &th, f));
        ps->addTextRect(new PTextRect(110, top, 390, rowHeight, dc->fName[def_lang], &th, f));
        top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(dc->fPrice * dc->fQty, 2), &th, f))->textHeight();
        if (top > sizePortrait.height()  - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
        foreach (DishStruct *ds, dc->fDishes) {
            //ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(dc->fQty, 1), &thdc, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, "**" + ds->fName[def_lang], &thdc, f));
            QString total = float_str(ds->fPrice * dc->fQty, 2);
            if (ds->fPrice < 0.1) {
                total = tr("Present");
            }
            top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, total, &thdc, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (od->fComplex > 0) {
            continue; //handle complex
        }
        ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(od->fQty, 1), &th, f));
        th.setWrapMode(QTextOption::WordWrap);
        int t = ps->addTextRect(new PTextRect(110, top, 390, rowHeight, od->fName[def_lang], &th, f))->textHeight();
        int k = ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(od->fTotal, 2), &th, f))->textHeight();
        top += (t > k ? t : k);
        if (top > sizePortrait.height()  - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    top += 2;
    ps->addLine(10, top, 680, top);
    top += 2;
    f.setPointSize(24);
    th.setFont(f);
    ps->addTextRect(new PTextRect(10, top, 400, rowHeight, tr("Total, AMD"), &th, f));
    top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, ui->tblTotal->item(1, 1)->data(Qt::EditRole).toString(), &th, f))->textHeight();
    // ps->addTextRect(new PTextRect(10, top, 400, rowHeight, tr("Total, USD"), &th, f));
    // top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(ui->tblTotal->item(1, 1)->data(Qt::EditRole).toDouble() / def_usd, 2), &th, f))->textHeight();

    top += rowHeight;
    f.setPointSize(28);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignHCenter);

    if (top > sizePortrait.height()  - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    if (!fTable->fRoomComment.isEmpty()) {
       top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, fTable->fRoomComment, &th, f))->textHeight();
       top += rowHeight;
       top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("Signature"), &th, f))->textHeight();
       top += rowHeight + 2;
       ps->addLine(150, top, 680, top);
    }

    if (top > sizePortrait.height()  - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    if (fTable->fPaymentMode == PAYMENT_COMPLIMENTARY) {
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("COMPLIMENTARY"), &th, f))->textHeight();
    } else {
        //top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("SALES"), &th, f))->textHeight();
    }
    if (printModePayment) {
        th.setTextAlignment(Qt::AlignLeft);
        PTextRect thr(th, "");
        thr.setTextAlignment(Qt::AlignRight);
        DatabaseResult dr;
        fDbBind[":f_id"] = fTable->fOrder;
        dr.select(fDb, "select * from o_header_payment where f_id=:f_id", fDbBind);
        if (dr.rowCount() == 0) {
            message_error(tr("Error while printing receipt"));
            return;
        }
        if (dr.value("f_cash").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Cash"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_cash").toDouble(), 0), &thr)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_card").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Card"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_card").toDouble(), 0), &thr)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_coupon").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Coupon"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_coupon").toDouble(), 0), &thr)->textHeight();
            top += ps->addTextRect(10, top, 600, rowHeight, dr.value("f_couponSeria").toString() + "/" + dr.value("f_couponNumber").toString(), &thr)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_debt").toDouble() > 0.1) {
            DatabaseResult drh;
            fDbBind[":f_debtHolder"];
            fDbBind[":f_id"] = fTable->fOrder;
            drh.select(fDb, "select f_name from o_debt_holder h "
                       "left join o_header_payment p on p.f_debtHolder=h.f_id "
                       "where p.f_id=:f_id", fDbBind);
            ps->addTextRect(10, top, 600, rowHeight, tr("Debt"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_debt").toDouble(), 0), &thr)->textHeight();
            if (drh.rowCount() > 0) {
                top += ps->addTextRect(10, top, 680, rowHeight, drh.value("f_name").toString(), &th)->textHeight();
            }
            top += 20;
            top += ps->addTextRect(10, top, 600, rowHeight, tr("Signature"), &th)->textHeight();
            ps->addLine(10, top, 680, top);
            top++;
        }
        if (dr.value("f_discount").toDouble() > 0.1) {
            ps->addTextRect(10, top, 600, rowHeight, tr("Discount"), &th);
            top += ps->addTextRect(10, top, 600, rowHeight, float_str(dr.value("f_discount").toDouble(), 0), &thr)->textHeight();
            fDbBind[":f_card"] = dr.value("f_costumer");
            dr.select(fDb, "select f_name from d_car_client where f_id=:f_card", fDbBind);
            if (dr.rowCount() > 0) {
                top += ps->addTextRect(10, top, 600, rowHeight, dr.value("f_name").toString(), &thr)->textHeight();
            } else {
                top += ps->addTextRect(10, top, 600, rowHeight, tr("Unknown costumer"), &thr)->textHeight();
            }
            ps->addLine(10, top, 680, top);
            top++;
        }
    }

    if (fShowRemoved) {
        top += (rowHeight * 3);
        top += ps->addTextRect(10, top, 600, rowHeight, tr("****VOID****"), &th)->textHeight();
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (!od) {
                continue;
            }
            if (od->fState != DISH_STATE_REMOVED_STORE) {
                continue;
            }
            if (od->fComplex > 0) {
                continue; //handle complex
            }
            ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(od->fQty, 1), &th, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, od->fName[def_lang], &th, f));
            top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(od->fTotal, 2), &th, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
        top += rowHeight;
        top += ps->addTextRect(10, top, 600, rowHeight, tr("****MISTAKE****"), &th)->textHeight();
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (!od) {
                continue;
            }
            if (od->fState != DISH_STATE_REMOVED_NOSTORE) {
                continue;
            }
            if (od->fComplex > 0) {
                continue; //handle complex
            }
            ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(od->fQty, 1), &th, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, od->fName[def_lang], &th, f));
            top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(od->fTotal, 2), &th, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
    }

    //Finish
    top += rowHeight;
    ps->addTextRect(new PTextRect(10, top, 680, rowHeight, "_", &th, f));

    QPrinter printer;
    qDebug() << defrest(dr_second_receipt_printer);
    qDebug() << defrest(dr_first_receipt_printer);
    if (printModePayment) {
        printer.setPrinterName(defrest(dr_second_receipt_printer));
    } else {
        printer.setPrinterName(defrest(dr_first_receipt_printer));
    }
    QMatrix m;
#ifdef QT_DEBUG
    m.scale(1, 1);
#else
    m.scale(3, 3);
#endif
    QPainter painter(&printer);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }

    fTable->fPrint = abs(fTable->fPrint) + 1;
    fDbBind[":f_print"] = fTable->fPrint;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    changeBtnState();
    QString v = printModePayment ? "v2" : "v1";
    fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Print receipt " + v, fTable->fPaymentComment + " " + fTable->fRoomComment, "");
}

void RDesk::changeBtnState()
{
    bool emptyReceipt = true;
    bool btnPrintService = false;
    bool btnPrintReceipt = false;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        float qty = od->fQty - od->fQtyPrint;
        emptyReceipt = false;
        if (qty > 0.1) {
            btnPrintService = true;
            break;
        }
    }
    btnPrintReceipt = !btnPrintService;
    btnPrintReceipt = btnPrintReceipt;
    ui->btnPayment_2->setEnabled(btnPrintReceipt);
    ui->btnPrint->setEnabled(btnPrintService);
    ui->btnPayment->setEnabled(btnPrintReceipt && !emptyReceipt && fTable->fPrint > 0);

    updateTableInfo();
}

void RDesk::checkEmpty()
{
    if (!fTable) {
        return;
    }
    if (fTable->fOrder.isEmpty()) {
        return;
    }

    fDbBind[":f_lockTime"] = 0;
    fDbBind[":f_lockHost"] = "";
    fDb.update("r_table", fDbBind, where_id(fTable->fId));

    bool orderEmpty = true;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState == DISH_STATE_READY) {
            orderEmpty = false;
        }
    }
    if (orderEmpty) {
        fDbBind[":f_state"] = ORDER_STATE_EMPTY;
        fDbBind[":f_dateClose"] = QDateTime::currentDateTime();
        fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
        fDbBind[":f_order"] = "";
        fDb.update("r_table", fDbBind, where_id(fTable->fId));
    }
}

void RDesk::resetPrintQty()
{
    if (fTable->fPrint > 0) {
        fTable->fPrint = (fTable->fPrint) * -1;
        fDbBind[":f_print"] = fTable->fPrint;
        fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
    }
}

void RDesk::updateDishQtyHistory(OrderDishStruct *od)
{
    fDbBind[":f_rec"] = od->fRecId;
    fDbBind[":f_user"] = fStaff->fName;
    fDbBind[":f_date"] = QDateTime::currentDateTime();
    fDbBind[":f_info"] = QString("%1/%2").arg(od->fQty).arg(od->fQtyPrint);
    fDb.insertWithoutId("o_dish_qty", fDbBind);
}

void RDesk::updateTableInfo()
{
    if (fTable) {
        for (int c = 0; c < ui->tblTables->columnCount(); c++) {
            for (int r = 0; r < ui->tblTables->rowCount(); r++) {
                TableStruct *t = ui->tblTables->item(r, c)->data(Qt::UserRole).value<TableStruct*>();
                if (!t) {
                    continue;
                }
                if (t->fId == fTable->fId) {
                    t->fAmount == ui->tblTotal->item(1, 1)->data(Qt::EditRole).toString();
                    ui->tblTables->viewport()->update();
                }
            }
        }
    }
    ui->lbCar->setText(fCarModel + " " + fCarGovNum);
}

TableStruct *RDesk::loadHall(int hall)
{
    int row = 0, col = 0;
    for (int i = 0; i < Hall::fTables.count(); i++) {
        if (row > 2) {
            break;
        }
        TableStruct *t = Hall::fTables.at(i);
        if (t->fHall != hall) {
            continue;
        }
        ui->tblTables->item(row, col)->setText(t->fName);
        ui->tblTables->item(row, col)->setData(Qt::UserRole, qVariantFromValue(t));
        col ++;
        if (col > 2) {
            col = 0;
            row ++;
        }
    }

    fMenu = Hall::fHallMap[hall]->fDefaultMenu;
    setupType(0);
    TableStruct *ts = ui->tblTables->item(0, 0)->data(Qt::UserRole).value<TableStruct*>();
    setTable(ts);
    ui->tblTables->viewport()->update();
    return ts;
}

void RDesk::on_tblPart_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    DishPartStruct *p = index.data(Qt::UserRole).value<DishPartStruct*>();
    if (!p) {
        return;
    }
    setupType(p->fId);
}

void RDesk::on_tblType_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    TypeStruct *t = index.data(Qt::UserRole).value<TypeStruct*>();
    if (!t) {
        return;
    }
    setupDish(t->fId);
}

void RDesk::on_tblDish_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    DishStruct *d = index.data(Qt::UserRole).value<DishStruct*>();
    if (!d) {
        return;
    }
    fCloseTimeout = 0;
    addDishToOrder(d);
}

void RDesk::on_btnTrash_clicked()
{
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    ui->tblOrder->clearSelection();
    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
    if (!od) {
        return;
    }
    if (!message_question(tr("Confirm to delete the selected item"))) {
        return;
    }
    QString oldQty = QString("%1, %2/%3").arg(od->fName[def_lang]).arg(od->fQty).arg(od->fQtyPrint);
    int trackUser = fStaff->fId;
    if (od->fComplex == 0) {
        if (od->fQtyPrint < 0.01) {
            od->fState = DISH_STATE_EMPTY;
            ui->tblOrder->setRowHidden(sel.at(0).row(), true);
        } else {
            float num = 0;
            float qtyToPrint = 0;
            QVariant reason;
            if (RNumbers::getNumber(num, od->fQty, this)) {
                qtyToPrint = od->fQtyPrint - (od->fQty - num);
                if (qtyToPrint > 0.01) {
                    if (!right(cr__o_cancelation, trackUser)) {
                        message_error(tr("Access denied"));
                        return;
                    }
                    QString name, sql = "select f_id, f_en from o_dish_state where f_id in (2, 3)";
                    if (!DlgList::getValue(tr("REMOVE OPTION"), name, reason, sql)) {
                        return;
                    }
                } else {
                    reason = DISH_STATE_REMOVED_NOSTORE;
                }
                if (float_equal(od->fQty, num)) {
                    od->fState = reason.toInt();
                    od->fCancelUser = trackUser;
                    od->fCancelDate = QDateTime::currentDateTime();
                    setOrderRowHidden(sel.at(0).row(), od);
                } else {
                    float qtyEmpty = od->fQty - od->fQtyPrint;
                    if (float_greaterOrEqual(qtyEmpty, num)) {
                        od->fQty -= num;
                    } else {
                        od->fQty -= qtyEmpty;
                        num -= qtyEmpty;
                        //Log empty
                        if (float_equal(od->fQty, num)) {
                            od->fState = reason.toInt();
                            od->fCancelUser = trackUser;
                            od->fCancelDate = QDateTime::currentDateTime();
                        } else {
                            od->fQty -= num;
                            od->fQtyPrint = od->fQty;
                            OrderDishStruct *r = new OrderDishStruct();
                            *r = *od;
                            r->fState = reason.toInt();
                            r->fQty = num;
                            r->fQtyPrint = num;
                            r->fRecId = uuid("DR", fDb);
                            fDb.insertId("o_dish", r->fRecId);
                            fDbBind[":f_header"] = fTable->fOrder;
                            fDbBind[":f_state"] = r->fState;
                            fDbBind[":f_dish"] = r->fDishId;
                            fDbBind[":f_qty"] = r->fQty;
                            fDbBind[":f_qtyPrint"] = r->fQtyPrint;
                            fDbBind[":f_price"] = r->fPrice;
                            fDbBind[":f_svcValue"] = r->fSvcValue;
                            fDbBind[":f_svcAmount"] = r->fSvcAmount;
                            fDbBind[":f_dctValue"] = r->fDctValue;
                            fDbBind[":f_dctAmount"] = r->fDctAmount;
                            fDbBind[":f_total"] = r->fTotal;
                            fDbBind[":f_print1"] = r->fPrint1;
                            fDbBind[":f_print2"] = r->fPrint2;
                            fDbBind[":f_store"] = r->fStore;
                            fDbBind[":f_comment"] = r->fComment;
                            fDbBind[":f_staff"] = fStaff->fId;
                            fDbBind[":f_cancelUser"] = trackUser;
                            fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
                            fDb.update("o_dish", fDbBind, where_id(ap(r->fRecId)));
                            addDishToTable(r);
                        }
                    }
                }
                if (qtyToPrint > 0.01) {
                    printRemovedDish(od, qtyToPrint, trackUser);
                }
            }
        }
        QString newQty = QString("%1, %2/%3").arg(od->fName[def_lang]).arg(od->fQty).arg(od->fQtyPrint);
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Dish qty", oldQty, newQty);
        countDish(od);
        updateDish(od);
        resetPrintQty();
    } else {
        //if od.f_complex > 0
        if (od->fQtyPrint > 0.1) {
            if (!right(cr__o_cancelation, trackUser)) {
                message_error(tr("Access denied"));
                return;
            }
        }
        if (message_yesnocancel(tr("Are you sure remove whole complex?")) != RESULT_YES) {
            return;
        }
        QVariant reason;
        QString name, sql = "select f_id, f_en from o_dish_state where f_id in (2, 3)";
        if (!DlgList::getValue(tr("REMOVE OPTION"), name, reason, sql)) {
            return;
        }
        for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
            OrderDishStruct *o = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
            if (o->fComplexRecId == od->fComplexRecId) {
                o->fState = reason.toInt();
                ui->tblOrder->setRowHidden(i, !fShowRemoved);
                updateDish(o);
                fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Remove complex component", o->fName["en"], "");
            }
        }
        for (int i = 0; i < ui->tblComplex->rowCount(); i++) {
            DishComplexStruct *dc = ui->tblComplex->item(i, 0)->data(Qt::UserRole).value<DishComplexStruct*>();
            if (dc->fRecId == od->fComplexRecId) {
                fDbBind[":f_state"] = reason.toInt();
                fDb.update("o_dish", fDbBind, where_id(ap(dc->fRecId)));
                ui->tblComplex->removeRow(i);
                fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, trackUser, "Remove complex", dc->fName["en"], "");
                delete dc;
                break;
            }
        }
        fDbBind[":f_state"] = reason;
        fDb.update("o_dish", fDbBind, where_id(od->fComplexRecId));
    }
    countTotal();
    changeBtnState();
}

void RDesk::on_btnPayment_clicked()
{

    fCloseTimeout = 0;
    /*
    int cardCode = 0;
    QString cardName;
    int cityLedger = 0;
    QString cityLedgerName;
    QString invoice;
    int prnTax = 0;
    int side = 0;
    QString room = "";
    QString guest = "-";
    QString dc = "DEBIT";
    int sign = 1;
    switch (RPaymentType::getPaymentMode(this, cardCode)) {
    case -1:
        return;
    case 0:
        fTable->fPaymentMode = PAYMENT_CASH;
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_cash);
            prnTax = 1;
            cardCode = 0;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    case 1:
        fTable->fPaymentMode = PAYMENT_CARD;
        fTable->fPaymentComment = "VISA";
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_card);
            prnTax = 1;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    case 2:
        fTable->fPaymentMode = PAYMENT_CARD;
        fTable->fPaymentComment = "MAESTRO";
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_card);
            prnTax = 1;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    case 3:
        fTable->fPaymentMode = PAYMENT_CARD;
        fTable->fPaymentComment = "MASTERCARD";
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_card);
            prnTax = 1;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    case 4:
        fDbBind[":f_paymentMode"] = PAYMENT_CARD;
        fTable->fPaymentComment = "ARCA";
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_card);
            prnTax = 1;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    case 5:
        fTable->fPaymentMode = PAYMENT_CARD;
        fTable->fPaymentComment = "AMEX";
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_card);
            prnTax = 1;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    case 6: {
        fDbBind[":f_paymentMode"] = PAYMENT_COUPONT;
        fTable->fPaymentComment = "COUPON";
        switch (message_yesnocancel(tr("Print tax?"))) {
        case RESULT_YES:
            printTax(tax_mode_card);
            prnTax = 1;
            break;
        case RESULT_CANCEL:
            return;
        }
        break;
    }
    default:
        message_error(tr("Payment mode not supported"));
        return;
    }
    fDbBind[":f_paymentMode"] = fTable->fPaymentMode;
    fDbBind[":f_paymentModeComment"] = fTable->fPaymentComment;
    fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));

    if (cardCode > 0) {
        CI_CreditCard *cc = CacheCreditCard::instance()->get(cardCode);
        if (cc) {
            cardName = cc->fName;
        }
    }

    fDb.insertId("m_register", fTable->fOrder);
    fDbBind[":f_source"] = VAUCHER_POINT_SALE_N;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = fStaff->fId;
    fDbBind[":f_room"] = room;
    fDbBind[":f_guest"] = guest;
    fDbBind[":f_itemCode"] = fHall->fItemIdForInvoice;
    fDbBind[":f_finalName"] = Hall::fHallMap[fTable->fHall]->fName + " " + fTable->fOrder;
    fDbBind[":f_amountAmd"] = ui->tblTotal->item(1, 1)->data(Qt::EditRole).toDouble();
    fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->tblTotal->item(1, 1)->data(Qt::EditRole).toDouble(), VAT_INCLUDED);
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = prnTax;
    fDbBind[":f_paymentMode"] = fTable->fPaymentMode;
    fDbBind[":f_creditCard"] = cardCode;
    fDbBind[":f_cityLedger"] = cityLedger;
    fDbBind[":f_paymentComment"] = cardName + cityLedgerName;
    fDbBind[":f_dc"] = dc;
    fDbBind[":f_sign"] = sign;
    fDbBind[":f_doc"] = fTable->fOrder;
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = invoice;
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = "";
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = side;
    fDb.update("m_register", fDbBind, where_id(ap(fTable->fOrder)));
    */
    if (fTable->fHall == 1) {
        if (fCarModel.isEmpty()) {
            message_error(tr("Car model must be selected"));
            return;
        }
    }
    if (!DlgPayment::payment(fTable->fOrder)) {
        return;
    }
    printReceipt(true);
    closeOrder();
    fTable->fAmount = "";
    changeBtnState();
}

void RDesk::on_btnPrint_clicked()
{
    QSet<QString> prn1, prn2;
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        if (od->fState != DISH_STATE_READY) {
            continue;
        }
        if (od->fQty - od->fQtyPrint > 0.1) {
            if (!od->fPrint1.isEmpty()) {
                prn1 << od->fPrint1;
            }
            if (!od->fPrint2.isEmpty()) {
                prn2 << od->fPrint2;
            }
        }
    }
    bool printed = false;
    if (prn1.count() > 0) {
        for (QSet<QString>::const_iterator prn = prn1.begin(); prn != prn1.end(); prn++) {
            printServiceCheck(*prn, 1);
        }
        printed = true;
    }
    if (prn2.count() > 0) {
        for (QSet<QString>::const_iterator prn = prn2.begin(); prn != prn2.end(); prn++) {
            printServiceCheck(*prn, 2);
        }
        printed = true;
    }
    for (int i = 0; i < ui->tblOrder->rowCount(); i++) {
        OrderDishStruct *od = ui->tblOrder->item(i, 0)->data(Qt::UserRole).value<OrderDishStruct*>();
        if (!od) {
            continue;
        }
        od->fQtyPrint = od->fQty;
        updateDish(od);
    }
    ui->tblOrder->viewport()->update();
    if (printed) {
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fOrder, fStaff->fId, "Printed service check", "", "");
    }
    changeBtnState();
}

void RDesk::on_btnTable_clicked()
{
    RSelectTable *t = new RSelectTable(this);
    t->setup(fTable->fHall);
    if (t->exec() == QDialog::Accepted) {
        if (setTable(t->table())) {
            fHall = Hall::getHallById(fTable->fHall);
            if (fMenu != fHall->fDefaultMenu) {
                fMenu = fHall->fDefaultMenu;
                setBtnMenuText();
                setupType(0);
            }
        }
    }
    delete t;
    fCloseTimeout = 0;
    changeBtnState();
}

void RDesk::on_btnComment_clicked()
{
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    OrderDishStruct *od = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
    if (!od) {
        return;
    }
    if (!od->fComment.isEmpty()) {
        if (od->fQtyPrint > 0.01) {
            message_error(tr("You cannot edit comment for this item"));
            return;
        }
    }
    if (od->fQtyPrint > 0.01) {
        message_error(tr("You cannot edit comment for this item"));
        return;
    }
    QString comment;
    if (RDishComment::getComment(comment, this)) {
        od->fComment = comment;
        updateDish(od);
        ui->tblOrder->viewport()->update();
    }
}

void RDesk::on_btnTools_clicked()
{
    RTools *t = new RTools(this);
    if (t->exec() == QDialog::Accepted) {

    }
    delete t;
}

void RDesk::on_btnCheckout_clicked()
{
    printReceipt(false);
    fCloseTimeout = 0;
}

void RDesk::on_btnTransfer_clicked()
{
    int trackUser;
    if (!right(cr__o_movement, trackUser)) {
        return;
    }
    QModelIndexList sel = ui->tblOrder->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return;
    }
    /*------------------------------BEGIN SELECT TABLE-------------------*/
    RSelectTable *t = new RSelectTable(this);
    TableStruct *table = 0;
    t->setup(fTable->fHall);
    if (t->exec() == QDialog::Accepted) {
        table = t->table();
        if (table == fTable) {
            DlgSmile *ds = new DlgSmile(this);
            ds->exec();
            delete ds;
            return;
        }
        fDb.fDb.transaction();
        fDb.queryDirect(QString("select f_id from r_table where f_id in (%1, %2) for update")
                .arg(fTable->fId)
                .arg(table->fId));
        if (!table->fOrder.isEmpty()) {
            if (!message_question(tr("Destination table is not empty, continue with merge?"))) {
                delete t;
                fDb.fDb.commit();
                return;
            }
        } else {
            checkOrderHeader(table);
        }
        /*----------------------------- END SELECT TABLE -------------------------*/
        /*-----------------------------BEGIN MOVE DISH-----------------------------*/
        OrderDishStruct *om = sel.at(0).data(Qt::UserRole).value<OrderDishStruct*>();
        OrderDishStruct od = *om;
        om->fState = DISH_STATE_MOVED;
        setOrderRowHidden(sel.at(0).row(), om);
        updateDish(om);
        fDbBind[":f_header"] = table->fOrder;
        fDbBind[":f_state"] = od.fState;
        fDbBind[":f_dish"] = od.fDishId;
        fDbBind[":f_qty"] = od.fQty;
        fDbBind[":f_qtyPrint"] = od.fQtyPrint;
        fDbBind[":f_price"] = od.fPrice;
        fDbBind[":f_svcValue"] = od.fSvcValue;
        fDbBind[":f_svcAmount"] = od.fSvcAmount;
        fDbBind[":f_dctValue"] = od.fDctValue;
        fDbBind[":f_dctAmount"] = od.fDctAmount;
        fDbBind[":f_total"] = od.fTotal;
        fDbBind[":f_print1"] = od.fPrint1;
        fDbBind[":f_print2"] = od.fPrint2;
        fDbBind[":f_store"] = od.fStore;
        fDbBind[":f_comment"] = od.fComment;
        fDbBind[":f_staff"] = fStaff->fId;
        fDbBind[":f_complex"] = od.fComplex;
        fDbBind[":f_complexId"] = 0;
        fDbBind[":f_adgt"] = od.fAdgt;
        fDbBind[":f_complexRec"] = od.fComplexRecId;
        od.fRecId = uuid("DR", fDb);
        fDbBind[":f_id"] = od.fRecId;
        fDb.insertWithoutId("o_dish", fDbBind);
        if (od.fRecId.isEmpty()) {
            message_error(tr("Cannot insert new dish. Please reopen application"));
            return;
        }
        fTrackControl->addMessage(TRACK_REST_ORDER, fTable->fId, fStaff->fId, "Dish movement",
                                  QString("%1: %2, %3/%4")
                                  .arg(fTable->fName)
                                  .arg(od.fName[def_lang])
                                  .arg(od.fQty)
                                  .arg(od.fQtyPrint),
                                  QString("%1: %2/%3")
                                  .arg(tr("New table"))
                                  .arg(table->fName)
                                  .arg(table->fOrder));
    }    
    fDb.fDb.commit();
    resetPrintQty();
    changeBtnState();
    /*-----------------------------END MOVE DISH-----------------------------*/
}

int RDesk::right(int right, int &trackUser)
{
    bool access = RIGHT(fStaff->fGroup, right);
    trackUser = fStaff->fId;
    if (!access) {
        trackUser = 0;
        QString login;
        if (RLogin::getLogin(login, tr("Raise privileges"), this)) {
            User u(login);
            if (u.isValid()) {
                access = RIGHT(u.fGroup, right);
                if (access) {
                    trackUser = u.fId;
                }
            }
        }
    }
    if (!access) {
        message_error(tr("Access denied"));
    }
    return access;
}

void RDesk::on_btnTypeUp_clicked()
{
    ui->tblType->verticalScrollBar()->setValue(ui->tblType->verticalScrollBar()->value() - 6);
}

void RDesk::on_btnTypeDown_clicked()
{
    ui->tblType->verticalScrollBar()->setValue(ui->tblType->verticalScrollBar()->value() + 6);
}

void RDesk::on_btnDishUp_clicked()
{
    ui->tblDish->verticalScrollBar()->setValue(ui->tblDish->verticalScrollBar()->value() - 6);
}

void RDesk::on_btnDishDown_clicked()
{
    ui->tblDish->verticalScrollBar()->setValue(ui->tblDish->verticalScrollBar()->value() + 6);
}

void RDesk::on_btnOrdDown_clicked()
{
    ui->tblOrder->verticalScrollBar()->setValue(ui->tblOrder->verticalScrollBar()->value() + 6);
}

void RDesk::on_btnOrdUp_clicked()
{
    ui->tblOrder->verticalScrollBar()->setValue(ui->tblOrder->verticalScrollBar()->value() - 6);
}

void RDesk::on_btnSetRoom_clicked()
{
    DlgReservation *d = new DlgReservation(this);
    d->loadRoom();
    if (d->exec() == QDialog::Accepted) {
        fDbBind[":f_id"] = d->fReservationId;
        fDb.select("select r.f_room, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName) "
                   "from f_reservation r "
                   "inner join f_guests g on g.f_id=r.f_guest "
                   "where r.f_id=:f_id", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            fDbBind[":f_roomComment"] = fDbRows.at(0).at(0).toString() + ", " + fDbRows.at(0).at(1).toString();
            fTable->fRoomComment = fDbBind[":f_roomComment"].toString();
            fDb.update("o_header", fDbBind, where_id(ap(fTable->fOrder)));
        }
    }
}

void RDesk::on_btnComplex_clicked()
{
    setComplexMode();
    fCloseTimeout = 0;
}

void RDesk::on_tblTables_itemClicked(QTableWidgetItem *item)
{
    if (!item) {
        return;
    }
    TableStruct *t = item->data(Qt::UserRole).value<TableStruct*>();
    if (!t) {
        return;
    }
    setTable(t);
}

void RDesk::on_btnPayment_2_clicked()
{
     printReceipt(false);
     changeBtnState();
}

void RDesk::on_btnSetCar_clicked()
{
    if (fTable->fOrder.isEmpty()) {
        message_error(tr("Emtpy order"));
        return;
    }
    int model;
    QString govNum;
    DlgCarSelection::selectCar(model, govNum);
    govNum = govNum.toUpper();
    fDbBind[":f_model"] = model;
    fDbBind[":f_order"] = fTable->fOrder;
    fDbBind[":f_govNumber"] = govNum;
    fDb.select("update o_car set f_model=:f_model, f_govNumber=:f_govNumber where f_order=:f_order", fDbBind, fDbRows);
    fCarGovNum = govNum;
    fDbBind[":f_id"] = model;
    DatabaseResult dr;
    dr.select(fDb, "select concat(f_model, ' ', f_class) as car from d_car_model where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        fCarModel = dr.value("car").toString();
        ui->lbCar->setText(fCarModel + " " + fCarGovNum);
    }
}

void RDesk::on_btnDiscount_clicked()
{
    message_error(tr("No discount mechanism"));
}

void RDesk::on_btnPackage_clicked()
{
    setComplexMode();
}

void RDesk::on_btnHallWash_clicked()
{
    loadHall(1);
}

void RDesk::on_btnHallCafe_clicked()
{
    loadHall(2);
}
