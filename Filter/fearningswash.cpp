#include "fearningswash.h"
#include "ui_fearningswash.h"
#include "wreportgrid.h"
#include "cacheresthall.h"

FEarningsWash::FEarningsWash(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FEarningsWash)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Earings"), ":/images/earn.png");
    ui->cbHall->setCache(cid_rest_hall);
}

FEarningsWash::~FEarningsWash()
{
    delete ui;
}

QString FEarningsWash::reportTitle()
{
    return QString("%1,%2 %3 - %4").arg(tr("Earnings")).arg(ui->cbHall->currentText()).arg(ui->wd->dss1()).arg(ui->wd->dss2());
}

void FEarningsWash::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(300, "", tr("Type"))
            .setColumn(300, "", tr("Service"))
            .setColumn(80, "", tr("Qty"))
            .setColumn(80, "", tr("Amount"));
    QString hallf;
    if (ui->cbHall->currentIndex() > 0) {
        hallf = QString("and oh.f_hall=%1").arg(ui->cbHall->currentData().toInt());
    }
    QString sql = QString("select dt.f_en,d.f_en,sum(od.f_qty),sum(od.f_total) "
            "from o_header oh  "
            "inner join o_dish od on od.f_header=oh.f_id "
            "inner join r_dish d on d.f_id=od.f_dish "
            "inner join r_dish_type dt on dt.f_id=d.f_type "
            "where (oh.f_dateCash between :date1  and :date2)  "
            "and oh.f_state in (2)  %1  and od.f_state in (1)  "
            "and (od.f_complex=0 or (od.f_complex>0 and od.f_complexId>0))  "
            "group by d.f_type,dt.f_en,od.f_dish,d.f_en ").arg(hallf);

    sql.replace(":date1", ui->wd->ds1());
    sql.replace(":date2", ui->wd->ds2());

    rg->fModel->setSqlQuery(sql);
    rg->fModel->apply(rg);
    QList<int> cols;
    cols << 2 << 3;
    QList<double> vals;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

QWidget *FEarningsWash::firstElement()
{
    return ui->wd->fw();
}

void FEarningsWash::setHall(int h)
{
    ui->cbHall->setIndexForData(h);
    fReportGrid->on_btnRefresh_clicked();
    fReportGrid->setupTabTextAndIcon(tr("Earings") + ", " + ui->cbHall->currentText(), ":/images/earn.png");
}
