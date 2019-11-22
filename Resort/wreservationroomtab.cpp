#include "wreservationroomtab.h"
#include "ui_wreservationroomtab.h"
#include "wguest.h"
#include "cachereservation.h"
#include "cachereservestate.h"
#include "epushbutton.h"
#include "cacherights.h"
#include "dlgadvanceentry.h"
#include "dlgcheckadvancebeforecancel.h"
#include "cacheusers.h"
#include "paymentmode.h"
#include "dlgemail.h"
#include "wreservation.h"
#include "dlgcreategroupreservation.h"
#include "dlggroupreserveoption.h"
#include "dlgadvanceentry.h"
#include "dlgnoshow.h"
#include "dlgprintreservation.h"
#include "cacheinvoiceitem.h"
#include "vauchers.h"
#include "dlgreservationremarks.h"
#include "pprintcheckin.h"
#include <QKeyEvent>

WReservationRoomTab::WReservationRoomTab(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WReservationRoomTab)
{
    ui->setupUi(this);
    fReservation = static_cast<WReservation*>(parent);
    fDockItemSide = new DWReservationItemSize(fItemSide, this);
    fDockItemSide->hide();
    fDockCardex = new DWSelectorCardex(this);
    fDockCardex->configure();
    fDockCardex->setSelector(ui->leCardexCode);
    fDockCityLedger = new DWSelectorCityLedger(this);
    fDockCityLedger->configure();
    fDockCityLedger->setSelector(ui->leCityLedgerCode);
    fDockReserveStatus = new DWSelectorReserveStatus();
    fDockReserveStatus->configure();
    fDockReserveStatus->setSelector(ui->leStatusCode);
    fDockGuest = new DWSelectorGuest(this);
    fDockGuest->configure();
    fDockGuest->setSelector(ui->leSearchGuest);
    connect(fDockCardex, SIGNAL(cardex(CI_Cardex*)), this, SLOT(cardex(CI_Cardex*)));
    connect(fDockCityLedger, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));
    connect(fDockReserveStatus, SIGNAL(reserveStatus(CI_ReserveStatus*)), this, SLOT(reserveStatus(CI_ReserveStatus*)));
    connect(fDockGuest, SIGNAL(guest(CI_Guest*)), this, SLOT(guest(CI_Guest*)));
    connect(ui->leRoomCode, SIGNAL(customButtonClicked(bool)), this, SLOT(roomSearch(bool)));
    connect(CacheRoom::instance(), SIGNAL(cacheUpdated(QString)), this, SLOT(roomCacheUpdated(QString)));
    QList<int> fWidths;
    fWidths << 0 << 50 << 220 << 30 << 30 << 30;
    for (int i = 0; i < ui->tblGuest->columnCount(); i++) {
        ui->tblGuest->setColumnWidth(i, fWidths.at(i));
    }
    fReservRoomId = 0;

    ui->deEntry->setDate(QDate::currentDate());
    ui->deDeparture->setDate(QDate::currentDate());
    ui->lbVAT->setText(QString("%1 %2%")
            .arg(tr("VAT"))
            .arg(QString::number(def_vat, 'f', 2)));
    ui->leReserveCode->setInt(RESERVE_RESERVE);
    ui->leReserveName->setText(CacheReserveState::instance()->get(RESERVE_RESERVE)->fName);

    ui->cbPaymentType->setIndexForData(PAYMENT_CASH);
    ui->cbVAT->setIndexForData(VAT_INCLUDED);
    ui->leNoVATReason->setEnabled(ui->cbVAT->asInt() == VAT_NOVAT);

    setEarlyCheckIn(false);
    on_chLateCheckout_clicked(false);
    on_chExtraBed_stateChanged(0);
    ui->leArrivalFee->setValidator(new QDoubleValidator(0, 999999, 1));
    fStartDateOk = true;
    fEndDateOk = true;
    fCardexOk = true;
    fCityLedgerOk = true;
    fAuthor = WORKING_USERNAME;
    fLastModify = WORKING_USERNAME;

    ui->leStatusCode->setInt(CONFIRM_CONFIRM);
    ui->leStatusName->setText(CacheReserveStatus::instance()->get(1)->fName);
    ui->chMealIncluded->setChecked(true);
    ui->leCardexCode->setText(fPreferences.getDb(def_default_cardex).toString());
    cardex(CacheCardex::instance()->get(ui->leCardexCode->text()));
    on_chMealIncluded_clicked(true);
    ui->btnAppendAdvance->setVisible(RIGHT(WORKING_USERGROUP, cr__advance_vaucher));

    connect(ui->teRoomRemarks, &EQTextEdit::doubleClicked, [this](){
        QString remark = ui->teRoomRemarks->toPlainText();
        if (DlgReservationRemarks::Remarks(remark)) {
            ui->teRoomRemarks->setPlainText(remark);
        }
    });

    connect(ui->leRoomCode, &EQLineEdit::focusOut, [this]() {
        checkDatesCross();
    });

}

WReservationRoomTab::~WReservationRoomTab()
{
    delete ui;
}

bool WReservationRoomTab::check(Database &db, int room, const QDate &start, const QDate &end, const QString &rid, QString &errorMsg)
{
    if (room == 0) {
        return true;
    }
    QMap<QString, QVariant> bind;
    bind[":f_room"] = room;
    bind[":f_s1"] = 1;
    bind[":f_s2"] = 2;
    bind[":f_s3"] = 9;
    bind[":f_s4"] = 4;
    DatabaseResult dr;
    dr.select(db, "select * from f_reservation where (f_state=:f_s1 or f_state=:f_s2 or f_state=:f_s3 or f_state=:f_s4) and f_room=:f_room order by f_id ", bind);
    if (dr.rowCount() == 0) {
        return true;
    }
    for (int i = 0; i < dr.rowCount(); i++) {
        if (!rid.isEmpty()) {
            if (rid == dr.value(i, "f_id").toString()) {
                continue;
            }
        }
        if (dr.value(i, "f_state").toInt() == RESERVE_OUTOFINVENTORY || dr.value(i, "f_state").toInt() == RESERVE_OUTOFROOM) {
            if (start == dr.value(i, "f_endDate").toDate() || end == dr.value(i, "f_startDate").toDate()) {
                errorMsg = tr("Conflict reservation id") + ": " + dr.value(i, "f_id").toString();
                return false;
            }
        }
        if (start == end) {
            if (dr.value(i, "f_startDate").toDate() == dr.value(i, "f_endDate").toDate()) {
                if (dr.value(i, "f_startDate").toDate() == start) {
                    errorMsg = tr("Conflict reservation id") + ": " + dr.value(i, "f_id").toString();
                    return false;
                }
            }
            if (start == dr.value(i, "f_endDate").toDate()) {

            }
        }
        if ((start <= dr.value(i, "f_startDate").toDate())
                && (end >= dr.value(i, "f_endDate").toDate())
                && (dr.value(i, "f_startDate").toDate() != dr.value(i, "f_endDate").toDate())) {
            errorMsg = tr("Conflict reservation id") + ": " + dr.value(i, "f_id").toString();
            return false;
        }
        if (start > dr.value(i, "f_startDate").toDate() && end < dr.value(i, "f_endDate").toDate()) {
            errorMsg = tr("Conflict reservation id") + ": " + dr.value(i, "f_id").toString();
            return false;
        }
        if (start <= dr.value(i, "f_startDate").toDate()) {
            if (end > dr.value(i, "f_startDate").toDate()) {
                if (dr.value(i, "f_startDate").toDate() != dr.value(i, "f_endDate").toDate()) {
                    errorMsg = tr("Conflict reservation id") + ": " + dr.value(i, "f_id").toString();
                    return false;
                }
            }
        }
        if (end > dr.value(i, "f_endDate").toDate()) {
            if (start < dr.value(i, "f_endDate").toDate()) {
                errorMsg = tr("Conflict reservation id") + ": " + dr.value(i, "f_id").toString();
                return false;
            }
        }
    }
    return true;
}

void WReservationRoomTab::setBaseData(const QDate &date1, const QDate &date2, CI_Room *room)
{
    disconnectSignals();
    ui->deEntry->setDate(date1);
    ui->deDeparture->setDate(date2);
    ui->sbNights->setValue(date1.daysTo(date2));
    if (room) {
        ui->leRoomCode->setText(room->fCode);
        on_leRoomCode_returnPressed();
    }
    countTotal();
    connectSignals();
    checkDatesCross();
    emit roomChanged(ui->leRoomName->text(), fTabIndex);
}

void WReservationRoomTab::setTimes(const QTime &arrival, const QTime &departure)
{
    ui->teArrivalTime->setTime(arrival);
    ui->teDepartureTime->setTime(departure);
}

void WReservationRoomTab::setCardex(CI_Cardex *c)
{
    if (c) {
        ui->leCardexCode->setText(c->fCode);
        ui->leCardexname->setText(c->fName);
        ui->leCommission->setText(c->fCommission);
        ui->cbVAT->setIndexForData(c->fVatMode.toInt());
    } else {
        ui->leCardexCode->clear();
        ui->leCardexname->clear();
    }
}

void WReservationRoomTab::setArrangement(int id)
{
    ui->cbArrangment->setIndexForData(id);
}

void WReservationRoomTab::setRoomPrice(const QString &text)
{
    ui->leRooming->setText(text);
    countTotal();
}

void WReservationRoomTab::setRemarks(const QString &remarks)
{
    ui->teRoomRemarks->setPlainText(remarks);
}

