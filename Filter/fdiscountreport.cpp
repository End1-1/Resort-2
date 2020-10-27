#include "fdiscountreport.h"
#include "ui_fdiscountreport.h"
#include "wreportgrid.h"
#include "fdiscountfullstat.h"

FDiscountReport::FDiscountReport(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FDiscountReport)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Discount"), ":/images/discount.png");
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClickOnRow(QList<QVariant>)));
}

FDiscountReport::~FDiscountReport()
{
    delete ui;
}

QString FDiscountReport::reportTitle()
{
    return QString("%1 %2-%3")
            .arg(tr("Discount"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text());
}

QWidget *FDiscountReport::firstElement()
{
    return ui->deStart;
}

void FDiscountReport::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(120, "", tr("Date"))
            .setColumn(100, "", tr("Card"))
            .setColumn(200, "", tr("Costumer"))
            .setColumn(80, "", tr("Qty"))
            .setColumn(80, "", tr("Total"))
            .setColumn(80, "", tr("Discount"));
    QString sql = "select op.f_datecash, d.f_card, d.f_name,  count(hp.f_id), sum(op.f_total), sum(hp.f_discount) \
            from o_header op \
            left join o_header_payment hp on hp.f_id=op.f_id \
            left join d_car_client d on d.f_id=hp.f_costumer \
            where hp.f_costumer>0 and op.f_state=2 \
            and op.f_dateCash between :date1 and :date2 \
            group by 1,2";
    sql.replace(":date1", ui->deStart->dateMySql());
    sql.replace(":date2", ui->deEnd->dateMySql());
    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 3 << 4 << 5;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FDiscountReport::doubleClickOnRow(const QList<QVariant> &row)
{
    if (row.count() == 0) {
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_card"] = row.at(0);
    dr.select(fDb, "select f_id from d_car_client where f_card=:f_card", fDbBind);
    if (dr.rowCount() > 0) {
        FDiscountFullStat::openReport(ui->deStart->date(), ui->deEnd->date(), dr.value(0).toString());
    }
}
