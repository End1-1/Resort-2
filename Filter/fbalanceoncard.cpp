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
    rg->fModel->setColumn(150, "", tr("Code"))
            .setColumn(200, "", tr("Info"))
            .setColumn(100, "", tr("Balance"));
    QString sql = "select c.f_code, c.f_info, coalesce(u.f_amount, '-') as f_balance "
                "from d_gift_cart  c "
                "left join (select f_code, sum(f_amount) as f_amount from d_gift_cart_use group by 1) u on u.f_code=c.f_code ";
    if (ui->rbAll->isChecked()) {

    } else if (ui->rbFree->isChecked()) {
        sql += "where u.f_amount=c.f_initialamount ";
    } else {
        sql += "where coalesce(u.f_amount, 0)=0";
    }
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 2;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}
