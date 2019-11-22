#include "dlgpretax.h"
#include "ui_dlgpretax.h"

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

}

void DlgPreTax::on_btnCancel_clicked()
{
    reject();
}
