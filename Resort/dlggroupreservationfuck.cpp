#include "dlggroupreservationfuck.h"
#include "ui_dlggroupreservationfuck.h"
#include "cachereservation.h"
#include "cacheroom.h"
#include "cacheroomarrangment.h"
#include "cacheguest.h"
#include "wguest.h"
#include "dlgreservationremarks.h"
#include "waccinvoice.h"
#include "paymentmode.h"
#include "cachegroupreservations.h"
#include "winvoice.h"
#include "cacheredreservation.h"
#include "wreservation.h"
#include "vauchers.h"
#include "pprintscene.h"
#include "pprintpreview.h"
#include "wreservationroomtab.h"
#include "ptextrect.h"
#include "pimage.h"

DlgGroupReservationFuck::DlgGroupReservationFuck(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::DlgGroupReservationFuck)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblCat, ui->tblCat->columnCount(),
                                30, 30, 30, 40, 40, 70, 30);
    ui->tblCat->setMaximumWidth(350);
    ui->tblCat->setMinimumWidth(350);
    Utils::tableSetHeaderCaptions(ui->tblRoom, ui->tblRoom->columnCount(),
                                  QString("RS"), QString("Room"), QString("Cat"), QString("Bed"),
                                  QString("Arrival"), QString("Departure"), QString("Price"),
                                  QString("Nights"), QString("Total"),
                                  QString("Arr."), QString("Guest"), QString("Remarks"),
                                  QString("Male"), QString("Female"), QString("Child"), QString("GuestCode"),
                                  QString("X"), QString("O"), QString("R"), QString("I"), QString("State"), QString("State id"));
    Utils::tableSetColumnWidths(ui->tblRoom, ui->tblRoom->columnCount(),
                                100, 40, 40, 40, 100, 100, 50, 80, 80, 55, 200, 0, 50, 50, 50, 0, 30, 30, 30, 30, 80, 0);
    DatabaseResult dr;
    dr.select(fDb, "select rc.f_short, rm.f_bed, rm.f_rate, count(rm.f_id) as f_room "
              "from f_room rm "
              "inner join f_room_classes rc on rc.f_id=rm.f_class "
              "group by 1, 2", fDbBind);
    ui->tblCat->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->tblCat->setItemWithValue(i, 0, dr.value(i, "f_short"));
        ui->tblCat->setItemWithValue(i, 1, dr.value(i, "f_bed"));
        ui->tblCat->setItemWithValue(i, 2, dr.value(i, "f_room"));
        ui->tblCat->setItemWithValue(i, 3, 0);
        EQLineEdit *l = ui->tblCat->addLineEdit(i, 4, false);
        l->setReadOnly(true);
        connect(l, SIGNAL(textChanged(QString)), this, SLOT(preCount(QString)));
        l = ui->tblCat->addLineEdit(i, 5, false);
        l->setReadOnly(true);
        connect(l, SIGNAL(textChanged(QString)), this, SLOT(preCount(QString)));
        ui->tblCat->addButton(i, 6, SLOT(createMultiRoom(int)), this, QIcon(":/images/goto.png"));
    }
    ui->tblCat->setMaximumHeight(((ui->tblCat->rowCount() + 1) * ui->tblCat->verticalHeader()->defaultSectionSize()) + 15);
    ui->tblCat->setMinimumHeight(((ui->tblCat->rowCount() + 1) * ui->tblCat->verticalHeader()->defaultSectionSize()) + 15);
    QMapIterator<QString, CI_Room*> itroom(CacheRoom::instance()->it());
    while (itroom.hasNext()) {
        itroom.next();
        CI_Room *r = itroom.value();
        for (int i = 0; i < ui->tblCat->rowCount(); i++) {
            if (ui->tblCat->toString(i, 0) == r->fCategoryShort && ui->tblCat->toString(i, 1) == r->fBed) {
                ui->tblCat->setItemWithValue(i, 3, ui->tblCat->toInt(i, 3) + 1);
                break;
            }
        }
    }
    fDockCardex  = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardexCode);
    connect(fDockCardex, &DWSelectorCardex::cardex, [this](CI_Cardex *c){
       dockResponse<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, c);
    });
    ui->leCardexCode->setText(fPreferences.getDb(def_default_cardex).toString());
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, CacheCardex::instance()->get(ui->leCardexCode->text()));
    fCurrRow = -1;
    fDockGuest = new DWSelectorGuest(this);
    fDockGuest->configure();
    fDockGuest->setSelector(ui->leGuest);
    connect(fDockGuest, &DWSelectorGuest::guest, [this](CI_Guest *g) {
        if (!g) {
            if (!ui->leGuest->fHiddenText.isEmpty()) {
                g = CacheGuest::instance()->get(ui->leGuest->fHiddenText);
            }
        }
        if (g) {
            ui->leGuest->setText(g->fName);
            ui->leGuest->fHiddenText = g->fCode;
        } else {
            ui->leGuest->clear();
            ui->leGuest->fHiddenText.clear();
        }
    });

    fDockSingleGuest = new DWSelectorGuest(this);
    fDockSingleGuest->configure();
    fDockSingleGuest->setSelector(0);
    connect(fDockSingleGuest, SIGNAL(guest(CI_Guest*)), this, SLOT(singleGuest(CI_Guest*)));

    connect(ui->teCommonRemark, &EQTextEdit::doubleClicked, [this](){
        QString remark = ui->teCommonRemark->toPlainText();
        if (DlgReservationRemarks::Remarks(remark)) {
            ui->teCommonRemark->setPlainText(remark);
        }
    });

    connect(ui->teIndRemarks, &EQTextEdit::doubleClicked, [this](){
        QString remark = ui->teIndRemarks->toPlainText();
        if (DlgReservationRemarks::Remarks(remark)) {
            ui->teIndRemarks->setPlainText(remark);
        }
    });

    fDockRoom = new DlgSimpleSelectRoom(this);
    fDockRoom->show();
    fDockRoom->hide();

    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCLCode);
    connect(fDockCL, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));

    fDockCard = new DWSelectorCreditCard(this);
    fDockCard->configure();
    fDockCard->setSelector(ui->leCardCode);
    connect(fDockCard, SIGNAL(creditCard(CI_CreditCard*)), this, SLOT(card(CI_CreditCard*)));
    countReserve();
    on_cbModeOfPayment_currentIndexChanged(0);

    ui->cbArr->setIndexForData(fPreferences.getDb(def_room_arrangement));

    fTrackControl = new TrackControl(TRACK_RESERVATION);
}

DlgGroupReservationFuck::~DlgGroupReservationFuck()
{
    delete ui;
}

