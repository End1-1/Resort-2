#include "dlgooroomprop.h"
#include "ui_dlgooroomprop.h"
#include "cacheroom.h"

DlgOORoomProp::DlgOORoomProp(CI_Reservation *reserve, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgOORoomProp)
{
    ui->setupUi(this);
    fReservation = reserve;
    ui->leReserve->setText(fReservation->fId);
    ui->leRoom->setText(fReservation->fRoom);
    ui->teRemarks->setPlainText(fReservation->fRemarks);
    ui->deStart->setDate(fReservation->fDateStart);
    ui->deEnd->setDate(fReservation->fDateEnd);
    fTrackControl = new TrackControl(TRACK_ROOM_STATE);
}

DlgOORoomProp::~DlgOORoomProp()
{
    delete ui;
}

void DlgOORoomProp::on_btnCancel_clicked()
{
    reject();
}

void DlgOORoomProp::on_btnOk_clicked()
{
    if (ui->lbOO->text() != "OK") {
        message_error_tr("Invalid date range");
        return;
    }
    fDbBind[":f_startDate"] = ui->deStart->date();
    fDbBind[":f_endDate"] = ui->deEnd->date();
    fDbBind[":f_remarks"] = ui->teRemarks->toPlainText();
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserve->text())));
    accept();
    QString range = QString(" %1 - %2, %3")
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text())
            .arg(ui->teRemarks->toPlainText());
    if (ui->deStart->date() <= WORKING_DATE) {
        fDbBind[":f_id"] = ui->leRoom->text();
        fDbBind[":f_state"] = ROOM_STATE_OUTOF;
        fDb.select("update f_room set f_state=:f_state where f_id=:f_id", fDbBind, fDbRows);
        fTrackControl->insert("O/O modification", "?" , "OO");
        BroadcastThread::cmdRefreshCache(cid_room, ui->leRoom->text());
    }
    if (fReservation->fDateStart <= WORKING_DATE && ui->deStart->date() > WORKING_DATE) {
        fDbBind[":f_id"] = ui->leRoom->text();
        fDbBind[":f_state"] = ROOM_STATE_NONE;
        fDb.select("update f_room set f_state=:f_state where f_id=:f_id", fDbBind, fDbRows);
        fTrackControl->insert("O/O modification", "OO" , "VR");
        BroadcastThread::cmdRefreshCache(cid_room, ui->leRoom->text());
    }
    fTrackControl->insert("O/O modification", ui->leRoom->text() , range);
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReserve->text());
}

void DlgOORoomProp::on_deEnd_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    checkOO();
}

void DlgOORoomProp::on_deStart_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    checkOO();
}

void DlgOORoomProp::checkOO()
{
    QMap<QString, CI_Reservation*> out;
    bool startOk = true;
    bool endOk = true;
    CacheReservation::instance()->check(ui->deStart->date(), ui->deEnd->date(), ui->leRoom->asInt(), out, startOk, endOk, fReservation->fId);
    if (!startOk || !endOk) {
        ui->lbOO->setText("NO");
    } else {
        ui->lbOO->setText("OK");
    }
}

void DlgOORoomProp::on_btmRemove_clicked()
{
    if (message_confirm_tr("Confirm to remove") != QDialog::Accepted) {
        return;
    }
    accept();
    QString range = QString(" %1 - %2, %3")
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text())
            .arg(ui->teRemarks->toPlainText());
    fTrackControl->insert("O/O removed", ui->leRoom->text() , range);
    fDbBind[":f_state"] = RESERVE_REMOVED;
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReserve->text())));
    if (fReservation->fDateStart <= WORKING_DATE) {
        fDbBind[":f_state"] = ROOM_STATE_NONE;
        fDb.update("f_room", fDbBind, where_id(ui->leRoom->asInt()));
        BroadcastThread::cmdRefreshCache(cid_room, ui->leRoom->text());
    }
    BroadcastThread::cmdRefreshCache(cid_reservation, ui->leReserve->text());
}
