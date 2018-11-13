#include "fbalanceoncard.h"
#include "ui_fbalanceoncard.h"
#include "wreportgrid.h"

FBalanceOnCard::FBalanceOnCard(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FBalanceOnCard)
{
    ui->setupUi(this);
}

FBalanceOnCard::~FBalanceOnCard()
{
    delete ui;
}

QString FBalanceOnCard::reportTitle()
{
    return tr("Balances on the card");
}

QWidget *FBalanceOnCard::firstElement()
{
    return this;
}

void FBalanceOnCard::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(300, "", tr("Name"))
            .setColumn(200, "", tr("Info"))
            .setColumn(100, "", tr("Balance"));
    QString sql = "select f_name, f_info, substring(f_mode,locate(';', f_mode, locate(';', f_mode) + 1) +1, locate(';', f_mode, locate(';', f_mode, locate(';', f_mode) + 1)+1)-locate(';', f_mode, locate(';', f_mode) + 1)-1) from d_car_client where f_model=3";
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 2;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}
