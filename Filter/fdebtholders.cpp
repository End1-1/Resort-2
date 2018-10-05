#include "fdebtholders.h"
#include "ui_fdebtholders.h"
#include "wreportgrid.h"

FDebtHolders::FDebtHolders(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FDebtHolders)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Costumers"), ":/images/car2.png");
}

FDebtHolders::~FDebtHolders()
{
    delete ui;
}

QString FDebtHolders::reportTitle()
{
    return QString("%1")
            .arg(tr("Costumers"));
}

QWidget *FDebtHolders::firstElement()
{
    return this;
}

void FDebtHolders::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->clearColumns();
    rg->fModel->setColumn(0, "", tr("Costumer"))
            .setColumn(300, "", tr("Costumer"))
            .setColumn(120, "", tr("Phone"))
            .setColumn(200, "", tr("Numbers"));;
    QString sql = "select oc.f_costumer, dh.f_name, dh.f_info, g.f_gov "
                  " from o_car oc "
                  "left join o_debt_holder dh on dh.f_id=oc.f_costumer "
                  "left join (select f_holder, group_concat(f_govNumber) as f_gov from o_debt_holder_car group by 1) g on g.f_holder=dh.f_id ";

    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
}
