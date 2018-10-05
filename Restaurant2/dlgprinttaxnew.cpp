#include "dlgprinttaxnew.h"
#include "ui_dlgprinttaxnew.h"

DlgPrintTaxNew::DlgPrintTaxNew(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPrintTaxNew)
{
    ui->setupUi(this);
}

DlgPrintTaxNew::~DlgPrintTaxNew()
{
    delete ui;
}