bool WReservationRoomTab::save()
{
    if (ui->leRoomCode->asInt() == 0) {
        if (ui->leReserveCode->asInt() == ROOM_STATE_CHECKIN) {
            message_error(tr("Cannot save reservation. Room code for checkin reservation must be specified"));
            return false;
        }
    }
    QString errorStr;
    if (!check(fDb, ui->leRoomCode->asInt(), ui->deEntry->date(), ui->deDeparture->date(), ui->leReservId->text(), errorStr)) {
        message_error(tr("Cannot save reservation") + "<br>" + errorStr);
        return false;
    }
    if (ui->leStatusCode->asInt() == 0) {
        message_error(tr("Cannot save reservation, status code is empty"));
        return false;
    }
    bool result = true;
    int createUser = 0;
    bool changeRoom = (ui->leReserveCode->asInt() == RESERVE_CHECKIN || ui->leReserveCode->asInt() == RESERVE_RESERVE);
    fDb.fDb.transaction();
    if (ui->leReservId->isEmpty() || ui->leReserveCode->fHiddenText == "1") {
        ui->leReserveCode->setInt(RESERVE_RESERVE);
        ui->leReserveName->setText(CacheReserveState::instance()->get(RESERVE_RESERVE)->fName);
        ui->deCreated->setDate(WORKING_DATE);
        createUser = WORKING_USERID;
    }
    QString oldRoom = fTrackControl->oldValue(ui->leRoomCode);;
    if (ui->leReservId->notEmpty() && changeRoom) {
        if (fTrackControl->isValueChanged(ui->leRoomCode)) {
            /*------------- BEGIN CHANGE ROOM ------------------*/
            fDbBind[":f_upgradeFrom"] = fTrackControl->oldValue(ui->leRoomCode).toInt();
            result = result && fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservId->text())));
            if (result) {
                if (ui->leReserveCode->asInt() == RESERVE_CHECKIN) {
                    fDbBind[":f_state"] = ROOM_STATE_DIRTY;
                    result = result && fDb.update("f_room", fDbBind, where_id(fTrackControl->oldValue(ui->leRoomCode).toInt()));
                    fDbBind[":f_state"] = ROOM_STATE_CHECKIN;
                    result = result && fDb.update("f_room", fDbBind, where_id(ui->leReserveCode->asInt()));
                }
            }
            if (ui->leReserveCode->asInt() == RESERVE_CHECKIN) {
                fDbBind[":f_state"] = ROOM_STATE_DIRTY;
                result = result && fDb.update("f_room", fDbBind, where_id(fTrackControl->oldValue(ui->leRoomCode).toInt()));
                fDbBind[":f_state"] = ROOM_STATE_CHECKIN;
                result = result && fDb.update("f_room", fDbBind, where_id(ui->leRoomCode->asInt()));
            }
            if (fTrackControl->oldValue(ui->leRoomCode).toInt() > 0) {
                ui->leUpgradeFrom->setText(CacheRoom::instance()->get(fTrackControl->oldValue(ui->leRoomCode).toInt())->fName);
            }
            /*-------------- END CHANGE ROOM ------------------*/
        }
        if (fTrackControl->isValueChanged(ui->leRooming) && result) {
            /* ------------------- BEGIN CHANGE ROOM RATE -----------------*/

            QString vid = ui->leReservId->text();
            QString rid = uuuid(VAUCHER_ROOM_RATE_N, fAirDb);
            fDb.insertId("m_register", rid);
            fDbBind[":f_source"] = VAUCHER_ROOM_RATE_N;
            fDbBind[":f_wdate"] = WORKING_DATE;
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_room"] = ui->leRoomCode->text();
            fDbBind[":f_guest"] = ui->tblGuest->item(0, 2)->text();
            fDbBind[":f_itemCode"] = fPreferences.getDb(def_room_rate_change_id);
            fDbBind[":f_finalName"] = tr("ROOM RATE CHANGE FOR #") + ui->leReservId->text();
            fDbBind[":f_amountAmd"] = ui->leRooming->asDouble() - fTrackControl->oldValue(ui->leRooming).toDouble();
            fDbBind[":f_amountVat"] = 0;
            fDbBind[":f_amountUsd"] = def_usd;
            fDbBind[":f_fiscal"] = 0;
            fDbBind[":f_paymentMode"] = ui->cbPaymentType->currentData();
            fDbBind[":f_creditCard"] = 0;
            fDbBind[":f_cityLedger"] = ui->leCityLedgerCode->asInt();
            fDbBind[":f_paymentComment"] = vaucherPaymentName(ui->cbPaymentType->currentData().toInt(), 0, ui->leCityLedgerCode->text());
            fDbBind[":f_dc"] = "";
            fDbBind[":f_sign"] = 0;
            fDbBind[":f_doc"] = vid;
            fDbBind[":f_rec"] = 0;
            fDbBind[":f_finance"] = 0;
            fDbBind[":f_remarks"] = ui->teRoomRemarks->toPlainText();
            fDbBind[":f_canceled"] = 0;
            fDbBind[":f_cancelReason"] = "";
            fDbBind[":f_side"] = 0;
            fDb.update("m_register", fDbBind, where_id(ap(rid)));
            /* ------------------- END CHANGE ROOM RATE -----------------*/
        }
    }
    if (createUser > 0) {
        fDbBind[":f_author"] = createUser;
        fDbBind[":f_createTime"] = QDateTime::currentDateTime();
        fDbBind[":f_created"] = ui->deCreated->date();
    }
    fDbBind[":f_state"] = ui->leReserveCode->asInt();
    fDbBind[":f_reserveState"] = ui->leStatusCode->asInt();
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDbBind[":f_room"] = ui->leRoomCode->asInt();
    fDbBind[":f_arrangement"] = ui->cbArrangment->currentData();
    fDbBind[":f_mealIncluded"] = (int) ui->chMealIncluded->isChecked();
    fDbBind[":f_guest"] = firstGuestId();
    fDbBind[":f_man"] = ui->sbMan->value();
    fDbBind[":f_woman"] = ui->sbWoman->value();
    fDbBind[":f_child"] = ui->sbChild->value();
    fDbBind[":f_baby"] = ui->sbBaby->value();
    fDbBind[":f_cardex"] = ui->leCardexCode->text();
    fDbBind[":f_cityLedger"] = ui->leCityLedgerCode->asInt();
    fDbBind[":f_startDate"] = ui->deEntry->date();
    fDbBind[":f_endDate"] = ui->deDeparture->date();
    fDbBind[":f_arrivalTime"] = ui->teArrivalTime->time();
    fDbBind[":f_arrivalFee"] = ui->leArrivalFee->asDouble();
    fDbBind[":f_departureTime"] = ui->teDepartureTime->time();
    fDbBind[":f_earlyCheckIn"] = (int) ui->chEarlyCheckIn->isChecked();
    fDbBind[":f_earlyCheckInTime"] = ui->teEarlyCheckIn->time();
    fDbBind[":f_earlyCheckInFee"] = ui->leEarlyCheckInFee->asDouble();
    fDbBind[":f_lateCheckOut"] = (int) ui->chLateCheckout->isChecked();
    fDbBind[":f_lateCheckOutTime"] = ui->teLateChecout->time();
    fDbBind[":f_lateCheckOutFee"] = ui->leLateCheckoutFee->asDouble();
    fDbBind[":f_paymentType"] = ui->cbPaymentType->asInt();
    fDbBind[":f_roomFee"] = ui->leRooming->asDouble();
    fDbBind[":f_mealQty"] = ui->sbMealQty->value();
    fDbBind[":f_mealPrice"] = ui->leMealPrice->asDouble();
    fDbBind[":f_extraBed"] = ui->chExtraBed->isChecked();
    fDbBind[":f_extraBedFee"] = ui->leExtraBedAmount->asDouble();
    fDbBind[":f_pricePerNight"] = ui->lePricePerNight->asDouble();
    fDbBind[":f_vat"] = def_vat;
    fDbBind[":f_vatAmount"] = ui->leVAT->asDouble();
    fDbBind[":f_vatMode"] = ui->cbVAT->asInt();
    fDbBind[":f_total"] = ui->leTotal->asDouble();
    fDbBind[":f_grandTotal"] = ui->leGrandTotal->asDouble();
    fDbBind[":f_totalUSD"] = ui->leTotalUSD->asDouble();
    fDbBind[":f_novatReason"] = ui->leNoVATReason->text();
    fDbBind[":f_remarks"] = ui->teRoomRemarks->toPlainText();
    fDbBind[":f_commission"] = ui->leCommission->asDouble();
    fDbBind[":f_group"] = fReservation->groupId();
    fDbBind[":f_booking"] = ui->leBooking->text();
    fDbBind[":f_pickup"] = (int) ui->chPickup->isChecked();
    fDbBind[":f_lastEdit"] = WORKING_USERID;
    if (result) {
        if (ui->leReservId->isEmpty()) {
            QString rsId = uuuid(VAUCHER_RESERVATION_N, fAirDb);
            result = fDb.insertId("f_reservation", rsId);
            fDbBind[":f_author"] = WORKING_USERID;
            ui->leReservId->setText(rsId);
            QString invId = uuuid("IN", fAirDb);
            ui->leInvoice->setText(invId);
            fDbBind[":f_invoice"] = invId;
            fTrackControl->fReservation = rsId;
            fTrackControl->fInvoice = invId;
        }
    }
    if (fTrackControl->hasChanges()) {
        fDbBind[":f_lastEdit"] = WORKING_USERID;
    }
    if (result) {
        result = fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservId->text())));
    }
    if (createUser > 0) {
        fDbBind[":f_id"] = ui->leReservId->text();
        fDb.select("update f_reservation_last set f_id=:f_id", fDbBind, fDbRows);
    }
    if (result) {
        result = result && fDb.queryDirect(QString("delete from f_reservation_guests where f_reservation='%1'").arg(ui->leReservId->text()));
        if (result) {
            for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
                fDbBind[":f_reservation"] = ui->leReservId->text();
                fDbBind[":f_guest"] = ui->tblGuest->item(i, 0)->data(Qt::DisplayRole).toInt();
                if (i == 0) {
                    fDbBind[":f_first"] = 1;
                } else {
                    fDbBind[":f_first"] = 0;
                }
                result = result && (fDb.insert("f_reservation_guests", fDbBind) > 0);
                if (!result) {
                    break;
                }
            }
        }
    }
    bool first = true;
    QString query = "";
    for (QList<int>::const_iterator it =  fItemSide.begin(); it != fItemSide.end(); it++) {
        if (first) {
            first = false;
        } else {
            query += ",";
        }
        query += QString("('%1', %2)").arg(ui->leReservId->text()).arg(*it);
    }
    if (query.length() > 0) {
        query = "insert into f_reservation_item_side (f_reservation, f_itemId) values " + query;
        if (result) {
            result = fDb.queryDirect(query);
        }
    }
    if (result && !oldRoom.isEmpty()) {
        fDbBind[":f_room"] = ui->leRoomCode->text();
        result = result && (fDb.select("update m_register set f_room=:f_room where f_inv=" + ap(ui->leInvoice->text()), fDbBind, fDbRows) != -1);
    }
    if (result) {
        ui->btnAppendAdvance->setEnabled(true);
        fDb.fDb.commit();
        emit commonChanges();
        fTrackControl->saveChanges();
        saveVaucher();
        BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
        BroadcastThread::cmdRefreshCache(cid_room, ui->leRoomCode->text());
        BroadcastThread::cmdRefreshCache(cid_red_reservation, ui->leReservId->text());
        if (!oldRoom.isEmpty()) {
            BroadcastThread::cmdRefreshCache(cid_room, oldRoom);
            BroadcastThread::cmdRefreshCache(cid_active_room, oldRoom);
            BroadcastThread::cmdRefreshCache(cid_active_room, ui->leRoomCode->text());

        }
    } else {
        fDb.fDb.rollback();
    }
    return result;
}

void WReservationRoomTab::print()
{
    DlgPrintReservation *d = new DlgPrintReservation(this);
    d->exec();
    delete d;
}