void DlgGroupReservationFuck::loadGroup(int id)
{
    ui->tblRoom->clearContents();
    ui->tblRoom->setRowCount(0);
    DatabaseResult dr;
    fDbBind[":f_id"] = id;
    dr.select(fDb, "select * from f_reservation_group where f_id=:f_id", fDbBind);
    ui->leGroupCode->setInt(id);
    ui->leGroupName->setText(dr.value("f_name").toString());
    ui->deArrival->setDate(dr.value("f_entry").toDate());
    ui->deDeparture->setDate(dr.value("f_departure").toDate());
    ui->leCardexCode->setText(dr.value("f_cardex").toString());
    dockResponse<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, 0);
    ui->cbArr->setIndexForData(dr.value("f_arrangement").toInt());
    ui->lePrice->setDouble(dr.value("f_price").toDouble());
    ui->cbModeOfPayment->setIndexForData(dr.value("f_paymentMode").toInt());
    ui->leGuest->fHiddenText = dr.value("f_guest").toString();
    ui->leExtraname->setText(dr.value("f_extra1").toString());
    CI_Guest *g = CacheGuest::instance()->get(ui->leGuest->fHiddenText);
    if (g) {
        ui->leGuest->setText(g->fName);
        ui->leGuest->fHiddenText = g->fCode;
    }
    ui->leCLCode->setText(dr.value("f_cityLedger").toString());
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, 0);
    ui->leCardCode->setText(dr.value("f_card").toString());
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, 0);
    ui->teCommonRemark->setPlainText(dr.value("f_remarks").toString());
    on_cbModeOfPayment_currentIndexChanged(0);
    fDbBind[":f_group"] = id;
    dr.select(fDb, "select r.f_id, r.f_room, rc.f_short, rm.f_bed, r.f_startDate, r.f_endDate, "
              "r.f_pricePerNight, r.f_arrangement, g.guest, r.f_remarks, r.f_guest, r.f_paymentType, "
              "r.f_cardex, r.f_man, r.f_woman, r.f_child, r.f_state, rs.f_en as state_name "
              "from f_reservation r "
              "left join guests g on g.f_id=r.f_guest "
              "left join f_room rm on rm.f_id=r.f_room "
              "left join f_reservation_state rs on rs.f_id=r.f_state "
              "left join f_room_classes rc on rc.f_id=rm.f_class "
              "where r.f_group=:f_group and r.f_state in (1,2,3,6) ", fDbBind);
    bool editable = false;
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = ui->tblRoom->rowCount();
        ui->tblRoom->setRowCount(row + 1);
        ui->tblRoom->setItemWithValue(row, 0, dr.value(i, "f_id"));
        EQLineEdit *l = ui->tblRoom->addLineEdit(row, 1, false);
        l->fClickBool = true;
        l->setText(dr.value(i, "f_room").toString());
        connect(l, SIGNAL(textEdited(QString)), this, SLOT(roomTextChanged(QString)));
        connect(l, SIGNAL(customButtonClicked(bool)), this, SLOT(openRoomSelectDialog(bool)));
        ui->tblRoom->setItemWithValue(row, 2, dr.value(i, "f_short"));
        ui->tblRoom->setItemWithValue(row, 3, dr.value(i, "f_bed"));
        EDateEdit *d = ui->tblRoom->addDateEdit(row, 4, false);
        d->setDate(dr.value(i, "f_startDate").toDate());
        connect(d, SIGNAL(textEdited(QString)), this, SLOT(roomTextChanged(QString)));
        d = ui->tblRoom->addDateEdit(row, 5, false);
        d->setDate(dr.value(i, "f_endDate").toDate());
        connect(d, SIGNAL(textEdited(QString)), this, SLOT(roomTextChanged(QString)));
        l = ui->tblRoom->addLineEdit(row, 6, false);
        l->setDouble(dr.value(i, "f_pricePerNight").toDouble());
        connect(l, SIGNAL(textChanged(QString)), this, SLOT(roomPriceChanged(QString)));
        EQComboBox *c = ui->tblRoom->addComboBox(row, 9);
        c->setCache(cid_room_arrangment);
        c->setCurrentIndex(c->findData(dr.value(i, "f_arrangement").toInt()));
        EQLineEdit *lg = ui->tblRoom->addLineEdit(row, 10, false);
        lg->setText(dr.value(i, "guest").toString());
        lg->fHiddenText = dr.value(i, "f_guest").toString();
        connect(lg, SIGNAL(customButtonClicked(bool)), this, SLOT(singleGuestLineEdit(bool)));
        connect(lg, SIGNAL(focusIn()), this, SLOT(singleGuestFocusOut()));
        ui->tblRoom->setItemWithValue(row, 11, dr.value(i, "f_remarks"));        
        lg = ui->tblRoom->addLineEdit(row, 12, false);
        lg->setInt(dr.value(i, "f_man").toInt());
        lg = ui->tblRoom->addLineEdit(row, 13, false);
        lg->setInt(dr.value(i, "f_woman").toInt());
        lg = ui->tblRoom->addLineEdit(row, 14, false);
        lg->setInt(dr.value(i, "f_child").toInt());
        ui->tblRoom->setItemWithValue(row, 15, dr.value(i, "f_guest"));

        ui->tblRoom->addButton(row, 16, SLOT(removeRow()), this, QIcon(":/images/garbage.png"));
        ui->tblRoom->setItemWithValue(row, 17, "O");

        if (dr.value(i, "f_state").toInt() == 2) {
            editable = true;
        }
        if (dr.value(i, "f_state").toInt() != 2) {
            ui->tblRoom->lineEdit(row, 1)->setEnabled(false);
            ui->tblRoom->dateEdit(row, 4)->setEnabled(false);
            ui->tblRoom->dateEdit(row, 5)->setEnabled(false);
            ui->tblRoom->lineEdit(row, 6)->setEnabled(false);
            ui->tblRoom->comboBox(row, 9)->setEnabled(false);
            ui->tblRoom->lineEdit(row, 10)->setEnabled(false);
            ui->tblRoom->lineEdit(row, 12)->setEnabled(false);
            ui->tblRoom->lineEdit(row, 13)->setEnabled(false);
            ui->tblRoom->lineEdit(row, 14)->setEnabled(false);
        }
        if (dr.value(i, "f_state").toInt() == 2) {
            ui->tblRoom->addButton(row, 18, SLOT(editReserve()), this, QIcon(":/images/bed.png"));
        }
        if (dr.value(i, "f_state").toInt() == 3) {
            ui->tblRoom->button(row, 16)->setEnabled(false);
            ui->tblRoom->addButton(row, 18, SLOT(editReserve()), this, QIcon(":/images/bed.png"));
            ui->tblRoom->addButton(row, 19, SLOT(editAInvoice()), this, QIcon(":/images/invoice.png"));
        }
        if (dr.value(i, "f_state").toInt() == 1) {
            ui->tblRoom->button(row, 16)->setEnabled(false);
            ui->tblRoom->addButton(row, 18, SLOT(editReserve()), this, QIcon(":/images/bed.png"));
            ui->tblRoom->addButton(row, 19, SLOT(editInvoice()), this, QIcon(":/images/invoice.png"));
        }
        if (dr.value(i, "f_state").toInt() == RESERVE_REMOVED) {
            ui->tblRoom->button(row, 16)->setEnabled(false);
        }

        ui->tblRoom->setItemWithValue(row, 20, dr.value(i, "state_name").toString());
        ui->tblRoom->setItemWithValue(row, 21, dr.value(i, "f_state").toInt());
        makeTrackControl(row);
    }
    countTotalReservation();
    ui->wll->setEnabled(editable || r__(cr__super_correction));
    ui->wl->setEnabled(editable || r__(cr__super_correction));
    ui->wr->setEnabled(editable || r__(cr__super_correction));
    ui->btnSave->setEnabled(editable || r__(cr__super_correction));
    ui->btnClear->setEnabled(editable || r__(cr__super_correction));
    ui->btnClearFilter->setEnabled(editable || r__(cr__super_correction));
}

