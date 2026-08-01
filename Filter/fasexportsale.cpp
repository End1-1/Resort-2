#include "fasexportsale.h"
#include "ui_fasexportsale.h"
#include "reportquery.h"
#include "wreportgrid.h"
#include "dlggetidname.h"
#include "dlgasexportconstants.h"
#include "database2.h"
#include "doubledatabase.h"
#include "message.h"
#include <QDialog>
#include <QDir>
#include <QMap>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QSignalBlocker>
#include <QTemporaryFile>
#include <QTextStream>
#include <QToolButton>
#include <QStringConverter>

#ifndef _ORGANIZATION_
#define _ORGANIZATION_ "SmartHotel"
#endif
#ifndef _APPLICATION_
#define _APPLICATION_ "SmartHotel"
#endif

static const char *AS_EXPORT_SETTINGS_GROUP = "AsExport/Common1";

void AsExportConstants::load()
{
    QSettings s(_ORGANIZATION_, _APPLICATION_);
    s.beginGroup(AS_EXPORT_SETTINGS_GROUP);
    docNumberStart = s.value("docNumberStart", 1).toLongLong();
    buyer = s.value("buyer").toString();
    buyerAccount = s.value("buyerAccount").toString();
    prepayAccount = s.value("prepayAccount").toString();
    cashlessAccount = s.value("cashlessAccount").toString();
    cashlessAmount = s.value("cashlessAmount", QStringLiteral("0")).toString();
    prepayUsage = s.value("prepayUsage", QStringLiteral("*")).toString();
    comment = s.value("comment").toString();
    vatCalcMethod = s.value("vatCalcMethod", QStringLiteral("2")).toString();
    vatAccount = s.value("vatAccount").toString();
    issueMethod = s.value("issueMethod", QStringLiteral("*")).toString();
    docStatus = s.value("docStatus", QStringLiteral("1")).toString();
    vatLine = s.value("vatLine", QStringLiteral("1")).toString();
    transType = s.value("transType", QStringLiteral("1")).toString();
    ecoTax = s.value("ecoTax", QStringLiteral("0")).toString();
    expenseAccount = s.value("expenseAccount").toString();
    revenueAccount = s.value("revenueAccount").toString();
    s.endGroup();
}

void AsExportConstants::save() const
{
    QSettings s(_ORGANIZATION_, _APPLICATION_);
    s.beginGroup(AS_EXPORT_SETTINGS_GROUP);
    s.setValue("docNumberStart", docNumberStart);
    s.setValue("buyer", buyer);
    s.setValue("buyerAccount", buyerAccount);
    s.setValue("prepayAccount", prepayAccount);
    s.setValue("cashlessAccount", cashlessAccount);
    s.setValue("cashlessAmount", cashlessAmount);
    s.setValue("prepayUsage", prepayUsage);
    s.setValue("comment", comment);
    s.setValue("vatCalcMethod", vatCalcMethod);
    s.setValue("vatAccount", vatAccount);
    s.setValue("issueMethod", issueMethod);
    s.setValue("docStatus", docStatus);
    s.setValue("vatLine", vatLine);
    s.setValue("transType", transType);
    s.setValue("ecoTax", ecoTax);
    s.setValue("expenseAccount", expenseAccount);
    s.setValue("revenueAccount", revenueAccount);
    s.endGroup();
}

bool AsExportConstants::isValid(QString *errorMessage) const
{
    if(docNumberStart < 1) {
        if(errorMessage) {
            *errorMessage = QObject::tr("Document number start must be greater than zero");
        }
        return false;
    }

    if(buyer.trimmed().isEmpty()) {
        if(errorMessage) {
            *errorMessage = QObject::tr("Buyer code is required");
        }
        return false;
    }

    if(expenseAccount.trimmed().isEmpty()) {
        if(errorMessage) {
            *errorMessage = QObject::tr("Expense account is required");
        }
        return false;
    }

    if(revenueAccount.trimmed().isEmpty()) {
        if(errorMessage) {
            *errorMessage = QObject::tr("Revenue account is required");
        }
        return false;
    }

    return true;
}

