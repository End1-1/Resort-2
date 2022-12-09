#include "fcouponsservice.h"
#include "ui_fcouponsservice.h"
#include "wreportgrid.h"
#include "reportquery.h"

FCouponsService::FCouponsService(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCouponsService)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Coupons of services"), ":/images/talon.png");
    fReportQuery = new ReportQuery("coupon_service");
}

FCouponsService::~FCouponsService()
{
    delete ui;
}

void FCouponsService::apply(WReportGrid *rg)
{
    QString query = fReportQuery->query;
    query.replace("%d1%", ui->leD1->dateMySql()).replace("%d2%", ui->leD2->dateMySql());
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    QList<double> vals;
    rg->fModel->sumOfColumns(fReportQuery->sumColumns, vals);
    rg->setTblTotalData(fReportQuery->sumColumns, vals);
    for (QMap<int, int>::const_iterator it = fReportQuery->columnsWidths.constBegin(); it != fReportQuery->columnsWidths.constEnd(); it++) {
        rg->fTableView->setColumnWidth(it.key(), it.value());
    }
}

QWidget *FCouponsService::firstElement()
{
    return ui->leD1;
}

QString FCouponsService::reportTitle()
{
    return tr("Coupon of service remains");
}