void DlgGroupReservationFuck::setup()
{
    setupTabTextAndIcon(tr("Group reservation"), ":/images/groupreservation.png");
}

void DlgGroupReservationFuck::preCount(const QString &str)
{
    Q_UNUSED(str)
    double total = 0;
    int qty = 0;
    for (int i = 0; i < ui->tblCat->rowCount(); i++) {
        total += (ui->tblCat->lineEdit(i, 5)->asDouble());
        qty += ui->tblCat->lineEdit(i, 4)->asInt();
    }
    /*for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != 2) {
            //total -= ui->tblRoom->toDouble(i, 8);
            qty --;
        }
    }*/
    ui->lePreQty->setInt(qty);
    ui->lePreAmount->setDouble(total);
}

void DlgGroupReservationFuck::cityLedger(CI_CityLedger *ci)
{
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, ci);
}

void DlgGroupReservationFuck::card(CI_CreditCard *ci)
{
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, ci);
}

void DlgGroupReservationFuck::removeRow()
{
    EPushButton *btn = static_cast<EPushButton*>(sender());
    int row = -1;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        EPushButton *btnWidget = static_cast<EPushButton*>(ui->tblRoom->cellWidget(i, 16));
        if (btnWidget == btn) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    if (message_confirm_tr("Confirm to remove selected row") != QDialog::Accepted) {
        return;
    }
    if (ui->tblRoom->toString(row, 0).isEmpty()) {
        ui->tblRoom->removeRow(row);
        countTotalReservation();
        return;
    }
    fDbBind[":f_state"] = RESERVE_REMOVED;
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(row, 0))));
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->tblRoom->toString(row, 0));
    ui->tblRoom->removeRow(row);
    message_info_tr("Reservation was canceled.");
    countTotalReservation();
}

void DlgGroupReservationFuck::editReserve()
{
    EPushButton *btn = static_cast<EPushButton*>(sender());
    int row = -1;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        EPushButton *btnWidget = static_cast<EPushButton*>(ui->tblRoom->cellWidget(i, 18));
        if (btnWidget == btn) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    WReservation::openReserveWindows(ui->tblRoom->toString(row, 0));
}

void DlgGroupReservationFuck::editInvoice()
{
    EPushButton *btn = static_cast<EPushButton*>(sender());
    int row = -1;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        EPushButton *btnWidget = static_cast<EPushButton*>(ui->tblRoom->cellWidget(i, 19));
        if (btnWidget == btn) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    CI_Reservation *r = CacheReservation::instance()->get(ui->tblRoom->toString(row, 0));
    if (r) {
        WInvoice::openInvoiceWindow(r->fInvoice);
    }
}

void DlgGroupReservationFuck::editAInvoice()
{
    EPushButton *btn = static_cast<EPushButton*>(sender());
    int row = -1;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        EPushButton *btnWidget = static_cast<EPushButton*>(ui->tblRoom->cellWidget(i, 19));
        if (btnWidget == btn) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    fDbBind[":f_id"] = ui->tblRoom->toString(row, 0);
    DatabaseResult dr;
    dr.select(fDb, "select f_invoice from f_reservation where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        WAccInvoice *wa = addTab<WAccInvoice>();
        wa->load(dr.value("f_invoice").toString());
    }
}

void DlgGroupReservationFuck::countReserve()
{
    QSet<int> rooms;
    CacheReservation::instance()->exludeList(ui->deArrival->date(), ui->deDeparture->date(), rooms);
    QMap<QString, QMap<QString, int> > total;
    for (int i = 0; i < ui->tblCat->rowCount(); i++) {
        total[ui->tblCat->toString(i, 0)][ui->tblCat->toString(i, 1)] = ui->tblCat->toInt(i, 2);
    }
    foreach (int i, rooms) {
        CI_Room *r = CacheRoom::instance()->get(i);
        if (!r) {
            continue;
        }
        total[r->fCategoryShort][r->fBed] = total[r->fCategoryShort][r->fBed] - 1;
    }
    for (QMap<QString, QMap<QString, int> >::const_iterator l1 = total.begin(); l1 != total.end(); l1++) {
        for (QMap<QString, int>::const_iterator lb = (*l1).begin(); lb != (*l1).end(); lb++) {
            for (int i = 0; i < ui->tblCat->rowCount(); i++) {
                if (ui->tblCat->toString(i, 0) == l1.key() && ui->tblCat->toString(i, 1) == lb.key()) {
                    ui->tblCat->setItemWithValue(i, 3, lb.value());
                }
            }
        }
    }
}

void DlgGroupReservationFuck::countTotalReservation()
{
    for (int i = 0; i < ui->tblCat->rowCount(); i++) {
        ui->tblCat->lineEdit(i, 4)->clear();
        ui->tblCat->lineEdit(i, 5)->clear();
    }
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        for (int j = 0; j < ui->tblCat->rowCount(); j++) {
            if (ui->tblCat->toString(j, 0) == ui->tblRoom->toString(i, 2) && ui->tblCat->toString(j, 1) == ui->tblRoom->toString(i, 3)) {
                if (ui->tblRoom->toInt(i, 21) != 2) {
                    continue;
                }
                ui->tblCat->lineEdit(j, 4)->setInt(ui->tblCat->lineEdit(j, 4)->asInt() + 1);
                ui->tblCat->lineEdit(j, 5)->setDouble(ui->tblCat->lineEdit(j, 5)->asDouble() +
                                                      (ui->tblRoom->lineEdit(i, 6)->asDouble() * (ui->tblRoom->dateEdit(i, 4)->date().daysTo(ui->tblRoom->dateEdit(i, 5)->date()))));
                break;
            }
        }
        ui->tblRoom->setItemWithValue(i, 7,
            ui->tblRoom->dateEdit(i, 4)->date().daysTo(ui->tblRoom->dateEdit(i, 5)->date()));
        ui->tblRoom->setItemWithValue(i, 8,
            ui->tblRoom->toInt(i, 7) * ui->tblRoom->lineEdit(i, 6)->asDouble());
    }
}

void DlgGroupReservationFuck::makeTrackControl(int row)
{
    TrackControl *t = new TrackControl(TRACK_RESERVATION);
    t->fReservation = ui->tblRoom->toString(row, 0);
    t->addWidget(ui->tblRoom->dateEdit(row, 4), "(In group) Entry date");
    t->addWidget(ui->tblRoom->dateEdit(row, 5), "(In group) Departure date");
    t->addWidget(ui->tblRoom->lineEdit(row, 6), "(In group) Price");
    t->addWidget(ui->tblRoom->comboBox(row, 9), "(In group) Arrangement");
    t->addWidget(ui->tblRoom->lineEdit(row, 10), "(In group) Guest");
    //t->addWidget(ui->tblRoom->lineEdit(row, 12), "(In group) Male");
    //t->addWidget(ui->tblRoom->lineEdit(row, 13), "(In group) Female");
    //t->addWidget(ui->tblRoom->lineEdit(row, 14), "(In group) Child");
    ui->tblRoom->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(t));
}

