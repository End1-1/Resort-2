#include "fcouponsservice.h"
#include "ui_fcouponsservice.h"
#include "wreportgrid.h"

namespace {

const char *couponServiceSql = R"(
select tig.f_id as `Կոդ`,  tig.f_name as `Տեսակ`, tig.f_price as `Գին`,
 COALESCE(td1.qty, 0) - COALESCE(td2.qty, 0) as `Սկիզբ`, (COALESCE(td1.qty, 0) - COALESCE(td2.qty, 0)) * tig.f_price as ``,
 coalesce(t4.qty4, 0) as `Մուտք`,  coalesce(t4.qty4, 0) * tig.f_price as ``,
 coalesce(t2.qty2, 0) as `Վաճառք`, coalesce(t2.qty2, 0) * tig.f_price as ``,
 coalesce(t3.qty3, 0) as `Սպառում`, coalesce(t3.qty3, 0) * tig.f_price as ``,
 COALESCE(td1.qty, 0) - COALESCE(td2.qty, 0)+coalesce(t4.qty4, 0)-coalesce(t2.qty2, 0) as `Մնացորդ`, (COALESCE(td1.qty, 0) - COALESCE(td2.qty, 0)+coalesce(t4.qty4, 0)-coalesce(t2.qty2, 0))*tig.f_price as ``

from talon_service_items_group tig

LEFT  JOIN (SELECT t1.f_group, COUNT(t1.f_id) as qty from talon_service t1
	inner join talon_documents_header h1 on h1.f_id=t1.f_trregister
     where h1.f_date<%1 GROUP BY 1) td1 ON td1.f_group=tig.f_id

LEFT  JOIN (SELECT t1.f_group, COUNT(t1.f_id) as qty from talon_service t1
	inner join talon_documents_header h1 on h1.f_id=t1.f_trsale
     where h1.f_date<%1 GROUP BY 1) td2 ON td2.f_group=tig.f_id


 left join (select t4.f_group , count(t4.f_group) as qty4 from talon_service t4
	inner join talon_documents_header h4 on h4.f_id=t4.f_trregister
     where h4.f_date between %1 and %2
     group by 1) t4 on t4.f_group=tig.f_id

 left join (select t2.f_group , count(t2.f_group) as qty2 from talon_service t2
	inner join talon_documents_header h2 on h2.f_id=t2.f_trsale
     where h2.f_date between %1 and %2
     group by 1) t2 on t2.f_group=tig.f_id

 left join (select t3.f_group , count(t3.f_group) as qty3 from talon_service t3
	inner join talon_documents_header h3 on h3.f_id=t3.f_trback
     where h3.f_date between %1 and %2
     group by 1) t3 on t3.f_group=tig.f_id

 order by 1
)";

const QList<int> sumColumns = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

} // namespace

FCouponsService::FCouponsService(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCouponsService)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Coupons of services"), ":/images/talon.png");
}

FCouponsService::~FCouponsService()
{
    delete ui;
}

void FCouponsService::apply(WReportGrid *rg)
{
    const QString query = QString(couponServiceSql)
            .arg(ui->leD1->dateMySql(), ui->leD2->dateMySql());
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<double> vals;
    rg->fModel->sumOfColumns(sumColumns, vals);
    rg->setTblTotalData(sumColumns, vals);
    rg->fTableView->resizeColumnsToContents();
}

QWidget *FCouponsService::firstElement()
{
    return ui->leD1;
}

QString FCouponsService::reportTitle()
{
    return tr("Coupon of service remains");
}