QString WReservationRoomTab::roomName()
{
    return ui->leRoomCode->text();
}

QString WReservationRoomTab::roomCode()
{
    return ui->leRoomCode->text();
}

QDate WReservationRoomTab::date1()
{
    return ui->deEntry->date();
}

QDate WReservationRoomTab::date2()
{
    return ui->deDeparture->date();
}

QString WReservationRoomTab::docId()
{
    return ui->leReservId->text();
}

void WReservationRoomTab::loadReservation(const QString &id)
{
    disconnectSignals();
    ui->leReservId->setText(id);
    QString query = "select "
            "f_state,"
            "f_reserveState,"
            "f_invoice,"
            "f_room,"
            "f_arrangement,"
            "f_mealIncluded,"
            "f_invoice,"
            "f_guest,"
            "f_man,"
            "f_woman,"
            "f_child,"
            "f_baby,"
            "f_cardex,"
            "f_cityLedger,"
            "f_startDate,"
            "f_endDate,"
            "f_arrivalTime,"
            "f_arrivalFee,"
            "f_departureTime,"
            "f_earlyCheckIn,"
            "f_earlyCheckInTime,"
            "f_earlyCheckInFee,"
            "f_lateCheckOut,"
            "f_lateCheckOutTime,"
            "f_lateCheckOutFee,"
            "f_paymentType,"
            "f_roomFee,"
            "f_mealQty,"
            "f_mealPrice,"
            "f_extraBed,"
            "f_extraBedFee,"
            "f_pricePerNight,"
            "f_vat,"
            "f_vatAmount,"
            "f_vatMode,"
            "f_total,"
            "f_grandTotal,"
            "f_totalUSD,"
            "f_novatReason,"
            "f_remarks, "
            "f_commission,"
            "f_upgradeFrom, "
            "f_created, "
            "f_author,"
            "f_lastEdit, "
            "f_group, "
            "f_booking, "
            "f_pickup "
            "from f_reservation "
            "where f_id=" + ap(ui->leReservId->text());
    fDb.select(query, fDbBind, fDbRows);
    if (fDbRows.at(0).at(1).toInt() == RESERVE_SERVICE) {
        startTrackControl();
        fDbBind[":f_table"] = TRACK_RESERVATION;
        fDbBind[":f_windowId"] = ui->leReservId->text();
        //fDb.select("delete from f_changes_tracking where f_table=:f_table and f_windowId=:f_windowsId", fDbBind, fDbRows);
        fTrackControl->resetChanges();
    }
    int c = 0;
    if (fDbRows.count() > 0) {
        QList<QVariant> &row = fDbRows[0];
        ui->leReserveCode->setInt(row.at(c++).toInt());
        if (ui->leReserveCode->asInt() == RESERVE_SERVICE) {
            ui->leReserveCode->setInt(RESERVE_RESERVE);
            ui->leReserveCode->fHiddenText = "1";
        }
        ui->leReserveName->setText(CACHERESERVESTATE_NAME(ui->leReserveCode->asInt()));
        ui->leStatusCode->setInt(row.at(c++).toInt());
        if (ui->leStatusCode->asInt() == CONFIRM_BLOCK) {
            ui->leStatusCode->setInt(CONFIRM_TENTATIVE);
        }
        ui->leStatusName->setText(CACHERESERVESTATUS_NAME(ui->leStatusCode->asInt()));
        ui->leInvoice->setText(row.at(c++).toString());
        ui->leRoomCode->setText(row.at(c++).toString());
        ui->cbArrangment->setIndexForData(row.at(c++));
        ui->chMealIncluded->setChecked((bool) row.at(c++).toInt());
        c++; //Skip invoice
        c++; //Skip main guest
        ui->sbMan->setValue(row.at(c++).toInt());
        ui->sbWoman->setValue(row.at(c++).toInt());
        ui->sbChild->setValue(row.at(c++).toInt());
        ui->sbBaby->setValue(row.at(c++).toInt());
        ui->leCardexCode->setText(row.at(c++).toString());
        ui->leCityLedgerCode->setText(row.at(c++).toString());
        ui->deEntry->setDate(row.at(c++).toDate());
        ui->deDeparture->setDate(row.at(c++).toDate());
        ui->sbNights->setValue(ui->deEntry->date().daysTo(ui->deDeparture->date()));
        ui->teArrivalTime->setTime(row.at(c++).toTime());
        ui->leArrivalFee->setText(row.at(c++).toString());
        ui->teDepartureTime->setTime(row.at(c++).toTime());
        ui->chEarlyCheckIn->setChecked(row.at(c++).toInt());
        ui->teEarlyCheckIn->setTime(row.at(c++).toTime());
        ui->leEarlyCheckInFee->setText(row.at(c++).toString());
        ui->chLateCheckout->setChecked(row.at(c++).toInt());
        ui->teLateChecout->setTime(row.at(c++).toTime());
        ui->leLateCheckoutFee->setDouble(row.at(c++).toDouble());
        ui->cbPaymentType->setIndexForData(row.at(c++).toInt());
        ui->leRooming->setText(row.at(c++).toString());
        ui->sbMealQty->setValue(row.at(c++).toInt());
        ui->leMealPrice->setText(row.at(c++).toString());
        ui->chExtraBed->setChecked(row.at(c++).toInt());
        ui->leExtraBedAmount->setText(row.at(c++).toString());
        ui->lePricePerNight->setText(row.at(c++).toString());
        c++; //Skip VAT value
        ui->leVAT->setText(row.at(c++).toString());
        ui->cbVAT->setIndexForData(row.at(c++).toInt());
        ui->leTotal->setText(row.at(c++).toString());
        ui->leGrandTotal->setText(row.at(c++).toString());
        ui->leTotalUSD->setText(row.at(c++).toString());
        ui->leNoVATReason->setText(row.at(c++).toString());
        ui->teRoomRemarks->setPlainText(row.at(c++).toString());
        ui->leCommission->setText(row.at(c++).toString());
        QString roomUpgradeFrom = row.at(c++).toString();
        if (roomUpgradeFrom.toInt() > 0) {
            ui->leUpgradeFrom->setText(CacheRoom::instance()->get(roomUpgradeFrom)->fName);
        }
        ui->deCreated->setDate(row.at(c++).toDate());
        CI_User *cu = CacheUsers::instance()->get(row.at(c++).toString());
        if (cu) {
            fAuthor = cu->fName;
        }
        cu = CacheUsers::instance()->get(row.at(c++).toString());
        if (cu) {
            fLastModify = cu->fName;
        }
        c++; // skip group id
        ui->leBooking->setText(row.at(c++).toString());
        ui->chPickup->setChecked(row.at(c++).toInt());
        /// END OF ROW


        ui->leRoomName->clear();
        if (ui->leRoomCode->asInt() == 0) {
            ui->leRoomCode->clear();
            ui->lbRoom->setPixmap(QPixmap(":/images/question.png"));
        }
        CI_Room *r = CacheRoom::room(ui->leRoomCode->text());
        if (!r) {
            ui->lbRoom->setPixmap(QPixmap(":/images/warning.png"));
        } else {
            ui->lbRoom->setPixmap(QPixmap(":/images/ok.png"));
            ui->leRoomName->setText(r->fName);
            emit roomChanged(r->fName, fTabIndex);
        }

        checkDatesCross();
        int vatIndex = ui->cbVAT->currentIndex();
        cardex(0);
        ui->cbVAT->setCurrentIndex(vatIndex);
        if (ui->leReserveCode->asInt() == RESERVE_SERVICE) {
            ui->cbVAT->setCurrentIndex(0);
        }
        cityLedger(0);
    } else {
        ui->cbVAT->setCurrentIndex(0);
    }

    query = "select r.f_guest, g.f_title, concat(g.f_firstName, ' ' , g.f_lastName), "
            "g.f_tel1, g.f_remarks "
            "from f_reservation_guests r "
            "left join f_guests g on g.f_id=r.f_guest "
            "where r.f_reservation=:f_reservation "
            "order by r.f_first desc ";
    fDbBind[":f_reservation"] = ui->leReservId->text();
    fDb.select(query, fDbBind, fDbRows);
    ui->tblGuest->clearContents();
    ui->tblGuest->setRowCount(0);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        CI_Guest *g = CacheGuest::instance()->get(it->at(0).toString());
        addGuest(g, false);
    }
    getAdvance();
    ui->btnAppendAdvance->setEnabled(ui->leReserveCode->asInt() == RESERVE_RESERVE);
    ui->leNoVATReason->setEnabled(ui->cbVAT->asInt() == VAT_NOVAT);
    on_cbPaymentType_currentIndexChanged(0);
    fItemSide.clear();
    query = "select f_itemId from f_reservation_item_side where f_reservation=:f_reservation";
    fDbBind[":f_reservation"] = ui->leReservId->text();
    fDb.select(query, fDbBind, fDbRows);
    foreach_rows {
        fItemSide.append(it->at(0).toInt());
    }
    //Meal
    ui->sbMealQty->setEnabled(!ui->chMealIncluded->isChecked());
    ui->leMealPrice->setEnabled(!ui->chMealIncluded->isChecked());
    connectSignals();
    if (ui->leReserveCode->asInt() == RESERVE_CHECKIN)  {
        ui->deEntry->setEnabled(false);
    }
    bool editable = ui->leReserveCode->asInt() == RESERVE_RESERVE
            || ui->leReserveCode->asInt() == RESERVE_CHECKIN
            || ui->leReserveCode->asInt() == RESERVE_SERVICE;
    setGroupBoxesEnabled(editable);
    emit commonChanges();

    fTrackControl->fInvoice = ui->leInvoice->text();
    fTrackControl->fReservation = ui->leReservId->text();
    if (ui->leReserveCode->asInt() == RESERVE_SERVICE) {
        fTrackControl->saveChanges();
    }
    fTrackControl->resetChanges();
}

void WReservationRoomTab::setGuest(int id, bool removeFirst)
{
    CI_Guest *g = CacheGuest::instance()->get(id);
    if (!g) {
        return;
    }
    if (removeFirst) {
        for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
            fTrackControl->insert("Guest removed", ui->tblGuest->item(0, 1)->text() + " " + ui->tblGuest->item(0, 2)->text(), "");
            ui->tblGuest->removeRow(0);
        }
    }
    addGuest(g, true);
}

bool WReservationRoomTab::canRecheckin()
{
    bool result = ui->leReserveCode->asInt() == RESERVE_CHECKOUT;
    CI_Room *c = CacheRoom::instance()->get(ui->leRoomCode->text());
    if (!c) {
        return false;
    }
    result = result && c->fState == ROOM_STATE_NONE;
    result = result && ui->deDeparture->date() == WORKING_DATE;
    return result;
}