void DlgGroupReservationFuck::createRooms(const QString &cat, const QString &bed, int count, double price)
{
    if (count < 1) {
        return;
    }
    QSet<int> rooms;
    CacheReservation::instance()->exludeList(ui->deArrival->date(), ui->deDeparture->date(), rooms);
    QMapIterator<QString, CI_Room*> itroom(CacheRoom::instance()->it());
    while (itroom.hasNext() && count > 0) {
        itroom.next();
        CI_Room *r = itroom.value();
        if (rooms.contains(r->fCode.toInt())) {
            continue;
        }
        if (cat == r->fCategoryShort && bed == r->fBed) {
            count--;
            int row = ui->tblRoom->rowCount();
            ui->tblRoom->setRowCount(row + 1);
            ui->tblRoom->setItemWithValue(row, 0, "");
            EQLineEdit *l = ui->tblRoom->addLineEdit(row, 1, false);
            l->fClickBool = true;
            l->setText("");
            connect(l, SIGNAL(textEdited(QString)), this, SLOT(roomTextChanged(QString)));
            connect(l, SIGNAL(customButtonClicked(bool)), this, SLOT(openRoomSelectDialog(bool)));
            ui->tblRoom->setItemWithValue(row, 2, r->fCategoryShort);
            ui->tblRoom->setItemWithValue(row, 3, r->fBed);
            EDateEdit *d = ui->tblRoom->addDateEdit(row, 4, false);
            d->setDate(ui->deArrival->date());
            connect(d, SIGNAL(textEdited(QString)), this, SLOT(roomTextChanged(QString)));
            d = ui->tblRoom->addDateEdit(row, 5, false);
            d->setDate(ui->deDeparture->date());
            connect(d, SIGNAL(textEdited(QString)), this, SLOT(roomTextChanged(QString)));
            l = ui->tblRoom->addLineEdit(row, 6, false);
            l->setDouble(price);
            connect(l, SIGNAL(textChanged(QString)), this, SLOT(roomPriceChanged(QString)));
            EQComboBox *c = ui->tblRoom->addComboBox(row, 9);
            c->setCache(cid_room_arrangment);
            c->setCurrentIndex(c->findText(ui->cbArr->currentText()));
            EQLineEdit *lg = ui->tblRoom->addLineEdit(row, 10, false);
            lg->setText(ui->leGuest->text());
            connect(lg, SIGNAL(customButtonClicked(bool)), this, SLOT(singleGuestLineEdit(bool)));
            connect(lg, SIGNAL(focusIn()), this, SLOT(singleGuestFocusOut()));
            ui->tblRoom->setItemWithValue(row, 11, ui->teCommonRemark->toPlainText());
            ui->tblRoom->addLineEdit(row, 12, false);
            ui->tblRoom->addLineEdit(row, 13, false);
            ui->tblRoom->addLineEdit(row, 14, false);
            ui->tblRoom->setItemWithValue(row, 15, ui->leGuest->fHiddenText);
            ui->tblRoom->addButton(row, 16, SLOT(removeRow()), this, QIcon(":/images/garbage.png"));
            ui->tblRoom->setItemWithValue(row, 17, "O");
            ui->tblRoom->addButton(row, 18, SLOT(editReserve()), this, QIcon(":/images/bed.png"));
            ui->tblRoom->setItemWithValue(row, 20, tr("Reserved"));
            ui->tblRoom->setItemWithValue(row, 21, RESERVE_RESERVE);
        }
    }
    countTotalReservation();
}

void DlgGroupReservationFuck::on_deArrival_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countReserve();
}

void DlgGroupReservationFuck::on_deDeparture_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countReserve();
}

void DlgGroupReservationFuck::on_tblRoom_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QTableWidgetItem *item = ui->tblRoom->item(index.row(), 11);
    if (!item) {
        return;
    }
    fCurrRow = -1;
    ui->teIndRemarks->setPlainText(item->text());
    fCurrRow = index.row();
}

void DlgGroupReservationFuck::on_btnAddGuest_clicked()
{
    QList<QVariant> values;
    WGuest *g = WGuest::guest(values);
    if (g->exec() == QDialog::Accepted) {
        CI_Guest *guest = CacheGuest::instance()->get(values.at(0).toString());
        int wait = 10000;
        while (guest == 0 && wait > 0) {
            qApp->processEvents();
            guest = CacheGuest::instance()->get(values.at(0).toString());
            wait--;
        }
        if (!guest) {
            message_error_tr("Cannot register new guest, contact to application developer");
            return;
        }
        ui->leGuest->setText(guest->fName);
        ui->leGuest->fHiddenText = guest->fCode;
    }
    delete g;
}

void DlgGroupReservationFuck::on_leGuest_returnPressed()
{
    if (ui->leGuest->text().trimmed().isEmpty()) {
        return;
    }
    QStringList fn = ui->leGuest->text().split(" ");
    if (fn.count() > 0) {
        QString searchName = fn.at(0).toLower();
        if (fn.count() > 1) {
            searchName += " " + fn.at(1).toLower();
        }
        fDb.select("select f_id from f_guests where concat(lower(f_firstName), ' ', lower(f_lastName)) like '" + searchName + "%'", fDbBind, fDbRows);
        if (fDbRows.count()  > 0) {
            fDockGuest->setFilterValue(ui->leGuest->text());
            fDockGuest->show();
            fDockGuest->activateWindow();
            fDockGuest->raise();
        } else {
            fDbBind[":f_title"] = "";
            fDbBind[":f_firstName"] = fn.at(0);
            if (fn.count() > 1) {
                fDbBind[":f_lastName"] = fn.at(1);
            } else {
                fDbBind[":f_lastName"] = "";
            }
            int gid = fDb.insert("f_guests", fDbBind);
            CI_Guest g;
            g.fCode = QString::number(gid);
            g.fName = fn.at(0) + " ";
            if (fn.count() > 1) {
                g.fName += fn.at(1);
            }
            BroadcastThread::cmdRefreshCache(cid_guest, g.fCode);
            CI_Guest *gg = 0;
            int wait = 10000;
            while (gg == 0 && wait > 0) {
                qApp->processEvents();
                gg = CacheGuest::instance()->get(g.fCode);
                wait--;
            }
            if (!gg) {
                return;
            }
            ui->leGuest->setText(g.fName);
            ui->leGuest->fHiddenText = g.fCode;
        }
    }
}

void DlgGroupReservationFuck::on_teIndRemarks_textChanged()
{
    if (fCurrRow < 0) {
        return;
    }
    ui->tblRoom->setItemWithValue(fCurrRow, 11, ui->teIndRemarks->toPlainText());
}

void DlgGroupReservationFuck::on_btnCreateGroup_clicked()
{
    for (int i = 0; i < ui->tblCat->rowCount(); i++) {
        if (ui->tblCat->lineEdit(i, 4)->asInt() < 1) {
            ui->tblCat->lineEdit(i, 4)->clear();
            continue;
        }
        if (ui->tblCat->toInt(i, 3) - ui->tblCat->toInt(i, 4) > -1) {
            createRooms(ui->tblCat->toString(i, 0), ui->tblCat->toString(i, 1),
                        ui->tblCat->lineEdit(i, 4)->asInt(), ui->tblCat->lineEdit(i, 5)->asDouble());
        } else {
            createRooms(ui->tblCat->toString(i, 0), ui->tblCat->toString(i, 1),
                        ui->tblCat->toInt(i, 4), ui->tblCat->lineEdit(i, 5)->asDouble());
        }
        ui->tblCat->lineEdit(i, 4)->clear();
    }
}

