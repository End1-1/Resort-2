#include "fmonthlyoccperc.h"
#include "ui_fmonthlyoccperc.h"
#include "wreportgrid.h"

FMonthlyOccPerc::FMonthlyOccPerc(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FMonthlyOccPerc)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Yearly summary occupancy report"), ":/images/reports.png");
    ui->cbYear->addItem("2017");
    ui->cbYear->addItem("2018");
    ui->cbYear->setCurrentIndex(1);
}

FMonthlyOccPerc::~FMonthlyOccPerc()
{
    delete ui;
}

QWidget *FMonthlyOccPerc::firstElement()
{
    return ui->cbYear;
}

QString FMonthlyOccPerc::reportTitle()
{
    return QString("%1 - %2")
            .arg(tr("Yearly summary occupancy report"))
            .arg(ui->cbYear->currentText());
}

void FMonthlyOccPerc::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(150, "", tr("Month"))
            .setColumn(100, "", tr("Av. rooms"))
            .setColumn(100, "", tr("Occ. rooms"))
            .setColumn(100, "", tr("Occ. percent"));
    QStringList sql;
    sql << QString("call monthly_occ_perc(%1)").arg(ui->cbYear->currentText());
    sql << "select f_name, f_total, f_occ, f_perc from rep";
    rg->fModel->apply(sql);
}
