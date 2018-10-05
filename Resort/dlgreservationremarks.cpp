#include "dlgreservationremarks.h"
#include "ui_dlgreservationremarks.h"

DlgReservationRemarks::DlgReservationRemarks(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReservationRemarks)
{
    ui->setupUi(this);
    fTrackControl = new TrackControl(TRACK_RESERVATION_REMARKS);
    fDb.select("select f_id, f_text from f_reservation_remarks", fDbBind, fDbRows);
    ui->tblData->setColumnWidth(0, 0);
    ui->tblData->setColumnWidth(1, 500);
    ui->tblData->setRowCount(fDbRows.count());
    int row = 0;
    foreach_rows {
        ui->tblData->setItem(row, 0, new QTableWidgetItem(it->at(0).toString()));
        ui->tblData->setItem(row++, 1, new QTableWidgetItem(it->at(1).toString()));
    }
}

DlgReservationRemarks::~DlgReservationRemarks()
{
    delete ui;
}

bool DlgReservationRemarks::Remarks(QString &remark)
{
    bool result = false;
    DlgReservationRemarks *r = new DlgReservationRemarks(fPreferences.getDefaultParentForMessage());
    r->ui->teText->setPlainText(remark);
    if (r->exec() == QDialog::Accepted) {
        result = true;
        remark = r->ui->teText->toPlainText();
    }
    delete r;
    return result;
}

void DlgReservationRemarks::on_tblData_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->teText->setPlainText(index.data(Qt::DisplayRole).toString());
}

void DlgReservationRemarks::on_btnRemoveSelected_clicked()
{
    int row = ui->tblData->currentRow();
    if (row < 0) {
        return;
    }
    if (message_yesnocancel(tr("Are you sure to delete selected text?")) != RESULT_YES) {
        return;
    }
    fDbBind[":f_id"] = ui->tblData->toInt(row, 0);
    fDb.select("delete from f_reservation_remarks where f_id=:f_id", fDbBind, fDbRows);
    ui->tblData->removeRow(row);
    fTrackControl->insert("Remove remark", ui->tblData->toString(row, 1), "");
}

void DlgReservationRemarks::on_btnSaveAs_clicked()
{
    fDbBind[":f_text"] = ui->teText->toPlainText();
    int id = fDb.insert("f_reservation_remarks", fDbBind);
    ui->tblData->setRowCount(ui->tblData->rowCount() + 1);
    ui->tblData->setItem(ui->tblData->rowCount() - 1, 0, new QTableWidgetItem());
    ui->tblData->setItem(ui->tblData->rowCount() - 1, 1, new QTableWidgetItem());
    ui->tblData->setValue(ui->tblData->rowCount() - 1, 0, id);
    ui->tblData->setValue(ui->tblData->rowCount() - 1, 1, ui->teText->toPlainText());
    fTrackControl->insert("New remarks", ui->teText->toPlainText(), "");
}

void DlgReservationRemarks::on_btnSaveCurrent_clicked()
{
    int row = ui->tblData->currentRow();
    if (row < 0) {
        message_error_tr("No existing reservation was selected");
        return;
    }
    fDbBind[":f_text"] = ui->teText->toPlainText();
    fDb.update("f_reservation_remarks", fDbBind, where_id(ui->tblData->toInt(row, 0)));
    fTrackControl->insert("Remarks updated", ui->tblData->toString(row, 1), ui->teText->toPlainText());
    ui->tblData->setValue(row, 1, ui->teText->toPlainText());
}

void DlgReservationRemarks::on_btnOk_clicked()
{
    accept();
}

void DlgReservationRemarks::on_btnCancel_clicked()
{
    reject();
}

void DlgReservationRemarks::on_tblData_doubleClicked(const QModelIndex &index)
{
    on_tblData_clicked(index);
    accept();
}