void WReservationRoomTab::reCheckin()
{
    QString errors;
    fDbBind[":f_room"] = ui->leRoomCode->asInt();
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select f_id from f_reservation where f_room=:f_room and f_state=:f_state", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        errors += tr("Cannot checkin this room, becouse another reservation active") + "<br>";
    }
    if (ui->deDeparture->date() < WORKING_DATE) {
        errors += tr("Cannot re-checkin, date was expired") + "<br>";
    }
    CI_Room *c = CacheRoom::instance()->get(ui->leRoomCode->text());
    if (!c) {
        errors += tr("Program error. Contact to developer. Message: ReCheckIn: CI_Room==0") + "<br>";
    }
    if (c->fState != ROOM_STATE_NONE) {
        errors += tr("Change room state to 'Vacant ready' first") + "<br>";
    }
    bool passport = false;
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        CI_Guest *g = CacheGuest::instance()->get(ui->tblGuest->toString(i, 0));
        if (!g) {
            continue;
        }
        if (g->fNatShort.isEmpty() || g->fNatShort == "-") {
            errors += tr("Nationality for ") + g->fName + tr(" must be defined") + "<br>";
        }
        if (!g->fPassport.isEmpty()) {
            passport = true;
        }
    }
    if (!passport && fPreferences.getDb(def_passport_required).toBool()) {
        errors += tr("Guest(s) dont have a passport?");
    }
    if (!errors.isEmpty()) {
        message_error(errors);
        return;
    }
    ui->leReserveCode->setInt(RESERVE_CHECKIN);
    ui->leReserveName->setText(CacheReserveState::instance()->get(RESERVE_CHECKIN)->fName);
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservId->text())));
    fDbBind[":f_state"] = ROOM_STATE_CHECKIN;
    fDb.update("f_room", fDbBind, where_id(ui->leRoomCode->asInt()));
    fTrackControl->insert("Recheckin", "", "");
    setGroupBoxesEnabled(true);
    save();
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
    BroadcastThread::cmdRefreshCache(cid_room, ui->leRoomCode->text());
    BroadcastThread::cmdRefreshCache(cid_active_room, ui->leRoomCode->text());
    message_info_tr("Re-checkin completed");
}

bool WReservationRoomTab::canCheckIn(QString &why)
{
    bool result = ui->leReservId->notEmpty();
    if (ui->leReservId->isEmpty()) {
        why += tr("Reservation not saved\r\n");
    }
    result = result && ui->leRoomName->text().length() > 0;
    if (ui->leRoomCode->asInt() == 0) {
        why += tr("Room is not selected\r\n");
    }
    result = result && ui->deEntry->date() == WORKING_DATE;
    if (ui->deEntry->date() != WORKING_DATE) {
        why += tr("Date of entry is not equal to working date\r\n");
    }
    result = result && ui->leReserveCode->asInt() == RESERVE_RESERVE;
    if (ui->leReserveCode->asInt() != RESERVE_RESERVE) {
        why += tr("Reservatin state should be 'Reserved'\r\n");
    }
    CI_Room *r =  CacheRoom::instance()->get(ui->leRoomCode->text());
    result = result && r;
    if (r) {
        result = result && r->fState == ROOM_STATE_NONE;
        if (r->fState != ROOM_STATE_NONE) {
            why += tr("Room state is not vacant ready\r\n");
        }
    }
    return result;
}

bool WReservationRoomTab::checkIn(QString &errorString)
{
    bool error = false;
    if (QTime::currentTime() < fPreferences.getDb(def_earyly_checkin).toTime()) {
        if (!ui->chEarlyCheckIn->isChecked() || ui->leEarlyCheckInFee->asDouble() < 0.01) {
            if (fPreferences.getDb(def_earyly_checkin_id).toInt() == 0) {
                message_error_tr("Early checkin code not define in the settings. Contact to administrator.");
                return false;
            }
            switch (message_yesnocancel("Charge early checkin fee? Yes - return to editor")) {
            case RESULT_YES:
                return false;
            case RESULT_NO:
                break;
            case RESULT_CANCEL:
                return false;
            }
        }
    }
    ui->lbRoom->setStyleSheet("color:black");
    if (ui->leRoomName->text().length() == 0) {
        errorString += tr("No room selected for this reservation") + "<br>";
        ui->lbRoom->setStyleSheet("color:red");
        error = true;
    }
    if (ui->sbMan->value() + ui->sbWoman->value() < 1) {
        errorString += tr("Guests count cannot be equal to zero") + "<br>";
        error = true;
    }
    ui->lbStatusCode->setStyleSheet("color:black");
    if (ui->leStatusCode->asInt() == 0) {
        error = true;
        errorString += tr("Status code is not defined") + "<br>";
        ui->lbStatusCode->setStyleSheet("color:red");
    }
    ui->lbRoomPrice->setStyleSheet("color:black");
    if (ui->leRooming->text().toFloat() < 0.1 && ui->cbPaymentType->currentData().toInt() != PAYMENT_COMPLIMENTARY) {
        //message_error_tr("Price is not defined");
        //ui->lbRoomPrice->setStyleSheet("color:red");
        //return false;
    }
    bool passportFound = false;
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        CI_Guest *g = CacheGuest::instance()->get(ui->tblGuest->item(i, 0)->text());
        if (g) {
            if (g->fPassport.isEmpty()) {
                continue;
            } else {
                passportFound = true;
                break;
            }
        }
    }
    if (!passportFound && fPreferences.getDb(def_passport_required).toBool()) {
        errorString += tr("No passport record found for registered guest") + "<br>";
        error = true;
    }
    bool nationsOk = true;
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        CI_Guest *g = CacheGuest::instance()->get(ui->tblGuest->item(i, 0)->text());
        if (g) {
            if (g->fNatShort.trimmed().isEmpty() || g->fNatShort == "-") {
                nationsOk = false;
                break;
            }
        } else {
            nationsOk = false;
            break;
        }
    }
    if (!nationsOk) {
        errorString += tr("Check nations for all guests") + "<br>";
        error = true;
    }

    bool result = save();
    if (!result) {
        return false;
    }
    QDate date = WORKING_DATE;
    if (ui->deEntry->date() != date) {
        errorString += "<br>" + tr("The entry date is not equal to working date");
        error = true;
    }
    if (ui->leArrivalFee->asDouble() > 0.1) {
        if (fPreferences.getDb(def_airport_pickup_id).toInt() == 0) {
            errorString += tr("Airport pickup id not defined in the config. Contact to administrator");
            error = true;
        }
    }
    if (fPreferences.getDb(def_earyly_checkin_id).toInt() == 0) {
        errorString += tr("Early checkin id is not defined. Contact to administrator");
        error = true;
    }
    if (CacheRoom::instance()->get(ui->leRoomCode->text())->fState != ROOM_STATE_NONE) {
        errorString += tr("Room is not vacant ready");
        error = true;
    }
    if (error) {
        return false;
    }
    fDb.fDb.transaction();
    if (result) {
        fDbBind[":f_state"] = RESERVE_CHECKIN;
        fDbBind[":f_checkInDate"] = WORKING_DATE;
        fDbBind[":f_checkInTime"] = QTime::currentTime();
        fDbBind[":f_checkInUser"] = WORKING_USERID;
        result = result && fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservId->text())));
    }
    if (result) {
        fDbBind[":f_state"] = ROOM_STATE_CHECKIN;
        result = result && fDb.update("f_room", fDbBind, where_id(ui->leRoomCode->asInt()));

        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_oldState"] = ROOM_STATE_NONE;
        fDbBind[":f_newState"] = ROOM_STATE_CHECKIN;
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comment"] = "CHECKIN";
        fDbBind[":f_reservation"] = ui->leReservId->text();
        fDb.insert("f_room_state_change", fDbBind);
    }

    /*------------------------ BEGIN ADVANCE -------------------*/
    if (result) {
            fDbBind[":f_source"] = VAUCHER_ADVANCE_N;
            fDbBind[":f_res"] = ui->leReservId->text();
            fDb.select("select f_id, f_fiscal, f_amountAmd "
                       "from m_register where f_canceled=0 and f_source=:f_source "
                       "and f_rec=:f_rec", fDbBind, fDbRows);
            foreach_rows {
                int fiscal = it->at(1).toInt();
                fDbBind[":f_inv"] = ui->leInvoice->text();
                fDbBind[":f_room"] = ui->leRoomCode->text();
                result = result && fDb.update("m_register", fDbBind, where_id(ap(it->at(0).toInt())));
                if (fiscal > 0) {
                    QMap<QString, QVariant> bv;
                    QList<QList<QVariant> > br;
                    bv[":f_prepaid"] = it->at(2).toDouble();
                    bv[":f_id"] = ui->leInvoice->text();
                    fDb.select("update m_v_invoice set f_prepaid=f_prepaid+:f_prepaid where f_id=:f_id", bv, br);
                }
            }
    }
    /*------------------------ END ADVANCE ----------------------*/
    /*------------------------ BEGIN EARLY CHECKIN FEE ----------------*/
    if (result) {
        if (ui->leEarlyCheckInFee->asDouble() > 0.1) {
            CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(fPreferences.getDb(def_earyly_checkin_id).toString());
            fDbBind[":f_id"] = 0;
            int vid = fDb.insert("m_v_postcharge", fDbBind);
            fDbBind[":f_id"] = 0;
            int rid = fDb.insert("m_register", fDbBind);
            fDbBind[":f_source"] = ii->fVaucher;
            fDbBind[":f_wdate"] = WORKING_DATE;
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_room"] = ui->leRoomCode->text();
            fDbBind[":f_guest"] = ui->tblGuest->item(0, 2)->text();
            fDbBind[":f_itemCode"] = fPreferences.getDb(def_earyly_checkin_id).toInt();
            fDbBind[":f_finalName"] = ii->fName;
            fDbBind[":f_amountAmd"] = ui->leEarlyCheckInFee->asDouble();
            fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->leEarlyCheckInFee->asDouble(), ui->cbVAT->currentData().toInt());
            fDbBind[":f_amountUsd"] = def_usd;
            fDbBind[":f_fiscal"] = 0;
            fDbBind[":f_paymentMode"] = PAYMENT_CREDIT;
            fDbBind[":f_creditCard"] = 0;
            fDbBind[":f_cityLedger"] = 0;
            fDbBind[":f_paymentComment"] = "";
            fDbBind[":f_dc"] = "CREDIT";
            fDbBind[":f_sign"] = 1;
            fDbBind[":f_doc"] = vid;
            fDbBind[":f_rec"] = 0;
            fDbBind[":f_inv"] = ui->leInvoice->text();
            fDbBind[":f_finance"] = 1;
            fDbBind[":f_remarks"] = "";
            fDbBind[":f_canceled"] = 0;
            fDbBind[":f_cancelReason"] = "";
            fDbBind[":f_side"] = fItemSide.contains(fPreferences.getDb(def_earyly_checkin_id).toInt());
            result = result && fDb.update("m_register", fDbBind, where_id(ap(rid)));

            if (result) {
                fTrackControl->insert("Auto early checkin fee", QString("%1 / %2 / %3")
                             .arg(WORKING_DATE.toString(def_date_format))
                             .arg(ui->teEarlyCheckIn->time().toString(def_time_format))
                             .arg(ui->leEarlyCheckInFee->text()), "");
            }
        }
    }
    /*------------------------ END EARLY CHECKIN FEE ----------------*/
    /*------------------------ AIRPORT PICKUP -------------------------*/
    if (result) {
        if (ui->leArrivalFee->asDouble() > 0.1) {
            CI_InvoiceItem *ia = CacheInvoiceItem::instance()->get(fPreferences.getDb(def_airport_pickup_id).toString());
            fDbBind[":f_id"] = 0;
            int vid = fDb.insert("m_v_receipt", fDbBind);
            fDbBind[":f_id"] = 0;
            int rid = fDb.insert("m_register", fDbBind);
            fDbBind[":f_source"] = ia->fVaucher;
            fDbBind[":f_wdate"] = WORKING_DATE;
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
            fDbBind[":f_room"] = ui->leRoomCode->text();
            fDbBind[":f_guest"] = ui->tblGuest->item(0, 2)->text();
            fDbBind[":f_itemCode"] = ia->fCode;
            fDbBind[":f_finalName"] = ia->fName;
            fDbBind[":f_amountAmd"] = ui->leArrivalFee->asDouble();
            fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->leArrivalFee->asDouble(), ui->cbVAT->currentData().toInt());
            fDbBind[":f_amountUsd"] = def_usd;
            fDbBind[":f_fiscal"] = 0;
            fDbBind[":f_paymentMode"] = PAYMENT_CREDIT;
            fDbBind[":f_creditCard"] = 0;
            fDbBind[":f_cityLedger"] = 0;
            fDbBind[":f_paymentComment"] = "";
            fDbBind[":f_dc"] = "CREDIT";
            fDbBind[":f_sign"] = 1;
            fDbBind[":f_doc"] = vid;
            fDbBind[":f_rec"] = 0;
            fDbBind[":f_inv"] = ui->leInvoice->text();
            fDbBind[":f_finance"] = 1;
            fDbBind[":f_remarks"] = "";
            fDbBind[":f_canceled"] = 0;
            fDbBind[":f_cancelReason"] = "";
            fDbBind[":f_side"] = fItemSide.contains(fPreferences.getDb(def_earyly_checkin_id).toInt());
            result = result && fDb.update("m_register", fDbBind, where_id(ap(rid)));
            if (result) {
                fTrackControl->insert("Auto airport pickup fee", QString("%1 / %2 ")
                             .arg(WORKING_DATE.toString(def_date_format))
                             .arg(ui->leArrivalFee->text()), "");
            }
        }
    }
    /*------------------------ END AIRPORT PICKUP -------------------------*/
    if (result) {
        fDb.fDb.commit();
        ui->leReserveCode->setInt(RESERVE_CHECKIN);
        ui->leReserveName->setText(CACHERESERVESTATE_NAME(RESERVE_CHECKIN));
        ui->leStatusCode->setInt(2);
        ui->leStatusName->setText(CacheReserveStatus::instance()->get(2)->fName);
        ui->deEntry->setEnabled(false);
        fTrackControl->saveChanges();
        BroadcastThread::cmdRefreshCache(cid_red_reservation, ui->leReservId->text());
        BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
        BroadcastThread::cmdRefreshCache(cid_room, ui->leRoomCode->text());
        BroadcastThread::cmdRefreshCache(cid_active_room, ui->leRoomCode->text());
        PPrintCheckin::print(ui->leReservId->text());
    } else {
        fDb.fDb.rollback();
    }
    return result;
}

