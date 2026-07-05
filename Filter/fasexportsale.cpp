#include "fasexportsale.h"
#include "ui_fasexportsale.h"
#include "reportquery.h"
#include "wreportgrid.h"
#include "dlggetidname.h"
#include <QSignalBlocker>

namespace {
enum ReportType {
    REPORT_COMMON1 = 0,
    REPORT_IMPORT_RETAIL_INVOICE = 1
};

const char *IMPORT_RETAIL_INVOICE_QUERY =
    "SELECT asm.f_as, rd.f_as, od.f_qty, od.f_price "
    "FROM o_dish od "
    "LEFT JOIN o_header oh ON oh.f_id=od.f_header "
    "LEFT JOIN r_dish rd ON rd.f_id=od.f_dish "
    "LEFT JOIN r_store_as_map asm ON asm.f_store=od.f_store "
    "WHERE oh.f_state=2 AND od.f_state=1 "
    "AND oh.f_datecash BETWEEN :date1 AND :date2 "
    ":branch "
    ":store";
}

FAsExportSale::FAsExportSale(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAsExportSale),
    fReportQuery(nullptr),
    fTotalQuery(nullptr),
    fHasSubType(false)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("ArmSoft export"), ":/images/excel.png");
    fReportGrid->addToolBarButton(":/images/copy.png", tr("Copy"), SLOT(copyAllToClipboard()), fReportGrid)
        ->setFocusPolicy(Qt::NoFocus);
    fReportQuery = new ReportQuery("as_export");
    fTotalQuery = new ReportQuery("as_export2");
    initReportTypes();
    loadReportType();
    connect(ui->cbReportType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FAsExportSale::reportTypeChanged);
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FAsExportSale::branchEditDoubleClick);
    connect(ui->leHall, &EQLineEdit::customButtonClicked, this, &FAsExportSale::hallEditDoubleClick);
    connect(ui->leStore, &EQLineEdit::customButtonClicked, this, &FAsExportSale::storeEditDoubleClick);
}

FAsExportSale::~FAsExportSale()
{
    delete fReportQuery;
    delete fTotalQuery;
    delete ui;
}

void FAsExportSale::initReportTypes()
{
    QSignalBlocker blocker(ui->cbReportType);
    ui->cbReportType->clear();
    ui->cbReportType->addItem(tr("Common 1"));
    ui->cbReportType->addItem(tr("ImportRetailInvoice"));
}

void FAsExportSale::loadReportType()
{
    switch(ui->cbReportType->currentIndex()) {
    case REPORT_COMMON1:
        fReportQuery->loadData("as_export");
        fTotalQuery->loadData("as_export2");
        fHasSubType = true;
        break;
    case REPORT_IMPORT_RETAIL_INVOICE:
        fHasSubType = false;
        break;
    default:
        return;
    }

    updateFilterVisibility();
}

void FAsExportSale::updateFilterVisibility()
{
    const bool common1 = ui->cbReportType->currentIndex() == REPORT_COMMON1;
    const bool importInvoice = ui->cbReportType->currentIndex() == REPORT_IMPORT_RETAIL_INVOICE;

    ui->label_2->setVisible(common1);
    ui->leHall->setVisible(common1);
    ui->r1->setVisible(common1 && fHasSubType);
    ui->r2->setVisible(common1 && fHasSubType);
    ui->label_4->setVisible(importInvoice);
    ui->leStore->setVisible(importInvoice);
}

void FAsExportSale::reportTypeChanged(int index)
{
    if(index < 0) {
        return;
    }

    loadReportType();
    apply(fReportGrid);
}

void FAsExportSale::apply(WReportGrid *rg)
{
    switch(ui->cbReportType->currentIndex()) {
    case REPORT_COMMON1:
        applyCommon1(rg);
        break;
    case REPORT_IMPORT_RETAIL_INVOICE:
        applyImportRetailInvoice(rg);
        break;
    default:
        break;
    }
}

void FAsExportSale::applyCommon1(WReportGrid *rg)
{
    if(!fReportQuery || !fTotalQuery) {
        return;
    }

    ReportQuery *r = fReportQuery;

    if(ui->r2->isChecked()) {
        r = fTotalQuery;
    }

    QString query = r->query;

    if(query.isEmpty()) {
        message_error(tr("Report query is not configured"));
        return;
    }

    query.replace(":date1", ui->wd->ds1(), Qt::CaseInsensitive).replace(":date2", ui->wd->ds2(), Qt::CaseInsensitive);

    if(ui->leBranch->fHiddenText.isEmpty()) {
        query.replace(":branch", "");
    } else {
        query.replace(":branch", " and oh.f_branch in (" + ui->leBranch->fHiddenText + ")");
    }

    if(ui->leHall->fHiddenText.isEmpty()) {
        query.replace(":hall", "");
    } else {
        query.replace(":hall", " and oh.f_hall in (" + ui->leHall->fHiddenText + ")");
    }

    query.replace(":tax", "");
    query.replace(":prepaid", "");
    rg->fModel->clearColumns();
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);

    if(r->columnsWidths.isEmpty()) {
        rg->fTableView->resizeColumnsToContents();
    } else {
        for(QMap<int, int>::const_iterator it = r->columnsWidths.constBegin(); it != r->columnsWidths.constEnd(); it++) {
            rg->fTableView->setColumnWidth(it.key(), it.value());
        }
    }

    if(r->sumColumns.isEmpty() == false) {
        QList<double> sums;
        rg->fModel->sumOfColumns(r->sumColumns, sums);
        rg->setTblTotalData(r->sumColumns, sums);
    } else {
        rg->setTblNoTotalData();
    }
}

void FAsExportSale::applyImportRetailInvoice(WReportGrid *rg)
{
    if(ui->leBranch->fHiddenText.isEmpty()) {
        message_error(tr("Branch is required"));
        return;
    }

    QString query = IMPORT_RETAIL_INVOICE_QUERY;
    query.replace(":date1", ui->wd->ds1(), Qt::CaseInsensitive)
         .replace(":date2", ui->wd->ds2(), Qt::CaseInsensitive);
    query.replace(":branch", " and oh.f_branch in (" + ui->leBranch->fHiddenText + ")");

    if(ui->leStore->fHiddenText.isEmpty()) {
        query.replace(":store", "");
    } else {
        query.replace(":store", " and od.f_store in (" + ui->leStore->fHiddenText + ")");
    }

    rg->fModel->clearColumns();
    rg->fModel->setSqlQuery(query);
    rg->fModel->apply(rg);
    rg->fTableView->resizeColumnsToContents();

    QList<int> sumCols;
    sumCols << 2 << 3;
    QList<double> sums;
    rg->fModel->sumOfColumns(sumCols, sums);
    rg->setTblTotalData(sumCols, sums);
}

QWidget *FAsExportSale::firstElement()
{
    return ui->wd->fw();
}

void FAsExportSale::branchEditDoubleClick(bool v)
{
    Q_UNUSED(v)
    QString id, name;

    if(DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}

void FAsExportSale::hallEditDoubleClick(bool v)
{
    Q_UNUSED(v)
    QString id, name;

    if(DlgGetIDName::get(id, name, idname_hall, this)) {
        ui->leHall->setText(name);
        ui->leHall->fHiddenText = id;
    }
}

void FAsExportSale::storeEditDoubleClick(bool v)
{
    Q_UNUSED(v)
    QString id, name;

    if(DlgGetIDName::get(id, name, idname_store, this)) {
        ui->leStore->setText(name);
        ui->leStore->fHiddenText = id;
    }
}
