#include "dlgtaxback.h"
#include "ui_dlgtaxback.h"
#include "basewidget.h"
#include "vauchers.h"

DlgTaxBack::DlgTaxBack(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgTaxBack)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                0, 300, 100, 0, 30);
}

DlgTaxBack::~DlgTaxBack()
{
    delete ui;
}

void DlgTaxBack::taxBack(const QString &invoice, QList<QList<QVariant> > &rows)
{
    DlgTaxBack *d = new DlgTaxBack(fMainWindow);
    d->fInvoice = invoice;
    Utils::fillTableWithData(d->ui->tblData, rows);
    for (int i = 0; i < d->ui->tblData->rowCount(); i++) {
        d->ui->tblData->item(i, 4)->setCheckState(Qt::Unchecked);
    }
    d->exec();
    delete d;
}

void DlgTaxBack::on_btnCancel_clicked()
{
    reject();
}

void DlgTaxBack::on_btnOK_clicked()
{
    for (int i = 0; i < ui->tblData->rowCount(); i++) {
        if (ui->tblData->item(i, 4)->checkState() == Qt::Checked) {
            fDbBind[":f_fiscal"] = 0;
            fDb.update("m_register", fDbBind, where_id(ap(ui->tblData->toString(i, 0))));
            if (ui->tblData->toString(i, 3) == VAUCHER_ADVANCE_N) {
                fDbBind[":f_id"] = fInvoice;
                fDbBind[":f_prepaid"] = ui->tblData->toDouble(i, 2);
                fDb.select("update m_v_invoice set f_prepaid=f_prepaid-:f_prepaid where f_id=:f_id", fDbBind, fDbRows);
            }
        }
    }
    accept();
}
