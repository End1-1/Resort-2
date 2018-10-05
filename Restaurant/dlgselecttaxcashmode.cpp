#include "dlgselecttaxcashmode.h"
#include "ui_dlgselecttaxcashmode.h"

DlgSelectTaxCashMode::DlgSelectTaxCashMode(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgSelectTaxCashMode)
{
    ui->setupUi(this);
    fResult = 0;
}

DlgSelectTaxCashMode::~DlgSelectTaxCashMode()
{
    delete ui;
}

bool DlgSelectTaxCashMode::getCashMode(int &mode)
{
    bool result = false;
    DlgSelectTaxCashMode *d = new DlgSelectTaxCashMode(fParent);
    if (d->exec() == QDialog::Accepted) {
        mode = d->fResult;
        result = true;
    }
    delete d;
    return result;
}

void DlgSelectTaxCashMode::on_btnCash_clicked()
{
    fResult = tax_mode_cash;
    accept();
}

void DlgSelectTaxCashMode::on_btnCashLess_clicked()
{
    fResult = tax_mode_card;
    accept();
}

void DlgSelectTaxCashMode::on_btnCancel_clicked()
{
    reject();
}
