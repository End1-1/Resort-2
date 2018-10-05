#include "dlgprinttaxsideoption.h"
#include "ui_dlgprinttaxsideoption.h"
#include "basewidget.h"

DlgPrintTaxSideOption::DlgPrintTaxSideOption(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPrintTaxSideOption)
{
    ui->setupUi(this);
}

DlgPrintTaxSideOption::~DlgPrintTaxSideOption()
{
    delete ui;
}

int DlgPrintTaxSideOption::printTaxSide()
{
    DlgPrintTaxSideOption *d = new DlgPrintTaxSideOption(fMainWindow);
    int result = d->exec();
    delete d;
    return result;
}

void DlgPrintTaxSideOption::on_btnPrint_clicked()
{
    if (ui->rbGuest->isChecked()) {
        done(pts_guest);
    } else {
        done(pts_company);
    }
}

void DlgPrintTaxSideOption::on_btnCancel_clicked()
{
    done(pts_none);
}
