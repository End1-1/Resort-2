#include "fyearlyfinancialreport.h"
#include "ui_fyearlyfinancialreport.h"
#include "wreportgrid.h"

FYearlyFinancialReport::FYearlyFinancialReport(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FYearlyFinancialReport)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Yearly financial report"), ":/images/graph.png");
    ui->cbYear->setCurrentIndex(ui->cbYear->findText(QDate::currentDate().toString("yyyy")));
}

FYearlyFinancialReport::~FYearlyFinancialReport()
{
    delete ui;
}

QWidget *FYearlyFinancialReport::firstElement()
{
    return ui->cbYear;
}

QString FYearlyFinancialReport::reportTitle()
{
    return QString("%1 - %2")
            .arg(tr("Yearly financial report"))
            .arg(ui->cbYear->currentText());
}

void FYearlyFinancialReport::apply(WReportGrid *rg)
{
    rg->fModel->clearColumns();
    rg->fModel->setColumn(200, "", tr("Name"))
            .setColumn(80, "", tr("Jan"))
            .setColumn(80, "",  tr("Feb"))
            .setColumn(80, "", tr("Mar"))
            .setColumn(80, "", tr("Apr"))
            .setColumn(80, "", tr("May"))
            .setColumn(80, "", tr("Jun"))
            .setColumn(80, "", tr("Jul"))
            .setColumn(80, "", tr("Aug"))
            .setColumn(80, "", tr("Sep"))
            .setColumn(80, "", tr("Oct"))
            .setColumn(80, "", tr("Nov"))
            .setColumn(80, "", tr("Dec"))
            .setColumn(100, "", tr("Total"));
    QStringList queries;
    queries << QString("call yearly_financial_report(%1, %2)")
               .arg(ui->cbYear->currentText())
               .arg(ui->rbAmd->isChecked() ? 0 : 1);
    queries <<"select f_name, f_1, f_2, f_3, f_3, f_5, f_6,"
              "f_7, f_8, f_9, f_10, f_11, f_12, f_total "
              "from rep";
    rg->fModel->apply(queries);
    QList<int> cols;
    QList<double> vals;
    cols << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13;
    rg->fModel->sumOfColumns(cols, vals);
    rg->setTblTotalData(cols, vals);
}

void FYearlyFinancialReport::openReport()
{
    WReportGrid *rg = addTab<WReportGrid>();
    FYearlyFinancialReport *y = new FYearlyFinancialReport(rg);
    rg->addFilterWidget(y);
    y->apply(rg);
}
