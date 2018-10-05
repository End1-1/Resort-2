#include "fcommonfilterbydate.h"
#include "ui_fcommonfilterbydate.h"
#include "wreportgrid.h"

FCommonFilterByDate::FCommonFilterByDate(int sqlId, QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FCommonFilterByDate)
{
    ui->setupUi(this);

    fDbBind[":f_id"] = sqlId;
    fDb.select("select f_name, f_sql, f_widths, f_titles_" + def_lang + ", f_sum from serv_reports where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Cannot load report data. Contact to application developer.");
        return;
    }
    fReportGrid->setupTabTextAndIcon(fDbRows.at(0).at(0).toString(), ":/images/report.png");
    fTitle = fDbRows.at(0).at(0).toString();
    fQuery = fDbRows.at(0).at(1).toString();
    QStringList widths = fDbRows.at(0).at(2).toString().split(";", QString::SkipEmptyParts);
    QStringList titles = fDbRows.at(0).at(3).toString().split(";", QString::SkipEmptyParts);
    QStringList sums = fDbRows.at(0).at(4).toString().split(";", QString::SkipEmptyParts);
    for (int i = 0; i < widths.count(); i++) {
        fReportGrid->fModel->setColumn(widths.at(i).toInt(), "", titles.at(i));
    }
    foreach (QString s, sums) {
        fSumColumns << s.toInt();
    }
}

FCommonFilterByDate::~FCommonFilterByDate()
{
    delete ui;
}

QWidget *FCommonFilterByDate::firstElement()
{
    return ui->deFrom;
}

QString FCommonFilterByDate::reportTitle()
{
    return QString("%1 %2-%3").arg(fTitle).arg(ui->deFrom->text()).arg(ui->deTo->text());
}

void FCommonFilterByDate::apply(WReportGrid *rg)
{
    QString query = fQuery;
    query.replace(":f_date1", ui->deFrom->dateMySql()).replace(":f_date2", ui->deTo->dateMySql());
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    if (fSumColumns.count() > 0) {
        QList<double> vals;
        rg->fModel->sumOfColumns(fSumColumns, vals);
        rg->setTblTotalData(fSumColumns, vals);
    }
}

void FCommonFilterByDate::open(int id)
{
    WReportGrid *rg = addTab<WReportGrid>();
    FCommonFilterByDate *fd = new FCommonFilterByDate(id, rg);
    rg->addFilterWidget(fd);
    fd->apply(rg);
}
