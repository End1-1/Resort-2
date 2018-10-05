#include "fdiscountfullstat.h"
#include "ui_fdiscountfullstat.h"
#include "wreportgrid.h"

FDiscountFullStat::FDiscountFullStat(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FDiscountFullStat)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Discount full statistics"), ":/images/discount.png");
}

FDiscountFullStat::~FDiscountFullStat()
{
    delete ui;
}

QString FDiscountFullStat::reportTitle()
{
    QString title = QString("%1 %2 - %3")
            .arg(tr("Discount full report"))
            .arg(ui->wd->dss1())
            .arg(ui->wd->dss2());
    if (!ui->leDiscountCard->isEmpty()) {
        title += ", " + ui->leDiscountCard->text();
    }
    return title;
}

QWidget *FDiscountFullStat::firstElement()
{
    return ui->wd->fw();
}

void FDiscountFullStat::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(100, "", tr("Card"))
            .setColumn(200, "", tr("Costumer"))
            .setColumn(80, "", tr("Total"))
            .setColumn(80, "", tr("Discount"));
    QString sql = "select op.f_dateCash, d.f_card, d.f_name,  op.f_total, hp.f_discount \
            from o_header op \
            left join o_header_payment hp on hp.f_id=op.f_id \
            left join d_car_client d on d.f_id=hp.f_costumer \
            where hp.f_costumer>0 and op.f_state=2 \
            and op.f_dateCash between :date1 and :date2 :cost ";
    sql.replace(":date1", ui->wd->ds1());
    sql.replace(":date2", ui->wd->ds2());
    if (ui->leDiscountCard->isEmpty()) {
        sql.replace(":cost", "");
    } else {
        sql.replace(":cost", QString(" and hp.f_costumer=" + ui->leDiscountCard->text()));
    }
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 2 << 3 << 4;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FDiscountFullStat::openReport(const QDate &d1, const QDate &d2, const QString discountCard)
{
    WReportGrid *rg = addTab<WReportGrid>();
    FDiscountFullStat *fs = new FDiscountFullStat(rg);
    rg->addFilterWidget(fs);
    fs->ui->wd->setD1(d1);
    fs->ui->wd->setD2(d2);
    fs->ui->leDiscountCard->setText(discountCard);
    fs->apply(rg);
}
