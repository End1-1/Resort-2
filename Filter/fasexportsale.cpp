#include "fasexportsale.h"
#include "ui_fasexportsale.h"
#include "reportquery.h"
#include "wreportgrid.h"
#include "dlggetidname.h"

FAsExportSale::FAsExportSale(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAsExportSale)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("ArmSoft export"), ":/images/excel.png");
    fReportQuery = new ReportQuery("as_export");
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FAsExportSale::branchEditDoubleClick);
}

FAsExportSale::~FAsExportSale()
{
    delete ui;
}

void FAsExportSale::apply(WReportGrid *rg)
{
    QString query = fReportQuery->query;
    query.replace(":date1", ui->wd->ds1(), Qt::CaseInsensitive).replace(":date2", ui->wd->ds2(), Qt::CaseInsensitive);
    if (ui->leBranch->fHiddenText.isEmpty()) {
        query.replace(":branch", "");
    } else {
        query.replace(":branch", " and oh.f_branch in (" + ui->leBranch->fHiddenText + ")");
    }
    rg->fModel->clearColumns();
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
//    QList<double> vals;
//    rg->fModel->sumOfColumns(fReportQuery->sumColumns, vals);
//    rg->setTblTotalData(fReportQuery->sumColumns, vals);
    for (QMap<int, int>::const_iterator it = fReportQuery->columnsWidths.constBegin(); it != fReportQuery->columnsWidths.constEnd(); it++) {
        rg->fTableView->setColumnWidth(it.key(), it.value());
    }
}

QWidget *FAsExportSale::firstElement()
{
    return ui->wd->fw();
}

void FAsExportSale::branchEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}
