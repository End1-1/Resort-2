#include "roomstate.h"
#include "ui_roomstate.h"
#include "vauchers.h"
#include "cachereservation.h"
#include "paymentmode.h"

RoomState::RoomState(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RoomState)
{
    ui->setupUi(this);
    ui->wdtOut->setVisible(false);
    fDockState = new DWSelectorRoomState(this);
    fDockState->configure();
    fDockState->setSelector(ui->leNewState);
    connect(fDockState, SIGNAL(roomState(CI_RoomState*)), this, SLOT(roomState(CI_RoomState*)));
    fTrackControl = new TrackControl(TRACK_ROOM_STATE);
    adjustSize();
}

RoomState::~RoomState()
{
    delete ui;
}

void RoomState::setRoom(CI_Room *r)
{
    ui->leRoomCode->setText(r->fCode);
    ui->leRoomName->setText(r->fName);
    ui->leCurrentState->setInt(r->fState);
    ui->leCurrentStateName->setText(CacheRoomState::instance()->get(r->fState)->fName);
    fRoom = r;
    DatabaseResult dr;
    fDbBind[":f_room"] = r->fCode;
    dr.select(fDb, "select count(f_id) as c , sum(f_state) as s from f_room_inventory_journal where f_room=:f_room", fDbBind);
    if (dr.value("c").toInt() == dr.value("s").toInt()) {
        ui->lbStatus->setPixmap(QPixmap(":/images/ball-green.png"));
    } else {
        ui->lbStatus->setPixmap(QPixmap(":/images/ball-red.png"));
    }
}

void RoomState::roomState(CI_RoomState *c)
{
    dockResponse<CI_RoomState, CacheRoomState>(ui->leNewState, ui->leNewStateName, c);
    if (c) {
        switch (c->fCode.toInt()) {
        case ROOM_STATE_DIRTY:
        case ROOM_STATE_NONE:
            break;
        case ROOM_STATE_OUTOF:
            ui->wdtOut->setVisible(true);
            adjustSize();
            break;
        default:
            ui->leNewState->clear();
            ui->leNewStateName->clear();
            message_error_tr("This state is not available now");
            break;
        }
    }
}


void RoomState::on_btnCancel_clicked()
{
    reject();
}

void RoomState::on_btnOk_clicked()
{
    if (ui->leNewState->text().isEmpty()) {
        message_error_tr("Select the new state for room");
        return;
    }

    QString add = ui->leRoomCode->text();
    if (ui->leNewState->asInt() == ROOM_STATE_OUTOF) {
        QString fId;
        QString invId = uuuid(VAUCHER_INVOICE_N, fAirDb);
        add += QString(" %1 - %2")
                .arg(ui->deStart->text(), ui->deEnd->text());
        fDbBind[":f_state"] = RESERVE_OUTOFROOM;
        fDbBind[":f_reserveState"] = CONFIRM_BLOCK;
        fDbBind[":f_room"] = fRoom->fCode;
        fDbBind[":f_roomFee"] = fRoom->fPrice;
        fDbBind[":f_arrangement"] = 0;
        fDbBind[":f_paymentType"] = PAYMENT_CASH;
        fDbBind[":f_man"] = 1;
        fDbBind[":f_mealIncluded"] = 1;
        fDbBind[":f_cardex"] = fPreferences.getDb(def_default_cardex);
        fDbBind[":f_startDate"] = ui->deStart->date();
        fDbBind[":f_endDate"] = ui->deEnd->date();
        fDbBind[":f_author"] = WORKING_USERID;
        fDbBind[":f_invoice"] = invId;
        fDbBind[":f_remarks"] = ui->teRemarks->toPlainText();
        fId = uuuid("RS", fAirDb);
        fDb.insertId("f_reservation", fId);
        fDbBind[":f_id"] = fId;
        fDb.update("f_reservation", fDbBind, where_id(ap(fId)));

        fTrackControl = new TrackControl(TRACK_RESERVATION);
        fTrackControl->insert("-=CREATED OUTOFORDER=-", QString("%1 - %2")
                                  .arg(ui->deStart->text())
                                  .arg(ui->deEnd->text()), "");
        BroadcastThread::cmdRefreshCache(cid_reservation, fId);
        if (ui->deStart->date() == WORKING_DATE) {
            fDbBind[":f_id"] = ui->leRoomCode->text();
            fDbBind[":f_state"] = ui->leNewState->text();
            fDb.select("update f_room set f_state=:f_state where f_id=:f_id", fDbBind, fDbRows);
            fRoom->fState = ui->leNewState->asInt();
        }
    } else {
        if (ui->lbOO->text() != "OK") {
            message_error_tr("Invalid date range");
            return;
        }
        if (ui->leCurrentState->asInt() == ROOM_STATE_OUTOF) {
            fDbBind[":f_room"] = ui->leRoomCode->asInt();
            fDbBind[":f_date"] = WORKING_DATE;
            fDbBind[":f_state"] = RESERVE_OUTOFROOM;
            fDb.select("select f_id from f_reservation where f_room=:f_room "
                       "and :f_date>=f_startDate and :f_date<=f_endDate and f_state=:f_state", fDbBind, fDbRows);
            if (fDbRows.count() > 0) {
                fDbBind[":f_state"] = RESERVE_SERVICE_REMOVED;
                fDb.update("f_reservation", fDbBind, where_id(ap(fDbRows.at(0).at(0).toString())));
                BroadcastThread::cmdRefreshCache(cid_reservation, fDbRows.at(0).at(0).toString());
            }
        } else {

        }
        fDbBind[":f_id"] = ui->leRoomCode->text();
        fDbBind[":f_state"] = ui->leNewState->text();
        fDb.select("update f_room set f_state=:f_state where f_id=:f_id", fDbBind, fDbRows);
        fRoom->fState = ui->leNewState->asInt();
    }
    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_oldState"] = ui->leCurrentState->asInt();
    fDbBind[":f_newState"] = ui->leNewState->asInt();
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_comment"] = ui->teRemarks->toPlainText();
    fDb.insert("f_room_state_change", fDbBind);
    fTrackControl->insert("Room state changed", ui->leCurrentStateName->text(), ui->leNewStateName->text() + add);
    BroadcastThread::cmdRefreshCache(cid_room, ui->leRoomCode->text());
    accept();
}

void RoomState::on_deStart_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    checkOO();
}

void RoomState::on_deEnd_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    checkOO();
}

void RoomState::checkOO()
{
    CacheReservation *c = CacheReservation::instance();
    QMap<QString, CI_Reservation*> out;
    bool startOk = true;
    bool endOk = true;
    c->check(ui->deStart->date(), ui->deEnd->date(), ui->leRoomCode->asInt(), out, startOk, endOk, "");
    if (!startOk || !endOk) {
        ui->lbOO->setText("NO");
    } else {
        ui->lbOO->setText("OK");
    }
}
