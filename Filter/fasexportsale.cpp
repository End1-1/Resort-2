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
    fTotalQuery = new ReportQuery("as_export2");
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FAsExportSale::branchEditDoubleClick);
    connect(ui->leHall, &EQLineEdit::customButtonClicked, this, &FAsExportSale::hallEditDoubleClick);
}

FAsExportSale::~FAsExportSale()
{
    delete ui;
}

void FAsExportSale::apply(WReportGrid *rg)
{
    ReportQuery *r = fReportQuery;
    if (ui->r2->isChecked()) {
        r = fTotalQuery;
    }
    QString query = r->query;
    query.replace(":date1", ui->wd->ds1(), Qt::CaseInsensitive).replace(":date2", ui->wd->ds2(), Qt::CaseInsensitive);
    if (ui->leBranch->fHiddenText.isEmpty()) {
        query.replace(":branch", "");
    } else {
        query.replace(":branch", " and oh.f_branch in (" + ui->leBranch->fHiddenText + ")");
    }
    if (ui->leHall->fHiddenText.isEmpty()) {
        query.replace(":hall", "");
    } else {
        query.replace(":hall", " and oh.f_hall in (" + ui->leHall->fHiddenText + ")");
    }
    query.replace(":tax", "");
    query.replace(":prepaid", "");
    rg->fModel->clearColumns();
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
//    QList<double> vals;
//    rg->fModel->sumOfColumns(fReportQuery->sumColumns, vals);
//    rg->setTblTotalData(fReportQuery->sumColumns, vals);
    for (QMap<int, int>::const_iterator it = r->columnsWidths.constBegin(); it != r->columnsWidths.constEnd(); it++) {
        rg->fTableView->setColumnWidth(it.key(), it.value());
    }
    if (r->sumColumns.isEmpty() == false) {
        QList<double> sums;
        rg->fModel->sumOfColumns(r->sumColumns, sums);
        rg->setTblTotalData(r->sumColumns, sums);
    } else {
        rg->setTblNoTotalData();
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

void FAsExportSale::hallEditDoubleClick(bool v)
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_hall, this)) {
        ui->leHall->setText(name);
        ui->leHall->fHiddenText = id;
    }
}