void DlgGroupReservationFuck::save()
{
    if (ui->leGroupName->isEmpty()) {
        message_error_tr("The group name cannot be empty");
        return;
    }
    if (ui->leGroupCode->asInt() == 0) {
        fDbBind[":f_name"] = ui->leGroupName->text();
        ui->leGroupCode->setInt(fDb.insert("f_reservation_group", fDbBind));
        fDbBind[":f_firstName"] = ui->leGroupName->text();
        fDbBind[":f_lastName"] = "";
        fDbBind[":f_title"] = "";
        fDbBind[":f_nation"] = "-";
        ui->leGuest->fHiddenText = QString::number(fDb.insert("f_guests", fDbBind));
        ui->leGuest->setText(ui->leGroupName->text());
        BroadcastThread::cmdRefreshCache(cid_guest, ui->leGuest->fHiddenText);
    }

    fDbBind[":f_name"] = ui->leGroupName->text();
    fDbBind[":f_entry"] = ui->deArrival->date();
    fDbBind[":f_departure"] = ui->deDeparture->date();
    fDbBind[":f_cardex"] = ui->leCardexCode->text();
    fDbBind[":f_arrangement"] = ui->cbArr->currentData();
    fDbBind[":f_price"] = ui->lePrice->asDouble();
    fDbBind[":f_guest"] = ui->leGuest->fHiddenText;
    fDbBind[":f_paymentMode"] = ui->cbModeOfPayment->currentData();
    fDbBind[":f_remarks"] = ui->teCommonRemark->toPlainText();
    fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
    fDbBind[":f_card"] = ui->leCardCode->asInt();
    fDbBind[":f_extra1"] = ui->leExtraname->text();
    fDb.update("f_reservation_group", fDbBind, where_id(ui->leGroupCode->asInt()));

    bool err = false;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        if (ui->tblRoom->toString(i, 17) == "X") {
            err = true;
            continue;
        }
        QString errorStr;
        if (!WReservationRoomTab::check(fDb, ui->tblRoom->lineEdit(i, 1)->asInt(),
            ui->tblRoom->dateEdit(i, 4)->date(), ui->tblRoom->dateEdit(i, 5)->date(), ui->tblRoom->toString(i, 0), errorStr)) {
            err = true;
            ui->tblRoom->setItemWithValue(i, 17, "X");
            continue;
        }
        bool isNew = ui->tblRoom->toString(i, 0).isEmpty();
        QString invId;
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            fDbBind[":f_author"] = WORKING_USERID;
            fDbBind[":f_createTime"] = QDateTime::currentDateTime();
            fDbBind[":f_created"] = QDate::currentDate();
            invId = uuuid("IN", fAirDb);
            fDbBind[":f_invoice"] = invId;
        }
        fDbBind[":f_state"] = RESERVE_RESERVE;
        fDbBind[":f_reserveState"] = CONFIRM_CONFIRM;
        fDbBind[":f_room"] = ui->tblRoom->lineEdit(i, 1)->asInt();
        fDbBind[":f_arrangement"] = ui->tblRoom->comboBox(i, 9)->currentData();
        fDbBind[":f_mealIncluded"] = 0;
        if (ui->tblRoom->toString(i, 15).toInt() == 0) {
            ui->tblRoom->setItemWithValue(i, 15, ui->leGuest->fHiddenText);
            EQLineEdit *lgg = ui->tblRoom->lineEdit(i, 10);
            lgg->setText(ui->leGroupName->text());
            lgg->fHiddenText = ui->leGuest->fHiddenText;
        }
        fDbBind[":f_guest"] = ui->tblRoom->toInt(i, 15);
        fDbBind[":f_man"] = ui->tblRoom->lineEdit(i, 12)->asInt();
        fDbBind[":f_woman"] = ui->tblRoom->lineEdit(i, 13)->asInt();
        fDbBind[":f_child"] = ui->tblRoom->lineEdit(i, 14)->asInt();
        fDbBind[":f_baby"] = 0;
        fDbBind[":f_cardex"] = ui->leCardexCode->text();
        fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
        fDbBind[":f_startDate"] = ui->tblRoom->dateEdit(i, 4)->date();
        fDbBind[":f_endDate"] = ui->tblRoom->dateEdit(i, 5)->date();
        fDbBind[":f_arrivalTime"] = "00:00:00";
        fDbBind[":f_arrivalFee"] = 0;
        fDbBind[":f_departureTime"] = "00:00:00";
        fDbBind[":f_earlyCheckIn"] = 0;
        fDbBind[":f_earlyCheckInTime"] = "00:00:00";
        fDbBind[":f_earlyCheckInFee"] = 0;
        fDbBind[":f_lateCheckOut"] = 0;
        fDbBind[":f_lateCheckOutTime"] = "00:00:00";
        fDbBind[":f_lateCheckOutFee"] = 0;
        fDbBind[":f_paymentType"] = ui->cbModeOfPayment->currentData();
        fDbBind[":f_roomFee"] = ui->tblRoom->lineEdit(i, 6)->asDouble();
        fDbBind[":f_mealQty"] = 0;
        fDbBind[":f_mealPrice"] = 0;
        fDbBind[":f_extraBed"] = 0;
        fDbBind[":f_extraBedFee"] = 0;
        fDbBind[":f_pricePerNight"] = ui->tblRoom->lineEdit(i, 6)->asDouble();
        fDbBind[":f_vat"] = def_vat;
        fDbBind[":f_vatAmount"] = Utils::countVATAmount(ui->tblRoom->lineEdit(i, 6)->asDouble(), VAT_INCLUDED);
        fDbBind[":f_vatMode"] = VAT_INCLUDED;
        fDbBind[":f_total"] = ui->tblRoom->dateEdit(i, 4)->date().daysTo(ui->tblRoom->dateEdit(i, 5)->date()) * ui->tblRoom->lineEdit(i, 6)->asDouble();
        fDbBind[":f_grandTotal"] = ui->tblRoom->dateEdit(i, 4)->date().daysTo(ui->tblRoom->dateEdit(i, 5)->date()) * ui->tblRoom->lineEdit(i, 6)->asDouble();
        fDbBind[":f_totalUSD"] = def_usd;
        fDbBind[":f_novatReason"] = "";
        fDbBind[":f_remarks"] = ui->tblRoom->toString(i, 11);
        fDbBind[":f_commission"] = 0;
        fDbBind[":f_group"] = ui->leGroupCode->asInt();
        fDbBind[":f_booking"] = "";
        fDbBind[":f_pickup"] = 0;
        fDbBind[":f_lastEdit"] = WORKING_USERID;
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            QString rsId = uuuid(VAUCHER_RESERVATION_N, fAirDb);
            fDb.insertId("f_reservation", rsId);
            ui->tblRoom->setItemWithValue(i, 0, rsId);
            fTrackControl->resetChanges();
            fTrackControl->fReservation = ui->tblRoom->toString(i, 0);
            fTrackControl->insert("Created in group: " + ui->leGroupName->text(), "",  "");
            makeTrackControl(i);

        }
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(i, 0))));

        fDbBind[":f_reservation"] = ui->tblRoom->toString(i, 0);
        fDb.select("delete from f_reservation_guests where f_reservation=:f_reservation and f_first=1", fDbBind, fDbRows);
        fDbBind[":f_reservation"] = ui->tblRoom->toString(i, 0);
        fDbBind[":f_guest"] = ui->tblRoom->toString(i, 15);
        fDbBind[":f_first"] = 1;
        fDb.insert("f_reservation_guests", fDbBind);

        QString rid = ui->tblRoom->toString(i, 0);
        if (isNew) {
            fDbBind[":f_id"] = rid;
            fDb.insertWithoutId("m_register", fDbBind);
            fDbBind[":f_inv"] = invId;
            fDbBind[":f_source"] = VAUCHER_RESERVATION_N;
            fDbBind[":f_wdate"] = ui->tblRoom->dateEdit(i, 4)->date();
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_room"] = ui->tblRoom->lineEdit(i, 1)->asInt();
            fDbBind[":f_guest"] = ui->tblRoom->lineEdit(i, 10)->text();
            fDbBind[":f_itemCode"] = fPreferences.getDb(def_reservation_voucher_id);
            fDbBind[":f_finalName"] = tr("RESERVATION ") + ui->tblRoom->toString(i, 0);
            fDbBind[":f_amountAmd"] = ui->tblRoom->dateEdit(i, 4)->date().daysTo(ui->tblRoom->dateEdit(i, 5)->date()) * ui->tblRoom->lineEdit(i, 6)->asDouble();
            fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->tblRoom->lineEdit(i, 6)->asDouble(), VAT_INCLUDED);;
            fDbBind[":f_amountUsd"] = def_usd;
            fDbBind[":f_fiscal"] = 0;
            fDbBind[":f_paymentMode"] = PAYMENT_CASH;
            fDbBind[":f_creditCard"] = 0;
            fDbBind[":f_cityLedger"] = 0;
            fDbBind[":f_paymentComment"] = vaucherPaymentName(PAYMENT_CASH, 0, 0);
            fDbBind[":f_dc"] = "";
            fDbBind[":f_sign"] = 1;
            fDbBind[":f_doc"] = "";
            fDbBind[":f_rec"] = "";
            fDbBind[":f_finance"] = 0;
            fDbBind[":f_remarks"] = ui->tblRoom->toString(i, 11);
            fDbBind[":f_canceled"] = 0;
            fDbBind[":f_cancelReason"] = "";
            fDbBind[":f_side"] = 0;
            fDb.update("m_register", fDbBind, where_id(ap(rid)));
        }

        fDbBind[":f_wdate"] = ui->tblRoom->dateEdit(i, 4)->date();
        fDbBind[":f_room"] = ui->tblRoom->lineEdit(i, 1)->asInt();
        fDbBind[":f_guest"] = ui->tblRoom->toString(i, 10);
        fDbBind[":f_finalName"] = tr("RESERVATION ") + ui->tblRoom->toString(i, 0);
        fDbBind[":f_amountAmd"] = ui->tblRoom->dateEdit(i, 4)->date().daysTo(ui->tblRoom->dateEdit(i, 5)->date()) * ui->tblRoom->lineEdit(i, 6)->asDouble();
        fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->tblRoom->lineEdit(i, 6)->asDouble(), VAT_INCLUDED);;
        fDbBind[":f_amountUsd"] = def_usd;
        fDbBind[":f_fiscal"] = 0;
        fDbBind[":f_paymentMode"] = PAYMENT_CASH;
        fDbBind[":f_creditCard"] = 0;
        fDbBind[":f_cityLedger"] = 0;
        fDbBind[":f_paymentComment"] = vaucherPaymentName(PAYMENT_CASH, 0, 0);
        fDbBind[":f_dc"] = "";
        fDbBind[":f_sign"] = 1;
        fDbBind[":f_doc"] = "";
        fDbBind[":f_rec"] = "";
        fDbBind[":f_finance"] = 0;
        fDbBind[":f_remarks"] = ui->tblRoom->toString(i, 11);
        fDbBind[":f_canceled"] = 0;
        fDbBind[":f_cancelReason"] = "";
        fDbBind[":f_side"] = 0;
        fDb.update("m_register", fDbBind, where_id(ap(rid)));
        BroadcastThread::cmdRefreshCache(cid_reservation, ui->tblRoom->toString(i, 0));
        BroadcastThread::cmdRefreshCache(cid_group_reservation, ui->leGroupCode->text());
        BroadcastThread::cmdRefreshCache(cid_red_reservation, ui->tblRoom->toString(i, 0));

        TrackControl *t = ui->tblRoom->item(i, 0)->data(Qt::UserRole).value<TrackControl*>();
        if (t) {
            if (t->hasChanges()) {
                t->saveChanges();
            }
        }
    }

    if (err) {
        message_error_tr("Some reservation was not saved. Check for 'X' in the table");
    } else {
        message_info_tr("Saved");
        BroadcastThread::cmdRefreshCache(cid_group_reservation, ui->leGroupCode->text());
    }
    countReserve();
}

