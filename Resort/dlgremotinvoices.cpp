#include "dlgremotinvoices.h"
#include "ui_dlgremotinvoices.h"

DlgRemotInvoices::DlgRemotInvoices(bool local, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgRemotInvoices)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                120, 120, 120, 300);
    Database fSDb;
    if (local) {
        fSDb.copyConnectionParamsFrom(fDb);
        if (!fSDb.open()) {
            message_error_tr("Cannot connect to database");
            return;
        }
    } else {
        QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
        if (dbParams.count() < 4) {
            message_error_tr("Setup second database parameters");
            return;
        }
        fSDb.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
        if (!fSDb.open()) {
            message_error_tr("Cannot connect to second database");
            return;
        }
    }
    fSDb.select("select r.f_invoice, r.f_startDate, r.f_endDate, g.guest "
                "from f_reservation r "
                "left join guests g on g.f_id=r.f_guest "
                "where r.f_state=3 ", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblData, fDbRows);
}

DlgRemotInvoices::~DlgRemotInvoices()
{
    delete ui;
}

void DlgRemotInvoices::on_leSearch_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (arg1.isEmpty()) {
            ui->tblData->setRowHidden(i, false);
            continue;
        }
        ui->tblData->setRowHidden(i, true);
        for (int j = 0; j < ui->tblData->columnCount(); j++) {
            if (ui->tblData->item(i, j)->data(Qt::EditRole).toString().contains(arg1, Qt::CaseInsensitive)) {
                ui->tblData->setRowHidden(i, false);
                goto NEXT;
            }
        }
        NEXT:
        continue;
    }
}

void DlgRemotInvoices::on_tblData_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    fResult = ui->tblData->item(index.row(), 0)->data(Qt::EditRole).toString();
    accept();
}
