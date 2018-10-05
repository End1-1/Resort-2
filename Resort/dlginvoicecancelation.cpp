#include "dlginvoicecancelation.h"
#include "ui_dlginvoicecancelation.h"
#include "dlgraiseuser.h"
#include "cacherights.h"
#include "pprintvaucher.h"
#include "vauchers.h"

DlgInvoiceCancelation::DlgInvoiceCancelation(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgInvoiceCancelation)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 0, 120, 0, 300, 100);
    fTrackControl = new TrackControl(TRACK_RESERVATION);
    fRaise = false;
}

DlgInvoiceCancelation::~DlgInvoiceCancelation()
{
    delete ui;
}

void DlgInvoiceCancelation::setTrackWindowId(const QString &id)
{
    fTrackControl->resetChanges();
    fTrackId = id;
}

void DlgInvoiceCancelation::addRow(const QList<QVariant> &data)
{
    int row = ui->tblData->rowCount();
    ui->tblData->setRowCount(row + 1);
    QString msg;
    for (int i = 0; i < ui->tblData->columnCount(); i++) {
        ui->tblData->setItem(row, i, new QTableWidgetItem(data.at(i).toString()));
        msg += data.at(i).toString() + " /";
    }
    fTrackControl->insert("Invoice cancelation", msg, "");
    if (QDate::fromString(data.at(2).toString(), def_date_format) != WORKING_DATE) {
        fRaise = true;
    }
}

void DlgInvoiceCancelation::on_btnCancel_clicked()
{
    reject();
}

void DlgInvoiceCancelation::on_btnOk_clicked()
{
    int raise = 0;
    if (!RIGHT(WORKING_USERGROUP, cr__invoice_cancelation_current_date)) {
        raise = cr__invoice_cancelation_current_date;
    }
    if (fRaise) {
        if (!RIGHT(WORKING_USERGROUP, cr__invoice_cancelation_previouse_date)) {
            raise = cr__invoice_cancelation_previouse_date;
        } else {
            raise = 0;
        }
    }
    if (raise > 0) {
        int raiseUser = DlgRaiseUser::raiseUser(raise);
        if (raiseUser == 0) {
            return;
        } else {
            fTrackControl->insert("Raise privileegs", QString::number(WORKING_USERID), QString::number(raiseUser));
        }
    }
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        removeVaucher(ui->tblData->toString(i, 0), ui->teReason->toPlainText());
    }
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        PPrintVaucher::printVaucher(ui->tblData->item(i, 0)->text());
    }
    accept();
}