bool WReservationRoomTab::canCancel()
{
    bool result = true;
    result = result && ui->leReservId->notEmpty();
    result = result && ui->leReserveCode->asInt() == RESERVE_RESERVE;
    return result;
}

bool WReservationRoomTab::cancelReservation(bool confirm)
{
    if (ui->leReserveCode->asInt() == RESERVE_REMOVED) {
        return true;
    }
    int cr = DlgCheckAdvanceBeforeCancel::checkAdvance(ui->leInvoice->text());
    getAdvance();
    switch (cr) {
    case CR_NOCANCEL:
    case 0:
        getAdvance();
        return false;
    case CR_CANCEL:
        break;
    }

    int prevreserve = ui->leReserveCode->asInt();
    bool result = true;
    fDb.fDb.transaction();
    fDbBind[":f_state"] = RESERVE_REMOVED;
    fDbBind[":f_cancelUser"] = WORKING_USERID;
    fDbBind[":f_cancelDate"] = QDateTime::currentDateTime();
    ui->leReserveCode->setInt(RESERVE_REMOVED);
    ui->leReserveName->setText(CacheReserveState::instance()->get(RESERVE_REMOVED)->fName);
    result = result && fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservId->text())));

    if (result) {
        fTrackControl->insert("Reservation was canceled", "", "");
        fTrackControl->saveChanges();
        fDb.fDb.commit();
        BroadcastThread::cmdRefreshCache(cid_red_reservation, ui->leReservId->text());
        BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
        if (confirm) {
             message_info_tr("Reservation was canceled");
        }

    } else {
        fDb.fDb.rollback();
        ui->leReserveCode->setInt(prevreserve);
        ui->leReserveName->setText(CacheReserveState::instance()->get(prevreserve)->fName);
        if (confirm) {
            message_info_tr("Reservation was not canceled");
        }
    }
    emit commonChanges();
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
    return result;
}

bool WReservationRoomTab::canCopyLast()
{
    return ui->leReservId->isEmpty();
}

bool WReservationRoomTab::canRevive()
{
    QString errors;
    if (ui->deEntry->date() < WORKING_DATE) {
        errors += tr("This reservation cannot revive. Make another one") + "<br>";
    }
    if (!errors.isEmpty()) {
        message_error(errors);
    }
    return errors.isEmpty();
}

bool WReservationRoomTab::revive()
{
    ui->leReserveCode->setInt(RESERVE_RESERVE);
    ui->leReserveName->setText(CacheReserveState::instance()->get(RESERVE_RESERVE)->fName);
    fDbBind[":f_state"] = RESERVE_RESERVE;
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservId->text())));
    fTrackControl->insert("Revive", "", "");
    loadReservation(ui->leReservId->text());
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
    DatabaseResult dr;
    fDbBind[":f_id"] = ui->leReservId->text();
    dr.select(fDb, "select f_group from f_reservation where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        fReservation->setGroup(dr.value("f_group").toInt());
    }
    emit commonChanges();
    return true;
}

void WReservationRoomTab::copyLast(const QString &lastId)
{
    if (ui->leReservId->notEmpty()) {
        return;
    }
    QString last;
    if (!lastId.isEmpty()) {
        last = lastId;
    } else {
        fDb.select("select f_id from f_reservation_last", fDbBind, fDbRows);
        last = fDbRows.at(0).at(0).toString();
        if (last.isEmpty()) {
            message_info_tr("There are no last reservation");
            return;
        }
    }
    loadReservation(last);
    ui->leReservId->clear();
    ui->leInvoice->clear();
    fTrackControl->resetChanges();
    checkDatesCross();
}

QString WReservationRoomTab::author()
{
    return fAuthor;
}

QString WReservationRoomTab::lastModify()
{
    return fLastModify;
}

int WReservationRoomTab::reserveState()
{
    return ui->leReserveCode->asInt();
}

QString WReservationRoomTab::reserveId()
{
    return ui->leReservId->text();
}

void WReservationRoomTab::startTrackControl()
{
    if (fTrackControl) {
        return;
    }
    fTrackControl = new TrackControl(TRACK_RESERVATION);
    fTrackControl->fInvoice = ui->leInvoice->text();
    fTrackControl->fReservation = ui->leReservId->text();
    fTrackControl->addWidget(ui->leReservId, "Doc number")
            .addWidget(ui->deCreated, "Created")
            .addWidget(ui->leReserveName, "Reserve state")
            .addWidget(ui->leStatusName, "Status")
            .addWidget(ui->deEntry, "Entry date")
            .addWidget(ui->deDeparture, "Departure date")
            .addWidget(ui->sbNights, "Nights")
            .addWidget(ui->leInvoice, "Invoice")
            .addWidget(ui->leRoomCode, "Room")
            .addWidget(ui->cbArrangment, "Arrangement")
            .addWidget(ui->chMealIncluded, "Meal included")
            .addWidget(ui->sbMan, "Male")
            .addWidget(ui->sbWoman, "Female")
            .addWidget(ui->sbChild, "Childs")
            .addWidget(ui->sbBaby, "Babies")
            .addWidget(ui->chEarlyCheckIn, "Early checkin")
            .addWidget(ui->teEarlyCheckIn, "Early checkin time")
            .addWidget(ui->chLateCheckout, "Late checkout")
            .addWidget(ui->teLateChecout, "Late checkout time")
            .addWidget(ui->leLateCheckoutFee, "Late checkout fee")
            .addWidget(ui->teArrivalTime, "Arrival time")
            .addWidget(ui->teDepartureTime, "Departure time")
            .addWidget(ui->leArrivalFee, "Pickup fee")
            .addWidget(ui->leCardexCode, "Cardex")
            .addWidget(ui->leCardexname, "Cardex name")
            .addWidget(ui->leBooking, "Booking")
            .addWidget(ui->leCityLedgerName, "City ledger")
            .addWidget(ui->leRooming, "Rate")
            .addWidget(ui->chExtraBed, "Extra bed")
            .addWidget(ui->leExtraBedAmount, "Extra bed fee")
            .addWidget(ui->sbMealQty, "Meal")
            .addWidget(ui->leMealPrice, "Meal price")
            .addWidget(ui->lePricePerNight, "Price per night")
            .addWidget(ui->cbVAT, "VAT")
            .addWidget(ui->leNoVATReason, "No VAR reason")
            .addWidget(ui->leTotal, "Total")
            .addWidget(ui->leVAT, "VAT amount")
            .addWidget(ui->leGrandTotal, "Grand total")
            .addWidget(ui->cbPaymentType, "Payment mode")
            .addWidget(ui->teRoomRemarks, "Remarks")
            .addWidget(ui->chPickup, "Need pickup");
    ui->leStatusCode->setFocus();
}

TrackControl *WReservationRoomTab::trackControl()
{
    return fTrackControl;
}

EQComboBox *WReservationRoomTab::cbRoomArrangement()
{
    return ui->cbArrangment;
}

