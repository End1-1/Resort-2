#include "dlgpartnerpayment.h"
#include "ui_dlgpartnerpayment.h"

DlgPartnerPayment::DlgPartnerPayment(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPartnerPayment)
{
    ui->setupUi(this);
}

DlgPartnerPayment::~DlgPartnerPayment()
{
    delete ui;
}
