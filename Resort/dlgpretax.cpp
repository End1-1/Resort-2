#include "dlgpretax.h"
#include "ui_dlgpretax.h"
#include "printtax.h"

DlgPreTax::DlgPreTax(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPreTax)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 300, 100, 50, 0);
}

DlgPreTax::~DlgPreTax()
{
    delete ui;
}

void DlgPreTax::addRow(const QList<QVariant> &data)
{
    Utils::tableAppendRowData(ui->tblData, data, Qt::DisplayRole);
    QTableWidgetItem *item = ui->tblData->item(ui->tblData->rowCount() - 1, 3);
    item->setCheckState(Qt::Checked);
    countTotal();
}

void DlgPreTax::setInvoice(const QString &invoice)
{
    fInvoice = invoice;
}

void DlgPreTax::countTotal()
{
    double total = 0;
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 3);
        if (item->checkState() == Qt::Checked) {
            total += ui->tblData->toDouble(i, 2);
        }
    }
    ui->leTotal->setDouble(total);
}

void DlgPreTax::on_tblData_clicked(const QModelIndex &index)
{
    if (index.column() == 3) {
        countTotal();
    }
}

void DlgPreTax::on_btnOk_clicked()
{
    for (int  i = 0; i < ui->tblData->rowCount(); i++) {
        QTableWidgetItem *item = ui->tblData->item(i, 3);
        if (item->checkState() == Qt::Checked) {
            PrintTax::printAdvance(ui->tblData->toDouble(i, 2), ui->tblData->toInt(i, 4), QString("AV%1%2").arg(112233).arg(i));
            fDbBind[":f_fiscal"] = 1;
            fDb.update("m_register", fDbBind, where_id(ui->tblData->toInt(i, 0)));
            if (fInvoice > 0) {
                fDbBind[":f_amount"] = ui->tblData->toDouble(i, 2);
                fDbBind[":f_id"] = fInvoice;
                fDb.select("update m_v_invoice set f_prepaid=f_prepaid+:f_amount where f_id=:f_id", fDbBind, fDbRows);
            }
        }
    }
    accept();
}

void DlgPreTax::on_btnCancel_clicked()
{
    reject();
}