void WReservationRoomTab::addRemoveFromGroup()
{
    DlgGroupReserveOption::addRemove(ui->leReservId->text());
    DatabaseResult dr;
    fDbBind[":f_id"] = ui->leReservId->text();
    dr.select(fDb, "select f_group from f_reservation where f_id=:f_id", fDbBind);
    if (dr.rowCount() > 0) {
        fReservation->setGroup(dr.value("f_group").toInt());
    }
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReservId->text());
    BroadcastThread::cmdRefreshCache(cid_red_reservation, ui->leReservId->text());
}

void WReservationRoomTab::sendConfirmation()
{
    DlgEmail::sendDialog();
}

void WReservationRoomTab::countTotal()
{
    double total = ui->leRooming->asDouble();
    total += ui->leExtraBedAmount->asDouble();
    total += (ui->sbMealQty->value() * ui->leMealPrice->asDouble());

    ui->lePricePerNight->setDouble(total);
    total *= ui->sbNights->value();
    ui->leTotal->setText(QString::number(total, 'f', 2));
    switch (ui->cbVAT->asInt()) {
    case VAT_INCLUDED: {
        double vat = total - (total / ((def_vat / 100) + 1));
        ui->leVAT->setDouble(vat);
        ui->leGrandTotal->setText(ui->leTotal->text());
        break;
    }
    case VAT_WITHOUT: {
        double vat = total * (def_vat / 100);
        ui->leVAT->setDouble(vat);
        ui->leGrandTotal->setDouble(ui->leTotal->asDouble() + vat);
        break;
    }
    case VAT_NOVAT:
        ui->leVAT->setText("0.00");
        ui->leGrandTotal->setText(ui->leTotal->text());
        break;
    }
    ui->leGrandTotal->addDouble(ui->leEarlyCheckInFee->asDouble());
    ui->leGrandTotal->addDouble(ui->leArrivalFee->asDouble());
    ui->leTotalUSD->setText(QString::number(total / def_usd, 'f', 2));
}

void WReservationRoomTab::setEarlyCheckIn(bool v)
{
    ui->teEarlyCheckIn->setEnabled(v);
    ui->leEarlyCheckInFee->setEnabled(v);
}

void WReservationRoomTab::checkDatesCross()
{
    if (ui->deCreated->date() == ui->deEntry->date()) {
        ui->lbWolking->setEnabled(true);
    } else {
        ui->lbWolking->setEnabled(false);
    }
    if (ui->deEntry->date() == ui->deDeparture->date()) {
        ui->lbDayUse->setEnabled(true);
    } else {
        ui->lbDayUse->setEnabled(false);
    }
    ui->lbEarlyCheckinWarning->setPixmap(QPixmap(":/images/ok.png"));
    ui->lbEndDateWarning->setPixmap(QPixmap(":/images/ok.png"));
    ui->lbStartDateWarning->setPixmap(QPixmap(":/images/ok.png"));
    ui->lbStartDateWarning->setToolTip("");
    ui->lbEndDateWarning->setToolTip("");
    fStartDateOk = true;
    fEndDateOk = true;
    QString tt1, tt2;
    if (ui->leRoomCode->asInt() > 0) {
        QMap<QString, CI_Reservation*> r;
        CacheReservation::instance()->check(ui->deEntry->date(), ui->deDeparture->date(), ui->leRoomCode->asInt(), r, fStartDateOk, fEndDateOk, ui->leReservId->text());
        if (r.count() == 0) {

        } else {
            QMapIterator<QString, CI_Reservation*> ri(r);
            while (ri.hasNext()) {
                CI_Reservation *rm = ri.next().value();
                if (rm->fDateEnd > ui->deEntry->date()) {
                    tt1 += QString("%1 #%2/%3. %4 - %5\n")
                            .arg(tr("Room"))
                            .arg(rm->fId)
                            .arg(ui->leRoomCode->asInt())
                            .arg(rm->fDateStart.toString(def_date_format))
                            .arg(rm->fDateEnd.toString(def_date_format));
                }
                if (rm->fDateStart < ui->deDeparture->date()) {
                    tt2 += QString("%1 #%2/%3. %4 - %5\n")
                            .arg(tr("Room"))
                            .arg(rm->fId)
                            .arg(ui->leRoomCode->asInt())
                            .arg(rm->fDateStart.toString(def_date_format))
                            .arg(rm->fDateEnd.toString(def_date_format));
                }
            }
        }
    }
    if (ui->deEntry->date() > ui->deDeparture->date()) {
        fStartDateOk = false;
        fEndDateOk = false;
    }
    if (ui->deEntry->date() < WORKING_DATE && ui->leReserveCode->asInt() == RESERVE_RESERVE) {
        fStartDateOk = false;
    }
    if (ui->leReserveCode->asInt() == RESERVE_CHECKIN) {
        if (ui->deDeparture->date() < WORKING_DATE) {
            fEndDateOk = false;
        }
    }
    if (!fStartDateOk) {
        ui->lbStartDateWarning->setPixmap(QPixmap(":/images/warning.png"));
    }
    if (!fEndDateOk) {
        ui->lbEndDateWarning->setPixmap(QPixmap(":/images/warning.png"));
    }
    ui->lbStartDateWarning->setToolTip(tt1);
    ui->lbEndDateWarning->setToolTip(tt2);
}

void WReservationRoomTab::setGroupBoxesEnabled(bool v)
{
    ui->grCommon->setEnabled(v);
    ui->grRoom->setEnabled(v);
    ui->grAdd->setEnabled(v);
    ui->grPricing->setEnabled(v);
    ui->grPayment->setEnabled(v);
    ui->grRemarks->setEnabled(v);
}

void WReservationRoomTab::saveVaucher()
{
    fDb.fDb.transaction();
    QString rid = ui->leReservId->text();
    fDbBind[":f_source"] = VAUCHER_RESERVATION_N;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_room"] = ui->leRoomCode->text();
    fDbBind[":f_guest"] = ui->tblGuest->item(0, 2)->text();
    fDbBind[":f_itemCode"] = fPreferences.getDb(def_reservation_voucher_id);
    fDbBind[":f_finalName"] = tr("RESERVATION ") + ui->leReservId->text();
    fDbBind[":f_amountAmd"] = ui->leGrandTotal->asDouble();
    fDbBind[":f_amountVat"] = ui->leVAT->asDouble();
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->cbPaymentType->currentData();
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = ui->leCityLedgerCode->asInt();
    fDbBind[":f_paymentComment"] = vaucherPaymentName(ui->cbPaymentType->currentData().toInt(), 0, ui->leCityLedgerCode->text());
    fDbBind[":f_dc"] = "";
    fDbBind[":f_sign"] = 1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 0;
    fDbBind[":f_remarks"] = ui->teRoomRemarks->toPlainText();
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(rid)));
    fDb.fDb.commit();
}

void WReservationRoomTab::getAdvance()
{
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDb.select("select sum(f_amountAmd*f_sign*-1) from m_register "
               "where f_inv=:f_invoice and f_finance=1 and f_canceled=0", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leAdvance->setText(fDbRows.at(0).at(0).toString());
        if (ui->leAdvance->asDouble() < 0.01) {
            ui->leAdvance->setInt(0);
        }
    }
}

int WReservationRoomTab::firstGuestId()
{
    if (ui->tblGuest->rowCount() == 0)
        return 0;
    return ui->tblGuest->item(0, 0)->data(Qt::EditRole).toInt();
}

QString WReservationRoomTab::mainGuest()
{
    CI_Guest *g = CacheGuest::instance()->get(firstGuestId());
    if (g) {
        return g->fName;
    } else {
        return "No guest";
    }
}

bool WReservationRoomTab::checkDoc(QStringList &errors)
{
    QStringList temp;
    if (temp.count() > 0) {
        temp.insert(0, tr("Room page") + " #" + QString::number(fTabIndex + 1));
        temp.insert(0, "");
    }
    if (ui->tblGuest->rowCount() == 0) {
        temp.append(tr("Guest count must be greater than 0"));
    }
    ui->lbEntryDate->setStyleSheet("color:black");
    if (!fStartDateOk) {
        temp.append(tr("Incorrect entry date"));
        ui->lbEntryDate->setStyleSheet("color:red");
    }
    ui->lbDepartureDate->setStyleSheet("color:black");
    if (!fEndDateOk) {
        temp.append(tr("Incorrect departure date"));
        ui->lbDepartureDate->setStyleSheet("color:red");
    }
    ui->lbCardex->setStyleSheet("color:black");
    if (!fCardexOk) {
        temp.append(tr("Incorrect cardex code"));
        ui->lbCardex->setStyleSheet("color:red");
    }
    if (fCardexOk && !ui->leCardexCode->text().isEmpty()) {
        CI_Cardex *cardex = CacheCardex::instance()->get(ui->leCardexCode->text());
        if (cardex->fBookingRequired && ui->leBooking->text().isEmpty()) {
            temp.append(tr("Booking field is required"));
        }
    }
    if (!fCityLedgerOk) {
        temp.append(tr("Incorrect city ledger code"));
    }
    if (ui->leStatusCode->asInt() == CONFIRM_BLOCK) {
        temp.append(tr("Reservation status cannot be 'Block'"));
    }
    ui->lbArrangement->setStyleSheet("color:black");
    if (ui->cbArrangment->currentIndex() < 0) {
        temp.append(tr("Arrangement not correct"));
        ui->lbArrangement->setStyleSheet("color:red");
    }
    ui->lbVATMode->setStyleSheet("color:black");
    if (ui->cbVAT->currentIndex() < 0) {
        temp.append(tr("VAT mode is not selected"));
        ui->lbVATMode->setStyleSheet("color:red");
    }
    ui->lbPaymentMode->setStyleSheet("color:black");
    if (ui->cbPaymentType->currentIndex() < 0) {
        temp.append("Payment mode is not selected");
        ui->lbPaymentMode->setStyleSheet("color:red");
    }
    ui->lbCityLedger->setStyleSheet("color:black");
    if (ui->cbPaymentType->asInt() == PAYMENT_CL && ui->leCityLedgerCode->asInt() == 0) {
        temp.append(tr("City ledger is not selected"));
        ui->lbCityLedger->setStyleSheet("color:red");
    }

    errors.append(temp);
    return temp.count() == 0;
}

void WReservationRoomTab::setRoom(CI_Room *r)
{
    if (!r) {
        return;
    }
    ui->leRoomCode->setText(r->fCode);
    ui->leRoomName->setText(r->fName);
    if (ui->leReserveCode->asInt() == RESERVE_SERVICE) {
        ui->leRooming->setText(r->fPrice);
    }
    ui->lbRoom->setPixmap(QPixmap(":/images/ok.png"));
    checkDatesCross();
    emit roomChanged(r->fName, fTabIndex);
}