namespace {
enum ReportType {
    REPORT_COMMON1 = 0,
    REPORT_IMPORT_RETAIL_INVOICE = 1
};

const char *COMMON1_DETAIL_QUERY =
    "SELECT oh.f_datecash, asm.f_as AS store_as, rd.f_as AS item_code, "
    "od.f_dish AS dish_id, rd.f_en AS dish_name, od.f_qty AS qty, od.f_price AS price "
    "FROM o_dish od "
    "LEFT JOIN o_header oh ON oh.f_id=od.f_header "
    "LEFT JOIN r_dish rd ON rd.f_id=od.f_dish "
    "LEFT JOIN r_store_as_map asm ON asm.f_store=od.f_store "
    "WHERE oh.f_state=2 AND od.f_state=1 "
    "AND oh.f_datecash BETWEEN :date1 AND :date2 "
    ":branch "
    ":hall "
    "ORDER BY oh.f_datecash, oh.f_id, od.f_id";

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

const QStringList ARM_SOFT_COLUMN_TITLES = {
    QString::fromUtf8("Ամսաթիվ"),
    QString::fromUtf8("Փաստաթղթի N"),
    QString::fromUtf8("Գնորդ"),
    QString::fromUtf8("Դրամարկղ"),
    QString::fromUtf8("Գնորդի հաշիվ"),
    QString::fromUtf8("Ստացված կանխավճարի հաշիվ"),
    QString::fromUtf8("Անկանխիկ գործարքների հաշիվ"),
    QString::fromUtf8("Անկանխիկ գումար"),
    QString::fromUtf8("Կանխավճարի օգտագործում"),
    QString::fromUtf8("Մեկնաբանություն"),
    QString::fromUtf8("ԱԱՀ-ի հաշվարկի ձև"),
    QString::fromUtf8("ԱԱՀ-ի հաշիվ"),
    QString::fromUtf8("Դուրս գրման եղանակ"),
    QString::fromUtf8("Փաստաթղթի վիճակ"),
    QString::fromUtf8("Պահեստ"),
    QString::fromUtf8("Կոդ"),
    QString::fromUtf8("Քանակ"),
    QString::fromUtf8("Գին"),
    QString::fromUtf8("Զեղչված գին"),
    QString::fromUtf8("Գումար"),
    QString::fromUtf8("ԱԱՀ"),
    QString::fromUtf8("Գործարքի տեսակ"),
    QString::fromUtf8("Բն. հրկ. գումար"),
    QString::fromUtf8("Ծախսի հաշիվ"),
    QString::fromUtf8("Հասույթի հաշիվ"),
    QString::fromUtf8("Սխալներ")
};

QString formatAmount(double value)
{
    QString text = QString::number(value, 'f', 2);
    while(text.contains('.') && (text.endsWith('0') || text.endsWith('.'))) {
        text.chop(1);
    }
    return text;
}

void openMissingItemCodesFile(const QMap<int, QString> &dishes)
{
    if(dishes.isEmpty()) {
        return;
    }

    QTemporaryFile tempFile(QDir::tempPath() + "/missing_as_codes_XXXXXX.txt");
    tempFile.setAutoRemove(false);

    if(!tempFile.open()) {
        return;
    }

    QTextStream out(&tempFile);
    out.setEncoding(QStringConverter::Utf8);
    out << "Dishes without ArmSoft code (r_dish.f_as):\r\n";
    out << "ID\tName\r\n";

    for(QMap<int, QString>::const_iterator it = dishes.constBegin(); it != dishes.constEnd(); ++it) {
        out << it.key() << '\t' << it.value() << "\r\n";
    }

    tempFile.close();

    QProcess::startDetached(
        "notepad.exe",
        QStringList() << QDir::toNativeSeparators(tempFile.fileName())
    );
}
}

