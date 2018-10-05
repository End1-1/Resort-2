#include "fpartnerpayments.h"
#include "ui_fpartnerpayments.h"
#include "wreportgrid.h"

FPartnerPayments::FPartnerPayments(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FPartnerPayments)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Partners payments"), "");
}

FPartnerPayments::~FPartnerPayments()
{
    delete ui;
}

QString FPartnerPayments::reportTitle()
{
    return QString("%1 %2 - %3")
            .arg(tr("Partners payments"));
}

QWidget *FPartnerPayments::firstElement()
{
    return this;
}

void FPartnerPayments::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
}