void WReservationRoomTab::disconnectSignals()
{
    disconnect(ui->sbMan, SIGNAL(valueChanged(int)), this, SLOT(on_sbMan_valueChanged(int)));
    disconnect(ui->sbWoman, SIGNAL(valueChanged(int)), this, SLOT(on_sbWoman_valueChanged(int)));
    disconnect(ui->sbChild, SIGNAL(valueChanged(int)), this, SLOT(on_sbChild_valueChanged(int)));
    disconnect(ui->leMealPrice, SIGNAL(textChanged(QString)), this, SLOT(on_leMealPrice_textChanged(QString)));
    disconnect(ui->leRooming, SIGNAL(textChanged(QString)), this, SLOT(on_leRooming_textChanged(QString)));
    disconnect(ui->sbNights, SIGNAL(valueChanged(int)), this, SLOT(on_sbNights_valueChanged(int)));
    disconnect(ui->deEntry, SIGNAL(dateChanged(QDate)), this, SLOT(on_deEntry_dateChanged(QDate)));
    disconnect(ui->deDeparture, SIGNAL(dateChanged(QDate)), this, SLOT(on_deDeparture_dateChanged(QDate)));
}

void WReservationRoomTab::connectSignals()
{
    connect(ui->sbMan, SIGNAL(valueChanged(int)), this, SLOT(on_sbMan_valueChanged(int)));
    connect(ui->sbWoman, SIGNAL(valueChanged(int)), this, SLOT(on_sbWoman_valueChanged(int)));
    connect(ui->sbChild, SIGNAL(valueChanged(int)), this, SLOT(on_sbChild_valueChanged(int)));
    connect(ui->leMealPrice, SIGNAL(textChanged(QString)), this, SLOT(on_leMealPrice_textChanged(QString)));
    connect(ui->leRooming, SIGNAL(textChanged(QString)), this, SLOT(on_leRooming_textChanged(QString)));
    connect(ui->sbNights, SIGNAL(valueChanged(int)), this, SLOT(on_sbNights_valueChanged(int)));
    connect(ui->deEntry, SIGNAL(dateChanged(QDate)), this, SLOT(on_deEntry_dateChanged(QDate)));
    connect(ui->deDeparture, SIGNAL(dateChanged(QDate)), this, SLOT(on_deDeparture_dateChanged(QDate)));
}

void WReservationRoomTab::setupTab()
{
    setupTabTextAndIcon(tr("Room"), ":/images/room-key.png");
}

bool WReservationRoomTab::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        switch (ke->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            focusNextChild();
            return true;
        }
    }
    return QWidget::event(event);
}

void WReservationRoomTab::roomCacheUpdated(const QString &id)
{
    if (fTabWidget->currentWidget() == this) {
        if (id == ui->leRoomCode->text()) {
            emit commonChanges();
        }
    }
}

void WReservationRoomTab::roomSearch(bool v)
{
    Q_UNUSED(v)
    DlgCreateGroupReservation *d = new DlgCreateGroupReservation(this);
    d->setSingleMode(true);
    d->setReservationTab(this);
    d->exec();
    delete d;
}

void WReservationRoomTab::tblGuestChange(int tag)
{
    tag = -1;
    EPushButton *btn = static_cast<EPushButton*>(sender());
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        if (ui->tblGuest->cellWidget(i, btn->fColumn) == btn) {
            tag = i;
        }
    }
    if (tag < 0) {
        return;
    }
    QList<QVariant> v;
    WGuest *g = WGuest::guest(v);
    g->setValues();
    if (g->exec() == QDialog::Accepted) {
        fTrackControl->insertMessage("Guest changed", ui->tblGuest->toString(tag, 2), v.at(2).toString() + " " + v.at(3).toString());
        ui->tblGuest->item(tag, 1)->setText(v.at(1).toString());
        ui->tblGuest->item(tag, 2)->setText(v.at(2).toString() + " " + v.at(3).toString());
    }
    delete g;
}

void WReservationRoomTab::tblGuestRemove(int row)
{
    if (message_confirm_tr("Are you sure to delete this guest") != QDialog::Accepted) {
        return;
    }
    row = -1;
    EPushButton *btn = static_cast<EPushButton*>(sender());
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        if (ui->tblGuest->cellWidget(i, btn->fColumn) == btn) {
            row = i;
            break;
        }
    }
    if (row < 0) {
        return;
    }
    EPushButton *btn1 = static_cast<EPushButton*>(ui->tblGuest->cellWidget(row, 4));
    EPushButton *btn2 = static_cast<EPushButton*>(ui->tblGuest->cellWidget(row, 3));
    fTrackControl->insertMessage("Guest removed", ui->tblGuest->item(row, 1)->text() + " " + ui->tblGuest->item(row, 2)->text(), "");
    ui->tblGuest->removeCellWidget(row, 3);
    ui->tblGuest->removeCellWidget(row, 4);
    ui->tblGuest->removeRow(row);
    btn1->deleteLater();
    btn2->deleteLater();
}

void WReservationRoomTab::tblGuestChangeInfo(int tag)
{
    tag = -1;
    EPushButton *btn = static_cast<EPushButton*>(sender());
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        if (ui->tblGuest->cellWidget(i, btn->fColumn) == btn) {
            tag = i;
        }
    }
    if (tag < 0) {
        return;
    }
    fDbBind[":f_id"] = ui->tblGuest->toInt(tag, 0);
    fDb.select("select g.f_id, g.f_title, g.f_firstName, g.f_lastName, g.f_sex, s.f_" + def_lang + ", g.f_dateBirth, "
               "g.f_placeBirth, "
               "n.f_short, n.f_name as nationality, g.f_passport, g.f_address, g.f_tel1, g.f_tel2, "
               "g.f_email, g.f_postBox, g.f_remarks "
               "from f_guests g "
               "left join f_nationality n on g.f_nation=n.f_short "
               "left join f_guests_sex s on s.f_id=g.f_sex "
               "where g.f_id=:f_id ", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Cannot get guest info");
        return;
    }
    QList<QVariant> v = fDbRows.at(0);
    WGuest *g = WGuest::guest(v);
    g->setValues();
    if (g->exec() == QDialog::Accepted) {
        fTrackControl->insert("Guest info edited", ui->tblGuest->toString(tag, 2), v.at(2).toString() + " " + v.at(3).toString());
        ui->tblGuest->item(tag, 1)->setText(v.at(1).toString());
        ui->tblGuest->item(tag, 2)->setText(v.at(2).toString() + " " + v.at(3).toString());
    }
    delete g;
}

void WReservationRoomTab::guest(CI_Guest *c)
{
    if (c) {
        ui->leSearchGuest->clear();
        addGuest(c, true);
    }
}

void WReservationRoomTab::room(CI_Room *c)
{
    if (!c) {
        ui->lbRoom->setPixmap(QPixmap(":/images/question.png"));
        ui->leRoomName->clear();
        if (ui->leRoomCode->asInt() > 0) {
            c = CacheRoom::instance()->get(ui->leRoomCode->text());
            if (!c) {
                ui->leRoomCode->clear();
                ui->lbRoom->setPixmap(QPixmap(":/images/warning.png"));
            }
        }
    }
    if (c) {
        ui->leRoomCode->setText(c->fCode);
        ui->leRoomName->setText(c->fName);
        if (ui->leReserveCode->asInt() == RESERVE_SERVICE) {
            ui->leRooming->setText(c->fPrice);
        }
        ui->lbRoom->setPixmap(QPixmap(":/images/ok.png"));
        checkDatesCross();
    }
}

void WReservationRoomTab::cardex(CI_Cardex *c)
{
    if (!c) {
        if (ui->leCardexCode->text().length() == 0) {
            ui->leCardexname->clear();
            ui->lbCardexError->setPixmap(QPixmap(":/images/warning.png"));
            fCardexOk = false;
            return;
        } else {
            c = CacheCardex::instance()->get(ui->leCardexCode->text());
            if (!c) {
                ui->leCardexname->clear();
                ui->lbCardexError->setPixmap(QPixmap(":/images/warning.png"));
                ui->leCommission->setText("0.00");
                fCardexOk = false;
                return;
            }
        }
    }
    ui->leCardexCode->setText(c->fCode);
    ui->leCardexname->setText(c->fName);
    fCardexOk = true;
    ui->lbCardexError->setPixmap(QPixmap(":/images/ok.png"));
    ui->leCommission->setText(c->fCommission);
    ui->cbVAT->setIndexForData(c->fVatMode.toInt());
}

void WReservationRoomTab::cityLedger(CI_CityLedger *c)
{
    if (!c) {
        if (ui->leCityLedgerCode->asInt() == 0) {
            fCityLedgerOk = true;
            ui->lbCLedgerError->setPixmap(QPixmap(":/images/question.png"));
            ui->leCityLedgerName->clear();
            return;
        } else {
            c = CacheCityLedger::instance()->get(ui->leCityLedgerCode->text());
            if (!c) {
                ui->leCityLedgerName->clear();
                fCityLedgerOk = false;
                ui->lbCLedgerError->setPixmap(QPixmap(":/images/warning.png"));
                return;
            }
        }
    }
    ui->leCityLedgerCode->setText(c->fCode);
    ui->leCityLedgerName->setText(c->fName);
    ui->lbCLedgerError->setPixmap(QPixmap(":/images/ok.png"));
    fCityLedgerOk = true;
}

void WReservationRoomTab::reserveStatus(CI_ReserveStatus *c)
{
    if (c) {
        ui->leStatusCode->setText(c->fCode);
        ui->leStatusName->setText(c->fName);
    } else {
        c = CacheReserveStatus::instance()->get(ui->leStatusCode->text());
        if (c) {
            ui->leStatusCode->setText(c->fCode);
            ui->leStatusName->setText(c->fName);
        } else {
            ui->leStatusCode->clear();
            ui->leStatusName->clear();
        }
    }
}

void WReservationRoomTab::on_leRoomCode_returnPressed()
{
    ui->leRoomName->clear();
    if (ui->leRoomCode->asInt() == 0) {
        ui->leRoomCode->clear();
        ui->lbRoom->setPixmap(QPixmap(":/images/question.png"));
        return;
    }
    CI_Room *r = CacheRoom::room(ui->leRoomCode->text());
    if (!r) {
        ui->lbRoom->setPixmap(QPixmap(":/images/warning.png"));
        return;
    }
    ui->lbRoom->setPixmap(QPixmap(":/images/ok.png"));
    ui->leRoomName->setText(r->fName);
    if (ui->leReserveCode->asInt() == RESERVE_SERVICE) {
        ui->leRooming->setText(r->fPrice);
    }
    emit roomChanged(r->fName, fTabIndex);
    checkDatesCross();
}