FAsExportSale::FAsExportSale(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAsExportSale),
    fReportQuery(nullptr),
    fTotalQuery(nullptr),
    fHasSubType(false),
    fConfigButton(nullptr)
{
    ui->setupUi(this);
    fConstants.load();
    fReportGrid->setupTabTextAndIcon(tr("ArmSoft export"), ":/images/excel.png");
    fReportGrid->addToolBarButton(":/images/copy.png", tr("Copy"), SLOT(copyAllToClipboard()), fReportGrid)
        ->setFocusPolicy(Qt::NoFocus);
    fConfigButton = fReportGrid->addToolBarButton(":/images/update.png", tr("Config"), SLOT(configConstants()), this);
    fConfigButton->setFocusPolicy(Qt::NoFocus);
    fReportQuery = new ReportQuery("as_export");
    fTotalQuery = new ReportQuery("as_export2");
    initReportTypes();
    loadReportType();
    connect(ui->cbReportType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FAsExportSale::reportTypeChanged);
    connect(ui->wd, &WDate2::changed, [this]() {
        apply(fReportGrid);
    });
    connect(ui->r1, &QRadioButton::toggled, [this](bool) {
        if(ui->cbReportType->currentIndex() == REPORT_COMMON1) {
            apply(fReportGrid);
        }
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

    if(fConfigButton) {
        fConfigButton->setVisible(common1);
    }
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

void FAsExportSale::setupArmSoftColumns(WReportGrid *rg)
{
    rg->fModel->clearColumns();

    for(const QString &title : ARM_SOFT_COLUMN_TITLES) {
        rg->fModel->setColumn(80, "", title);
    }
}

void FAsExportSale::applyCommon1Detail(WReportGrid *rg)
{
    QString error;
    if(!fConstants.isValid(&error)) {
        message_error(error + "<br>" + tr("Open Config and fill required fields."));
        return;
    }

    QString query = COMMON1_DETAIL_QUERY;
    query.replace(":date1", ui->wd->ds1(), Qt::CaseInsensitive)
         .replace(":date2", ui->wd->ds2(), Qt::CaseInsensitive);

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

    Database2 db;
    if(!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }

    if(!db.exec(query)) {
        message_error(db.lastDbError());
        return;
    }

    QList<QList<QVariant> > rows;
    qint64 docNumber = fConstants.docNumberStart;
    int missingStore = 0;
    int missingItemCode = 0;
    QMap<int, QString> missingDishes;

    while(db.next()) {
        const QDate date = db.date("f_datecash");
        const QString storeAs = db.string("store_as");
        const QString itemCode = db.string("item_code");
        const int dishId = db.integer("dish_id");
        const QString dishName = db.string("dish_name");
        const double qty = db.doubleValue("qty");
        const double price = db.doubleValue("price");
        const double amount = qty * price;

        if(itemCode.trimmed().isEmpty()) {
            missingItemCode++;
            if(dishId > 0 && !missingDishes.contains(dishId)) {
                missingDishes.insert(dishId, dishName);
            }
            continue;
        }

        if(storeAs.trimmed().isEmpty()) {
            missingStore++;
        }

        QList<QVariant> row;
        row << date.toString("dd.MM.yyyy")
            << QString::number(docNumber++)
            << fConstants.buyer
            << storeAs
            << fConstants.buyerAccount
            << fConstants.prepayAccount
            << fConstants.cashlessAccount
            << fConstants.cashlessAmount
            << fConstants.prepayUsage
            << fConstants.comment
            << fConstants.vatCalcMethod
            << fConstants.vatAccount
            << fConstants.issueMethod
            << fConstants.docStatus
            << storeAs
            << itemCode
            << formatAmount(qty)
            << QString()
            << formatAmount(price)
            << formatAmount(amount)
            << fConstants.vatLine
            << fConstants.transType
            << fConstants.ecoTax
            << fConstants.expenseAccount
            << fConstants.revenueAccount
            << QString();

        rows.append(row);
    }

    if(missingItemCode > 0) {
        message_error(tr("%1 rows skipped: item code is empty").arg(missingItemCode));
        openMissingItemCodesFile(missingDishes);
    } else if(missingStore > 0) {
        message_error(tr("%1 rows have no store mapping (r_store_as_map.f_store = o_dish.f_store)").arg(missingStore));
    }

    setupArmSoftColumns(rg);
    rg->fModel->fDD.fDbRows = rows;
    rg->fModel->applyFinal(rg, true);
    rg->fTableView->resizeColumnsToContents();
    rg->setTblNoTotalData();
}

void FAsExportSale::applyCommon1(WReportGrid *rg)
{
    if(!fReportQuery || !fTotalQuery) {
        return;
    }

    if(ui->r1->isChecked()) {
        applyCommon1Detail(rg);
        return;
    }

    ReportQuery *r = fTotalQuery;
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

void FAsExportSale::configConstants()
{
    DlgAsExportConstants dlg(this);
    dlg.setValues(fConstants);

    if(dlg.exec() != QDialog::Accepted) {
        return;
    }

    dlg.fillValues(fConstants);
    fConstants.save();

    if(ui->cbReportType->currentIndex() == REPORT_COMMON1 && ui->r1->isChecked()) {
        apply(fReportGrid);
    }
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