void DlgGroupReservationFuck::on_btnSave_clicked()
{
    save();
}

void DlgGroupReservationFuck::on_btnRemarksToAll_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        ui->tblRoom->setItemWithValue(i, 11, ui->teCommonRemark->toPlainText());
    }
}

void DlgGroupReservationFuck::on_btnRemarksToAll_2_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        ui->tblRoom->lineEdit(i, 10)->setText(ui->leGuest->text());
        ui->tblRoom->setItemWithValue(i, 15, ui->leGuest->fHiddenText);
    }
}

void DlgGroupReservationFuck::roomTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    int col, row;
    if (!ui->tblRoom->findWidgetCell(static_cast<QWidget*>(sender()), row, col)) {
        return;
    }
    CI_Room *r = CacheRoom::instance()->get(ui->tblRoom->lineEdit(row, 1)->text());
    if (!r) {
        ui->tblRoom->setItemWithValue(row, 17, "X");
        return;
    }
    ui->tblRoom->setItemWithValue(row, 2, r->fCategoryShort);
    ui->tblRoom->setItemWithValue(row, 3, r->fBed);
    QMap<QString, CI_Reservation*> out;
    bool startOk = true;
    bool endOk = true;
    CacheReservation::instance()->check(ui->tblRoom->dateEdit(row, 4)->date(),
                            ui->tblRoom->dateEdit(row, 5)->date(),
                            ui->tblRoom->lineEdit(row, 1)->asInt(), out, startOk, endOk, ui->tblRoom->toString(row, 0));
    if (!startOk || !endOk) {
        ui->tblRoom->setItemWithValue(row, 17, "X");
    } else {
        ui->tblRoom->setItemWithValue(row, 17, "O");
    }
    countTotalReservation();
}

void DlgGroupReservationFuck::openRoomSelectDialog(bool v)
{
    QStringList exclude;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->lineEdit(i, 1)->text().length() > 0) {
            exclude.append(ui->tblRoom->lineEdit(i, 1)->text());
        }
    }

    int row, col;
    if (!ui->tblRoom->findWidgetCell(static_cast<QWidget*>(sender()), row, col)) {
        return;
    }
    CI_Room *r = 0;
    fDockRoom->fSingleSelection = v;
    if (fDockRoom->getRoom(ui->tblRoom->toString(row, 2), ui->tblRoom->dateEdit(row, 4)->date(),
                           ui->tblRoom->dateEdit(row, 5)->date(), exclude, ui->tblCat->toString(row, 1), v) != QDialog::Accepted) {
        return;
    }

    r = fDockRoom->fRoom;
    if (!r) {
        return;
    }
    ui->tblRoom->lineEdit(row, 1)->setText(r->fCode);
    countTotalReservation();

}

