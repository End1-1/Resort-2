#include "fcouponstatistics.h"
#include "ui_fcouponstatistics.h"
#include "wreportgrid.h"

FCouponStatistics::FCouponStatistics(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCouponStatistics)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Coupons statistics"), ":/images/discount.png");
}

FCouponStatistics::~FCouponStatistics()
{
    delete ui;
}

QString FCouponStatistics::reportTitle()
{
    return QString("%1")
            .arg(tr("Coupons statistics"));
}

QWidget *FCouponStatistics::firstElement()
{
    return this;
}

void FCouponStatistics::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(200, "", tr("Partner"))
            .setColumn(150, "", tr("Seria"))
            .setColumn(100, "", tr("Total"))
            .setColumn(100, "", tr("Used"))
            .setColumn(100, "", tr("Active"));
    QString query = "select distinct(p.f_name), cs.f_name, ct.qty, cu.qty, ct.qty-cu.qty as ca "
            "from d_coupon c "
            "left join r_partners p on p.f_id=c.f_partner "
            "left join d_coupon_seria cs on cs.f_id=c.f_seria "
            "left join (select f_partner, f_seria, count(f_id) as qty from d_coupon group by 1, 2) ct on ct.f_partner=c.f_partner and ct.f_seria=c.f_seria "
            "left join (select f_partner, f_seria, count(f_id) as qty from d_coupon where f_used=1 group by 1, 2) cu on cu.f_partner=c.f_partner and cu.f_seria=c.f_seria  ";
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<int> cols;
    QList<double> vals;
    cols << 2 << 3 << 4;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}