void WReservationRoomTab::addGuest(CI_Guest *g, bool log)
{
    if (!g) {
        return;
    }
    int row = ui->tblGuest->rowCount();
    ui->tblGuest->setRowCount(ui->tblGuest->rowCount() + 1);
    QTableWidgetItem *iCode = new QTableWidgetItem();
    iCode->setData(Qt::DisplayRole, g->fCode);
    QTableWidgetItem *iTitle = new QTableWidgetItem();
    iTitle->setData(Qt::DisplayRole, g->fTitle);
    QTableWidgetItem *iName = new QTableWidgetItem();
    iName->setData(Qt::DisplayRole, g->fName);
    QTableWidgetItem *iInfo = new QTableWidgetItem();
    QTableWidgetItem *iRemove = new QTableWidgetItem();
    ui->tblGuest->setItem(row, 0, iCode);
    ui->tblGuest->setItem(row, 1, iTitle);
    ui->tblGuest->setItem(row, 2, iName);
    ui->tblGuest->setItem(row, 3, iInfo);
    ui->tblGuest->setItem(row, 4, iRemove);
    on_leMealPrice_textChanged("");
    EPushButton *btnInfo = new EPushButton(this);
    btnInfo->setText("");
    btnInfo->setToolTip(tr("Change guest"));
    btnInfo->setTag(row);
    btnInfo->fColumn = 3;
    btnInfo->setIcon(QIcon(":/images/change.png"));
    EPushButton *btnRemove = new EPushButton(this);
    btnRemove->setText("");
    btnRemove->setToolTip(tr("Remove guest"));
    btnRemove->setIcon(QIcon(":/images/garbage.png"));
    btnRemove->setTag(row);
    btnRemove->fColumn = 4;
    EPushButton *btnChangeInfo = new EPushButton(this);
    btnChangeInfo->setText("");
    btnChangeInfo->setToolTip(tr("Modify guest info"));
    btnChangeInfo->setIcon(QIcon(":/images/info.png"));
    btnChangeInfo->setTag(row);
    btnChangeInfo->fColumn = 5;
    ui->tblGuest->setCellWidget(row, 3, btnInfo);
    ui->tblGuest->setCellWidget(row, 4, btnRemove);
    ui->tblGuest->setCellWidget(row, 5, btnChangeInfo);
    connect(btnInfo, SIGNAL(clickedWithTag(int)), this, SLOT(tblGuestChange(int)));
    connect(btnRemove, SIGNAL(clickedWithTag(int)), this, SLOT(tblGuestRemove(int)));
    connect(btnChangeInfo, SIGNAL(clickedWithTag(int)), this, SLOT(tblGuestChangeInfo(int)));
    countTotal();
    ui->leSearchGuest->clear();
    if (log) {
        fTrackControl->fTable = TRACK_GUEST;
        fTrackControl->fRecord = iCode->text();
        fTrackControl->insert("New guest", "", iTitle->text() + " " + iName->text());
        fTrackControl->fTable = TRACK_RESERVATION;
        fTrackControl->insertMessage("New guest", "", iTitle->text() + " " + iName->text());
        fTrackControl->fRecord = "-";
    }
}

void WReservationRoomTab::on_leMealPrice_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countTotal();
}

void WReservationRoomTab::on_leRooming_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countTotal();
}

void WReservationRoomTab::on_sbNights_valueChanged(int arg1)
{
    disconnectSignals();
    ui->deDeparture->setDate(ui->deEntry->date().addDays(arg1));
    connectSignals();
    countTotal();
    checkDatesCross();
}

void WReservationRoomTab::on_deDeparture_dateChanged(const QDate &date)
{
    disconnectSignals();
    ui->sbNights->setValue(ui->deEntry->date().daysTo(date));
    connectSignals();
    countTotal();
    checkDatesCross();
    if (date == ui->deEntry->date()) {
        ui->lbDayUse->setEnabled(true);
    } else {
        ui->lbDayUse->setEnabled(false);
    }
}

void WReservationRoomTab::on_deEntry_dateChanged(const QDate &date)
{
    disconnectSignals();
    ui->sbNights->setValue(date.daysTo(ui->deDeparture->date()));
    connectSignals();
    countTotal();
    checkDatesCross();
}

void WReservationRoomTab::on_cbVAT_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    countTotal();
    ui->leNoVATReason->setEnabled(ui->cbVAT->asInt() == VAT_NOVAT);
}

void WReservationRoomTab::on_leExtraBedAmount_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countTotal();
}

void WReservationRoomTab::on_leEarlyCheckInFee_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countTotal();
}

void WReservationRoomTab::on_chEarlyCheckIn_stateChanged(int arg1)
{
    setEarlyCheckIn(arg1);
}

void WReservationRoomTab::on_chExtraBed_stateChanged(int arg1)
{
    ui->leExtraBedAmount->setEnabled(arg1);
}

void WReservationRoomTab::on_sbWoman_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    countTotal();
    on_cbArrangment_currentIndexChanged(ui->cbArrangment->currentIndex());
}

void WReservationRoomTab::on_sbChild_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    countTotal();
    on_cbArrangment_currentIndexChanged(ui->cbArrangment->currentIndex());
}

void WReservationRoomTab::on_sbMan_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    countTotal();
    on_cbArrangment_currentIndexChanged(ui->cbArrangment->currentIndex());
}

void WReservationRoomTab::on_chEarlyCheckIn_clicked(bool checked)
{
    if (checked) {
        ui->lbEarlyCheckinWarning->setEnabled(true);
        checkDatesCross();
    } else {
        ui->lbEarlyCheckinWarning->setPixmap(QPixmap(":/images/ok.png"));
        ui->lbEarlyCheckinWarning->setEnabled(false);
        ui->leEarlyCheckInFee->setText("");
        countTotal();
    }
}

void WReservationRoomTab::on_chExtraBed_clicked(bool checked)
{
    if (checked) {
        ui->leExtraBedAmount->setText(fPreferences.getLocalString(def_default_extra_bed));
    } else {
        ui->leExtraBedAmount->setText("");
    }
    countTotal();
}

void WReservationRoomTab::on_btnNewGuest_clicked()
{
    QList<QVariant> values;
    WGuest *g = WGuest::guest(values, true);
    if (g->exec() == QDialog::Accepted) {
        CI_Guest *guest = CacheGuest::instance()->get(values.at(0).toString());
        int wait = 500000;
        while (guest == 0 && wait > 0) {
            qApp->processEvents();
            guest = CacheGuest::instance()->get(values.at(0).toString());
            wait--;
        }
        if (!guest) {
            message_error_tr("Cannot register new guest, contact to application developer");
            return;
        }
        addGuest(guest, true);
    }
    delete g;
}

void WReservationRoomTab::on_cbPaymentType_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    int pm = ui->cbPaymentType->asInt();
    ui->leCityLedgerCode->setEnabled(pm == PAYMENT_CL);
    ui->leCityLedgerName->setEnabled(pm == PAYMENT_CL);
}

void WReservationRoomTab::on_btnItemSide_clicked()
{
    fDockItemSide->show();
    fDockItemSide->raise();
}

void WReservationRoomTab::on_leSearchGuest_returnPressed()
{
    if (ui->leSearchGuest->text().trimmed().isEmpty()) {
        return;
    }
    QStringList fn = ui->leSearchGuest->text().split(" ");
    QString ln;
    if (fn.count() > 0) {
        QString searchName = fn.at(0).toLower();
        if (fn.count() > 1) {
            searchName += " " + fn.at(1).toLower();
        }
        fDb.select("select f_id from f_guests where concat(lower(f_firstName), ' ', lower(f_lastName)) like '" + searchName + "%'", fDbBind, fDbRows);
        if (fDbRows.count()  > 0) {
            fDockGuest->setFilterValue(ui->leSearchGuest->text());
            fDockGuest->show();
            fDockGuest->activateWindow();
            fDockGuest->raise();
        } else {
            fDbBind[":f_title"] = "";
            fDbBind[":f_firstName"] = fn.at(0);
            if (fn.count() > 1) {
                QString lastName;
                bool first = true;
                for (int i = 1; i < fn.count(); i++) {
                    if (first) {
                        first = false;
                    } else {
                        lastName += " ";
                    }
                    lastName += fn.at(i);
                }
                fDbBind[":f_lastName"] = lastName;
                ln = lastName;
            } else {
                fDbBind[":f_lastName"] = "";
            }
            int gid = fDb.insert("f_guests", fDbBind);
            CI_Guest g;
            g.fCode = QString::number(gid);
            g.fName = fn.at(0) + " ";
            if (fn.count() > 1) {
                g.fName += ln;
            }
            BroadcastThread::cmdRefreshCache(cid_guest, g.fCode);
            addGuest(&g, true);
        }
    }
}

void WReservationRoomTab::on_chMealIncluded_clicked(bool checked)
{
    ui->leMealPrice->setEnabled(!checked);
    if (checked) {
        ui->leMealPrice->setText(0);
    }
    countTotal();
}

void WReservationRoomTab::on_btnAppendAdvance_clicked()
{
    if (ui->leReservId->isEmpty()) {
        message_error_tr("Save reservation first");
        return;
    }
    CI_RedReservation *rr = CacheRedReservation::instance()->get(ui->leReservId->text());
    DlgAdvanceEntry::advance(rr);
    getAdvance();
}

void WReservationRoomTab::on_chLateCheckout_clicked(bool checked)
{
    ui->leLateCheckoutFee->setEnabled(checked);
    ui->teLateChecout->setEnabled(checked);
    if (!checked) {
        ui->leLateCheckoutFee->clear();
    }
}

void WReservationRoomTab::on_sbMealQty_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
}

void WReservationRoomTab::on_leMealPrice_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    countTotal();
}

void WReservationRoomTab::on_deEntry_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    checkDatesCross();
}

void WReservationRoomTab::on_deDeparture_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)
    checkDatesCross();
}


void WReservationRoomTab::on_cbArrangment_currentIndexChanged(int index)
{
    if (index < 0) {
        ui->sbMealQty->setEnabled(false);
        ui->sbMealQty->setValue(0);
    }
    switch (ui->cbArrangment->currentData().toInt()) {
    case ARRANGEMENT_BO:
        ui->sbMealQty->setEnabled(false);
        ui->sbMealQty->setValue(0);
        break;
    default:
        ui->sbMealQty->setEnabled(true);
        ui->sbMealQty->setValue(ui->sbMan->value() + ui->sbWoman->value() + ui->sbChild->value());
    }
}

void WReservationRoomTab::on_btnAllNation_clicked()
{
    QString nation;
    for (int i = 0; i < ui->tblGuest->rowCount(); i++) {
        if (i == 0) {
            CI_Guest *g = CacheGuest::instance()->get(ui->tblGuest->toString(i, 0));
            nation = g->fNatShort;
            continue;
        }
        fDbBind[":f_nation"] = nation;
        fDb.update("f_guests", fDbBind, where_id(ui->tblGuest->toString(i, 0)));
    }
}
