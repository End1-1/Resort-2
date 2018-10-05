#include "dlginvoiceprintoption.h"
#include "ui_dlginvoiceprintoption.h"

DlgInvoicePrintOption::DlgInvoicePrintOption(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgInvoicePrintOption)
{
    ui->setupUi(this);
    fResult = pio_none;
}

DlgInvoicePrintOption::~DlgInvoicePrintOption()
{
    delete ui;
}

int DlgInvoicePrintOption::getOption()
{
    DlgInvoicePrintOption *d = new DlgInvoicePrintOption(fPreferences.getDefaultParentForMessage());
    d->exec();
    int result = d->fResult;
    delete d;
    return result;
}

void DlgInvoicePrintOption::on_btnCancel_clicked()
{
    reject();
}

void DlgInvoicePrintOption::on_btnOk_clicked()
{
    if (fResult == pio_none) {
        message_error_tr("Invoice print option was not selected");
        return;
    }
    accept();
}

void DlgInvoicePrintOption::on_rbGuest_clicked()
{
    fResult = pio_guest;
}

void DlgInvoicePrintOption::on_rbCompany_clicked()
{
    fResult = pio_comp;
}

void DlgInvoicePrintOption::on_rbGuestCompanySeparate_clicked()
{
    fResult = pio_guestcomp_ser;
}

void DlgInvoicePrintOption::on_rbGuestCompanyTogether_clicked()
{
    fResult = pio_guestcomp_tog;
}

void DlgInvoicePrintOption::on_pushButton_clicked()
{
    fResult = pio_guest;
    accept();
}

void DlgInvoicePrintOption::on_pushButton_2_clicked()
{
    fResult = pio_comp;
    accept();
}

void DlgInvoicePrintOption::on_pushButton_3_clicked()
{
    fResult = pio_guestcomp_ser;
    accept();
}

void DlgInvoicePrintOption::on_pushButton_4_clicked()
{
    fResult = pio_guestcomp_tog;
    accept();
}