void DlgGroupReservationFuck::singleGuestLineEdit(bool v)
{
    Q_UNUSED(v);
    EQLineEdit *lg = static_cast<EQLineEdit*>(sender());
    fDockSingleGuest->setSelector(lg);
    fDockSingleGuest->show();
}

void DlgGroupReservationFuck::singleGuest(CI_Guest *ci)
{
    EQLineEdit *lg = fDockSingleGuest->selector();
    if (!lg) {
        return;
    }
    int row = -1;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->lineEdit(i, 10) == lg) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    if (ci) {
        lg->setText(ci->fName);
        lg->fHiddenText = ci->fCode;
        ui->tblRoom->setItemWithValue(row, 15, ci->fCode);
    } else {
        CI_Guest *g = CacheGuest::instance()->get(lg->fHiddenText);
        if (g) {
            lg->setText(g->fName);
            lg->fHiddenText = g->fCode;
            ui->tblRoom->setItemWithValue(row, 15, g->fCode);
        } else {
            lg->clear();
            lg->fHiddenText = "";
            ui->tblRoom->setItemWithValue(row, 15, "");
        }
    }
}

void DlgGroupReservationFuck::singleGuestFocusOut()
{
    EQLineEdit *lg = fDockSingleGuest->selector();
    if (lg) {
        int row = -1;
        for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
            if (ui->tblRoom->lineEdit(i, 10) == lg) {
                row = i;
                break;
            }
        }
        CI_Guest *ci = CacheGuest::instance()->get(ui->tblRoom->toString(row, 15));
        if (ci) {
            lg->setText(ci->fName);
            lg->fHiddenText = ci->fCode;
        } else {
            CI_Guest *g = CacheGuest::instance()->get(lg->fHiddenText);
            if (g) {
                lg->setText(ci->fName);
                lg->fHiddenText = ci->fCode;
                ui->tblRoom->setItemWithValue(row, 15, ci->fCode);
            } else {
                lg->clear();
                lg->fHiddenText = "";
                ui->tblRoom->setItemWithValue(row, 15, "");
            }
        }
    }
    if (lg) {
        fDockSingleGuest->setSelector(static_cast<EQLineEdit*>(sender()));
    }
}

void DlgGroupReservationFuck::createMultiRoom(int tag)
{
    QStringList exclude;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->lineEdit(i, 1)->text().length() > 0) {
            exclude.append(ui->tblRoom->lineEdit(i, 1)->text());
        }
    }
    int currRow = ui->tblRoom->rowCount();

    ui->tblCat->lineEdit(tag, 4)->clear();

    if (fDockRoom->getRoom(ui->tblCat->toString(tag, 0), ui->deArrival->date(),
                           ui->deDeparture->date(), exclude, ui->tblCat->toString(tag, 1), true) != QDialog::Accepted) {
        return;
    }

    QList<CI_Room*> rooms;
    fDockRoom->getRoomsList(rooms);
    createRooms(ui->tblCat->toString(tag, 0), ui->tblCat->toString(tag, 1),
                rooms.count(), fDockRoom->price());
    foreach (CI_Room *r, rooms) {
        if (currRow == ui->tblRoom->rowCount()) {
            return;
        }
        ui->tblRoom->lineEdit(currRow++, 1)->setText(r->fCode);
    }
    countTotalReservation();
}

void DlgGroupReservationFuck::roomPriceChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countTotalReservation();
}

void DlgGroupReservationFuck::on_btnArrangeToAll_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        ui->tblRoom->comboBox(i, 9)->setCurrentIndex(ui->cbArr->currentIndex());
    }
}

void DlgGroupReservationFuck::on_btnPriceToAll_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        if (ui->tblRoom->toString(i, 2) == ui->cbCat->currentText() && ui->tblRoom->toString(i, 3) == ui->cbBed->currentData().toString()) {
            ui->tblRoom->lineEdit(i, 6)->setText(ui->lePrice->text());
        }
    }
    countTotalReservation();
}

void DlgGroupReservationFuck::on_btnClear_clicked()
{
    for (int i = ui->tblRoom->rowCount() - 1; i > -1; i--) {
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            ui->tblRoom->removeRow(i);
        }
    }
}

void DlgGroupReservationFuck::on_btnDateAll_clicked()
{
    for (int i = 0; i <ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        ui->tblRoom->dateEdit(i, 4)->setDate(ui->deArrival->date());
        ui->tblRoom->dateEdit(i, 5)->setDate(ui->deDeparture->date());
    }
}

void DlgGroupReservationFuck::on_btnArrangeToAll_2_clicked()
{
    if (ui->leCardexCode->isEmpty()) {
        return;
    }
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            continue;
        }
        fDbBind[":f_cardex"] = ui->leCardexCode->text();
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(i, 0))));

        fDbBind[":f_cardex"] = ui->leCardexCode->text();
        fDb.update("f_reservation_group", fDbBind, where_id(ui->leGroupCode->asInt()));

        BroadcastThread::cmdRefreshCache(cid_reservation, ui->tblRoom->lineEdit(i, 1)->text());
        TrackControl *t = ui->tblRoom->item(i, 0)->data(Qt::UserRole).value<TrackControl*>();
        if (t) {
            t->insert("Cardex changed", ui->leCardCode->text(), "");
        }
    }
}

void DlgGroupReservationFuck::on_btnAllPayments_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            continue;
        }
        fDbBind[":f_paymentType"] = ui->cbModeOfPayment->currentData();
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(i, 0))));
        TrackControl *t = ui->tblRoom->item(i, 0)->data(Qt::UserRole).value<TrackControl*>();
        if (t) {
            t->insert("Cardex changed", ui->leCardCode->text(), "");
        }
    }
}

void DlgGroupReservationFuck::on_cbModeOfPayment_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    /*
    ui->leCLCode->clear();
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, 0);


    ui->leCardCode->clear();
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, 0);
    */
}

void DlgGroupReservationFuck::on_btnAllCL_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            continue;
        }
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(i, 0))));
    }

    fDbBind[":f_cityLedger"] = ui->leCardexCode->text();
    fDb.update("f_reservation_group", fDbBind, where_id(ui->leGroupCode->asInt()));
}

void DlgGroupReservationFuck::on_btnAllCard_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toString(i, 0).isEmpty()) {
            continue;
        }
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        fDbBind[":f_card"] = ui->leCardCode->asInt();
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(i, 0))));
    }
}

void DlgGroupReservationFuck::on_tblCat_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        ui->tblRoom->setRowHidden(i, true);
        if (ui->tblCat->toString(row, 0) == ui->tblRoom->toString(i, 2) && ui->tblCat->toString(row, 1) == ui->tblRoom->toString(i, 3)) {
            ui->tblRoom->setRowHidden(i, false);
        }
    }
}

void DlgGroupReservationFuck::on_btnClearFilter_clicked()
{
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        ui->tblRoom->setRowHidden(i, false);
    }
}

