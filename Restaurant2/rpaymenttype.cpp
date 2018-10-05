#include "rpaymenttype.h"
#include "ui_rpaymenttype.h"

RPaymentType::RPaymentType(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RPaymentType)
{
    ui->setupUi(this);
    QList<int> pm;
    pm << 0 << 1 << 4 << 2 << 5 << 3;
    for (int i = 0; i < pm.count(); i++) {
        ui->lstData->item(i)->setData(Qt::UserRole, pm[i]);
    }
}

RPaymentType::~RPaymentType()
{
    delete ui;
}

int RPaymentType::getPaymentMode(QWidget *parent, int &addResult)
{
    int result = -1;
    RPaymentType *r = new RPaymentType(parent);
    if (r->exec() == QDialog::Accepted) {
        result = r->ui->lstData->currentRow();
        addResult = r->ui->lstData->item(r->ui->lstData->currentRow())->data(Qt::UserRole).toInt();
    }
    delete r;
    return result;
}

void RPaymentType::on_btnOk_clicked()
{
    if (ui->lstData->currentRow() < 0) {
        return;
    }
    accept();
}

void RPaymentType::on_btnCancel_clicked()
{
    reject();
}
