#include "dlgtransfer.h"
#include "ui_dlgtransfer.h"

DlgTransfer::DlgTransfer(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgTransfer)
{
    ui->setupUi(this);
    ui->leInvoiceFrom->setVisible(false);
    ui->leInvoiceTo->setVisible(false);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 120, 200, 80);
    fDockRoom = new DWSelectorActiveRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->leRoomCodeTo);
    connect(fDockRoom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomTo(CI_ActiveRoom*)));
    fTrackControl = new TrackControl(TRACK_RESERVATION);
}

DlgTransfer::~DlgTransfer()
{
    delete ui;
}

void DlgTransfer::setRoomByCode(const QString &code)
{
    CI_ActiveRoom *r = CacheActiveRoom::instance()->get(code);
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leRoomCodeFrom, ui->leRoomNameFrom, r);
    if (r) {
        ui->leGuestFrom->setText(r->fGuestName);
        ui->leInvoiceFrom->setText(r->fInvoice);
        ui->leRoomCodeFrom->setText(r->fRoomCode);
        fTrackControl->resetChanges();
    }
}

void DlgTransfer::addRow(QList<QVariant> row)
{
    Utils::tableAppendRowData(ui->tblData, row, Qt::DisplayRole);
    ui->btnOk->setEnabled(true);
}

void DlgTransfer::on_chGuest_clicked(bool checked)
{
    ui->chCompany->setChecked(!checked);
}

void DlgTransfer::on_chCompany_clicked(bool checked)
{
    ui->chGuest->setChecked(!checked);
}

void DlgTransfer::on_btnCancel_clicked()
{
    reject();
}

void DlgTransfer::on_btnOk_clicked()
{
    QString errors;
    if (ui->leRoomCodeTo->asInt() == 0) {
        errors += tr("Destination room is not defined") + "<br>";
    }
    if (!errors.isEmpty()) {
        message_error(errors);
        return;
    }
    fDb.fDb.transaction();
    for (int i = 0, count = ui->tblData->rowCount(); i < count; i++) {
        fDbBind[":f_inv"] = ui->leInvoiceTo->text();
        fDbBind[":f_res"] = ui->leReserveTo->text();
        fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->item(i, 0)->text())));
        fTrackControl->insert("Move out from invoice", QString("%1 / %2 / %3 / %4")
                                  .arg(ui->leInvoiceFrom->text())
                                  .arg(ui->tblData->item(i, 1)->text())
                                  .arg(ui->tblData->item(i, 2)->text())
                                  .arg(ui->tblData->item(i, 3)->text()), "");
    }
    fTrackControl->saveChanges();
    fTrackControl->resetChanges();
    for (int i = 0, count = ui->tblData->rowCount(); i < count; i++) {
        fTrackControl->insert("Move into invoice", QString("%1 / %2 / %3 / %4")
                                  .arg(ui->leInvoiceTo->text())
                                  .arg(ui->tblData->item(i, 1)->text())
                                  .arg(ui->tblData->item(i, 2)->text())
                                  .arg(ui->tblData->item(i, 3)->text()), "");
    }
    fTrackControl->saveChanges();
    fDb.fDb.commit();
    accept();
}

void DlgTransfer::roomTo(CI_ActiveRoom *c)
{
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leRoomCodeTo, ui->leRoomNameTo, c);
    if (c) {
        ui->leGuestTo->setText(c->fGuestName);
        ui->leInvoiceTo->setText(c->fInvoice);
        ui->leRoomCodeTo->setText(c->fRoomCode);
        ui->leReserveTo->setText(c->fCode);
    }
}