void DlgGroupReservationFuck::on_btnPrint_clicked()
{
    PPrintPreview *pv = new PPrintPreview(fMainWindow->fPreferences.getDefaultParentForMessage());
    PPrintScene *ps = pv->addScene(0, Landscape);

    int top = 10;
    //PTextRect *trInfo = new PTextRect(1500, 20, 500, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
    //                                  0, QFont("Arial", 25));
    int rowHeight = 80;
    QPen boldPen(Qt::black);
    boldPen.setWidth(6);
    QFont f("Arial", 30);
    f.setItalic(true);
    f.setBold(true);
    PTextRect th;
    th.setRectPen(boldPen);
    th.setBorders(false, false, false, false);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignLeft);

    ps->addTextRect(20, top, 500, 80, QObject::tr("GROUP RESERVATION") + ": ", &th);
    top += ps->addTextRect(510, top, 2700, 80, ui->leGroupName->text(), &th)->textHeight();
    ps->addTextRect(20, top, 500, 80, QObject::tr("Cardex") + ": ", &th);
    top += ps->addTextRect(510, top, 2700, 80, ui->leCardexCode->text() + "/" + ui->leCardexName->text(), &th)->textHeight();
    ps->addTextRect(20, top, 500, 80, QObject::tr("Arrival") + ": ", &th);
    top += ps->addTextRect(510, top, 2700, 80, ui->deArrival->text(), &th)->textHeight();
    ps->addTextRect(20, top, 500, 80, QObject::tr("Departure") + ": ", &th);
    top += ps->addTextRect(510, top, 2700, 80, ui->deDeparture->text(), &th)->textHeight();
    ps->addTextRect(20, top, 500, 80, QObject::tr("Arrangement") + ": " + ui->deArrival->text(), &th);
    top += ps->addTextRect(510, top, 2700, 80, ui->cbArr->currentText(), &th)->textHeight();

  //  top += trInfo->textHeight();
    top += 10;

    f.setPointSize(30);
    f.setItalic(false);
    f.setBold(false);
    th.setFont(f);
    th.setBorders(true, true, true, true);
    th.setTextAlignment(Qt::AlignHCenter);
    QList<int> cols;
    QStringList vals;

    cols << 60
         << 230
         << 130
         << 130
         << 130
         << 240
         << 240
         << 160
         << 160
         << 250
         << 130
         << 600
         << 100
         << 100
         << 100
         << 250
            ;

    vals << tr("Code")
         << tr("Room")
         << tr("Cat.")
         << tr("Bed")
         << tr("Arrival")
         << tr("Departure")
         << tr("Price")
         << tr("Nights")
         << tr("Total")
         << tr("Arr.")
         << tr("Guest")
         << tr("M")
         << tr("F")
         << tr("Ch")
         << tr("State")
            ;
    ps->addTableRow(top, rowHeight, cols, vals, &th);

    int totMan = 0,
            totNight = 0,
            totFemal = 0,
            totChild = 0;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        vals << ui->tblRoom->toString(i, 0)
             << ui->tblRoom->lineEdit(i, 1)->text()
             << ui->tblRoom->toString(i, 2)
             << ui->tblRoom->toString(i, 3)
             << ui->tblRoom->dateEdit(i, 4)->text()
             << ui->tblRoom->dateEdit(i, 5)->text()
             << ui->tblRoom->lineEdit(i, 6)->text()
             << ui->tblRoom->toString(i, 7)
             << ui->tblRoom->toString(i, 8)
             << ui->tblRoom->comboBox(i, 9)->currentText()
             << ui->tblRoom->lineEdit(i, 10)->text()
             << ui->tblRoom->lineEdit(i, 12)->text()
             << ui->tblRoom->lineEdit(i, 13)->text()
             << ui->tblRoom->lineEdit(i, 14)->text()
             << ui->tblRoom->toString(i, 20)
                ;
        ps->addTableRow(top, rowHeight, cols, vals, &th);
        if (top > sizeLandscape.height() - 200) {
            top = 10;
            ps = pv->addScene(0, Landscape);
        }
        totMan += ui->tblRoom->lineEdit(i, 12)->text().toInt();
        totFemal += ui->tblRoom->lineEdit(i, 13)->text().toInt();
        totChild += ui->tblRoom->lineEdit(i, 14)->text().toInt();
        totNight += ui->tblRoom->toInt(i, 7);
    }

    vals << tr("Total")
         << QString::number(ui->tblRoom->rowCount())
         << ""
         << ""
         << ""
         << ""
         << ""
         << QString::number(totNight)
         << ui->lePreAmount->text()
         << ""
         << ""
         << QString::number(totMan)
         << QString::number(totFemal)
         << QString::number(totChild)
         << ""
            ;
    ps->addTableRow(top, rowHeight, cols, vals, &th);
    if (top > sizeLandscape.height() - 200) {
        top = 10;
        ps = pv->addScene(0, Landscape);
    }

    top += rowHeight;

    top += rowHeight;
    QString footer = QString("%1: %2, %3: %4")
            .arg(QObject::tr("Printed"))
            .arg(QDateTime::currentDateTime().toString(def_date_time_format))
            .arg(QObject::tr("Operator"))
            .arg(WORKING_USERNAME);
    f.setBold(true);
    f.setPointSize(18);
    th.setBorders(false, false, false, false);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignRight);
    ps->addTextRect(20, top, 2100, rowHeight, footer, &th);

    pv->exec();
    delete pv;
}

void DlgGroupReservationFuck::on_btnCancelGroup_clicked()
{
    if (message_confirm(tr("Confirm to removing whole group")) != RESULT_YES) {
        return;
    }
    QString cannotCancel;
    for (int i = 0; i < ui->tblRoom->rowCount(); i++) {
        if (ui->tblRoom->toInt(i, 21) != RESERVE_RESERVE) {
            continue;
        }
        fDbBind[":f_id"] = ui->tblRoom->toString(i, 0);
        DatabaseResult dr;
        dr.select(fDb, "select r.f_invoice, r.f_room, g.guest from f_reservation r "
                  "left join guests g on g.f_id=r.f_guest "
                  "where r.f_id=:f_id", fDbBind);
        if (dr.rowCount() == 0) {
            continue;
        }
        QString r = dr.value("f_room").toString() + ", " + dr.value("guest").toString() + "<br>";
        fDbBind[":f_inv"] = dr.value("f_invoice");
        dr.select(fDb, "select * from m_register where f_inv=:f_inv and f_source='AV' and f_canceled=0 and f_finance=1", fDbBind);
        if (dr.rowCount() > 0) {
            cannotCancel += r;
            continue;
        }
        fDbBind[":f_state"] = RESERVE_REMOVED;
        fDbBind[":f_cancelUser"] = WORKING_USERID;
        fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
        fDb.update("f_reservation", fDbBind, where_id(ap(ui->tblRoom->toString(i, 0))));
        BroadcastThread::cmdRefreshCache(cid_reservation, ui->tblRoom->toString(i, 0));
        TrackControl::insert(TRACK_RESERVATION, "Canceled in group", "", "", "", "", ui->tblRoom->toString(i, 0));
    }
    if (!cannotCancel.isEmpty()) {
        message_info(tr("Next reservations was not canceled, because have an advance") + "<br>" + cannotCancel);
    } else {
        message_info(tr("Reservation group was successfully canceled"));
    }
    loadGroup(ui->leGroupCode->asInt());
}
