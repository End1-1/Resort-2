#include "fcladvance.h"
#include "ui_fcladvance.h"
#include "wreportgrid.h"

FCLAdvance::FCLAdvance(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCLAdvance)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("C/L available amounts"), ":/images/price-tag.png");
}

FCLAdvance::~FCLAdvance()
{
    delete ui;
}

QString FCLAdvance::reportTitle()
{
    return QString("%1").arg(tr("C/L available amounts"));
}

QWidget *FCLAdvance::firstElement()
{
    return this;
}

void FCLAdvance::apply(WReportGrid *rg)
{
    QString query = "select m.f_cityLedger, c.f_name, sum(m.f_amountAmd*f_sign) as amount from m_register m "
                    "inner join f_city_ledger c on c.f_id=m.f_cityLedger "
                    "where m.f_source='TC' and f_canceled=0 :cond "
                    "group by 1, 2 "
                    "having sum(m.f_amountAmd*f_sign) > 0";
    query.replace(":cond", "");
    rg->fModel->clearColumns();
    rg->fModel->setColumn(100, "", tr("C/L code"))
            .setColumn(300, "", tr("City ledger"))
            .setColumn(100, "", tr("amount"));
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 2;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}
