#include "frestauranttotal.h"
#include "ui_frestauranttotal.h"
#include "wreportgrid.h"
#include "dwselectordish.h"
#include "dwselectordishstate.h"
#include "dlgperemovereason.h"
#include "dlggposorderinfo.h"
#include "baseorder.h"
#include "storeoutput.h"
#include "paymentmode.h"
#include "cacheresthall.h"
#include "recalculatestoreoutputs.h"
#ifdef RESORT_AUDIT_LOG
#include "resortlog.h"
#include <QJsonObject>
#include <QUuid>
#endif
#include "cacheusers.h"
#include "cacheresttable.h"
#include "cachepaymentmode.h"
#include "dlggetidname.h"
#include "message.h"
#include "eqcheckbox.h"
#include "trackcontrol.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include <QDate>
#include <QFileDialog>
#include <QPrinter>
#include <QSet>
#include <QSqlQuery>
#include <algorithm>

namespace {
constexpr int portalHeaderRow = 1;
constexpr int portalDataStartRow = 2;

const QString portalHeaderDate = QStringLiteral("ամսաթիվ");
const QString portalHeaderFiscal = QStringLiteral("հդմ կտրոնի համար");
const QString portalHeaderService = QStringLiteral("ծառայություն");
const QString portalHeaderAmount = QStringLiteral("գումար");

struct PortalColumns {
    int dateTime = -1;
    int fiscal = -1;
    int service = -1;
    int amount = -1;
};

QString normalizeFiscalNumber(const QString &raw)
{
    const QString trimmed = raw.trimmed();

    if(trimmed.isEmpty() || trimmed.compare("null", Qt::CaseInsensitive) == 0) {
        return QString();
    }

    bool ok = false;
    const qint64 value = trimmed.toLongLong(&ok);

    if(ok && value > 0) {
        return QString::number(value);
    }

    const double asDouble = trimmed.toDouble(&ok);

    if(ok && asDouble > 0 && qAbs(asDouble - qRound(asDouble)) < 0.0001) {
        return QString::number(static_cast<qint64>(qRound(asDouble)));
    }

    return QString();
}

QString normalizeFiscalNumber(const QVariant &value)
{
    if(!value.isValid() || value.isNull()) {
        return QString();
    }

    switch(value.typeId()) {
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::UInt:
    case QMetaType::ULongLong: {
        const qint64 num = value.toLongLong();

        if(num <= 0) {
            return QString();
        }

        return QString::number(num);
    }
    case QMetaType::Double:
    case QMetaType::Float: {
        const double num = value.toDouble();

        if(num <= 0 || qIsNaN(num)) {
            return QString();
        }

        return QString::number(static_cast<qint64>(qRound(num)));
    }
    default:
        return normalizeFiscalNumber(value.toString());
    }
}

QVariant readPortalCellVariant(QXlsx::Document &doc, int row, int col)
{
    if(const std::shared_ptr<QXlsx::Cell> cell = doc.cellAt(row, col)) {
        return cell->value();
    }

    return QVariant();
}

QString formatPortalDateValue(const QVariant &value)
{
    if(!value.isValid() || value.isNull()) {
        return QString();
    }

    if(value.userType() == QMetaType::QDateTime) {
        return value.toDateTime().toString("dd.MM.yyyy hh:mm:ss");
    }

    if(value.userType() == QMetaType::QDate) {
        return value.toDate().toString("dd.MM.yyyy");
    }

    if(value.typeId() == QMetaType::Double || value.typeId() == QMetaType::Int
            || value.typeId() == QMetaType::LongLong) {
        const double num = value.toDouble();

        if(num >= 30000 && num < 70000) {
            return QDate(1899, 12, 30).addDays(static_cast<int>(num)).toString("dd.MM.yyyy");
        }
    }

    return value.toString().trimmed();
}

QString formatPortalTextValue(const QVariant &value)
{
    if(!value.isValid() || value.isNull()) {
        return QString();
    }

    if(value.typeId() == QMetaType::Double) {
        const double num = value.toDouble();

        if(qAbs(num - qRound(num)) < 0.0001) {
            return QString::number(static_cast<qint64>(qRound(num)));
        }
    }

    return value.toString().trimmed();
}

bool findPortalColumns(QXlsx::Document &doc, PortalColumns &cols, QString &error)
{
    bool hasHeaders = false;

    for(int col = 1; col <= 256; ++col) {
        const QString header = readPortalCellVariant(doc, portalHeaderRow, col).toString().trimmed();

        if(header.isEmpty()) {
            continue;
        }

        hasHeaders = true;

        if(header == portalHeaderDate) {
            cols.dateTime = col;
        } else if(header == portalHeaderFiscal) {
            cols.fiscal = col;
        } else if(header == portalHeaderService) {
            cols.service = col;
        } else if(header == portalHeaderAmount) {
            cols.amount = col;
        }
    }

    if(!hasHeaders) {
        error = QObject::tr("Excel file has no header row");
        return false;
    }

    if(cols.fiscal < 0) {
        error = QObject::tr("Required column \"%1\" not found in Excel file").arg(portalHeaderFiscal);
        return false;
    }

    return true;
}

bool readPortalFiscals(const QString &filePath,
                       QMap<QString, QMap<QString, QString>> &byFiscal,
                       QString &error)
{
    QXlsx::Document doc(filePath);

    if(doc.sheetNames().isEmpty()) {
        error = QObject::tr("Cannot open Excel file");
        return false;
    }

    PortalColumns cols;

    if(!findPortalColumns(doc, cols, error)) {
        return false;
    }

    int emptyRows = 0;

    for(int row = portalDataStartRow; row < 200000; ++row) {
        const QVariant fiscalValue = readPortalCellVariant(doc, row, cols.fiscal);
        const QString fiscal = normalizeFiscalNumber(fiscalValue);
        const QString dateTime = cols.dateTime > 0
                                 ? formatPortalDateValue(readPortalCellVariant(doc, row, cols.dateTime))
                                 : QString();
        const QString service = cols.service > 0
                                ? formatPortalTextValue(readPortalCellVariant(doc, row, cols.service))
                                : QString();
        const QString amount = cols.amount > 0
                               ? formatPortalTextValue(readPortalCellVariant(doc, row, cols.amount))
                               : QString();

        if(fiscal.isEmpty()) {
            if(dateTime.isEmpty() && service.isEmpty() && amount.isEmpty()) {
                if(++emptyRows >= 5) {
                    break;
                }
            }

            continue;
        }

        emptyRows = 0;

        if(!byFiscal.contains(fiscal)) {
            QMap<QString, QString> info;
            info["receipt"] = fiscal;
            info["datetime"] = dateTime;
            info["service"] = service;
            info["amount"] = amount;
            byFiscal[fiscal] = info;
        }
    }

    if(byFiscal.isEmpty()) {
        error = QObject::tr("No fiscal numbers found in Excel file");
        return false;
    }

    return true;
}

}

namespace {

bool ensureDatabaseReady(Database &db)
{
    if(!db.fDb.isOpen() && !db.open()) {
        message_error(db.fLastError);
        return false;
    }

    QSqlQuery ping(db.fDb);

    if(!ping.exec(QStringLiteral("SELECT 1"))) {
        db.fDb.close();

        if(!db.open()) {
            message_error(db.fLastError);
            return false;
        }
    }

    return true;
}

bool execSql(Database &db,
             const QString &sql,
             QMap<QString, QVariant> &bind,
             QList<QList<QVariant>> &rows)
{
    if(db.select(sql, bind, rows) < 0) {
        message_error(db.fLastError);
        return false;
    }

    return true;
}

void abortRemoveTransaction(Database &db)
{
    if(db.fDb.isOpen()) {
        db.fDb.rollback();
    }
}

} // namespace

#define sn_order_state 1
#define sn_table 2
#define sn_dish 3
#define sn_dish_state 4

FRestaurantTotal::FRestaurantTotal(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FRestaurantTotal)
{
    ui->setupUi(this);

    if(check_permission(pr_remove_order)) {
        fReportGrid->addToolBarButton(":/images/garbage.png", tr("Remove"), SLOT(removeOrder()),
                                      this)->setFocusPolicy(Qt::ClickFocus);
    }

    if(check_permission(pr_make_store_output_of_sale)) {
        fReportGrid->addToolBarButton(":/images/puzzle.png", tr("Recalculate store"), SLOT(recalculateStore()),
                                      this)->setFocusPolicy(Qt::ClickFocus);
    }

    //fReportGrid->addToolBarButton(":/images/printer.png", tr("Print receipts"), SLOT(printReceipt()), this)->setFocusPolicy(Qt::ClickFocus);
    fReportGrid->addToolBarButton(":/images/excel.png", tr("Compare fiscal info"), SLOT(compareFiscalInfo()),
                                  this)->setFocusPolicy(Qt::ClickFocus);
    connect(fReportGrid, SIGNAL(doubleClickOnRow(QList<QVariant>)), this, SLOT(doubleClick(QList<QVariant>)));
    fDockHall = new DWSelectorHall(this);
    fDockHall->configure();
    fDockHall->setSelector(ui->leHall);
    connect(fDockHall, SIGNAL(hall(CI_RestHall*)), this, SLOT(hall(CI_RestHall*)));
    fDockOrderState = new DWSelectorOrderState(this);
    fDockOrderState->configure();
    fDockOrderState->setSelector(ui->leState);
    fDockOrderState->setDialog(this, sn_order_state);
    DWSelectorRestTable *dwTable = new DWSelectorRestTable(this);
    dwTable->configure();
    dwTable->setSelector(ui->leTable);
    dwTable->setDialog(this, sn_table);
    fDockUsers = new DWSelectorUsers(this);
    fDockUsers->configure();
    fDockUsers->setSelector(ui->leStaff);
    connect(fDockUsers, SIGNAL(user(CI_User*)), this, SLOT(user(CI_User*)));
    fDockStore = new DWSelectorRestStore(this);
    fDockStore->configure();
    fDockStore->setSelector(ui->leStore);
    connect(fDockStore, SIGNAL(store(CI_RestStore*)), this, SLOT(store(CI_RestStore*)));
    fDockDishType = new DWSelectorDishType(this);
    fDockDishType->configure();
    fDockDishType->setSelector(ui->leDishType);
    connect(fDockDishType, SIGNAL(dishType(CI_RestDishType*)), this, SLOT(dishType(CI_RestDishType*)));
    DWSelectorDish *dwDish = new DWSelectorDish(this);
    dwDish->configure();
    dwDish->setSelector(ui->leDish);
    dwDish->setDialog(this, sn_dish);
    DWSelectorDishState *dwDishState = new DWSelectorDishState(this);
    dwDishState->configure();
    dwDishState->setSelector(ui->leDishState);
    dwDishState->setDialog(this, sn_dish_state);
    CI_OrderState *os = CacheOrderState::instance()->get(ORDER_STATE_CLOSED);

    if(os) {
        selector(sn_order_state, QVariant::fromValue(os));
    }

    CI_DishState *ds = CacheDishState::instance()->get(DISH_STATE_READY);

    if(ds) {
        selector(sn_dish_state, QVariant::fromValue(ds));
    }

    connect(ui->leBranch, &EQLineEdit::customButtonClicked, this, &FRestaurantTotal::branchEditDoubleClick);
    fReportGrid->fIncludes.clear();
    fReportGrid->fIncludes["oh.f_id"] = false;
    fReportGrid->fIncludes["oh.f_state"] = false;
    fReportGrid->fIncludes["os.f_" + def_lang] = false;
    fReportGrid->fIncludes["oh.f_datecash"] = false;
    fReportGrid->fIncludes["oh.f_dateopen"] = false;
    fReportGrid->fIncludes["oh.f_dateclose"] = false;
    fReportGrid->fIncludes["oh.f_hall"] = false;
    fReportGrid->fIncludes["h.f_name"] = false;
    fReportGrid->fIncludes["oh.f_table"] = false;
    fReportGrid->fIncludes["t.f_name"] = false;
    fReportGrid->fIncludes["oh.f_staff"] = false;
    fReportGrid->fIncludes["concat(u.f_firstname,' ',u.f_lastname)"] = false;
    fReportGrid->fIncludes["oh.f_cityledger"] = false;
    fReportGrid->fIncludes["cl.f_name"] = false;
    fReportGrid->fIncludes["od.f_store"] = false;
    fReportGrid->fIncludes["s.f_name"] = false;
    fReportGrid->fIncludes["od.f_state"] = false;
    fReportGrid->fIncludes["ds.f_en"] = false;
    fReportGrid->fIncludes["d.f_type"] = false;
    fReportGrid->fIncludes["dt.f_" + def_lang] = false;
    fReportGrid->fIncludes["d.f_as"] = false;
    fReportGrid->fIncludes["od.f_dish"] = false;
    fReportGrid->fIncludes["d.f_" + def_lang] = false;
    fReportGrid->fIncludes["od.f_price"] = false;
    fReportGrid->fIncludes["oh.f_tax"] = false;
    fReportGrid->fIncludes["oh.f_paymentMode"] = false;
    fReportGrid->fIncludes["pm.f_" + def_lang] = false;
    fReportGrid->fIncludes["oc.f_govnumber"] = false;
    fReportGrid->fIncludes["oh.f_comment"] = false;
    fReportGrid->fIncludes["sum(od.f_qty)"] = true;
    fReportGrid->fIncludes["sum(od.f_total)"] = true;
    fReportGrid->fIncludes["count(oh.f_id)"] = false;
    fReportGrid->fIncludes["sum(oh.f_total)"] = false;
    fReportGrid->fIncludes["op.f_discountcard"] = false;
    fReportGrid->fIncludes["sum(op.f_cash)"] = false;
    fReportGrid->fIncludes["sum(op.f_card)"] = false;
    fReportGrid->fIncludes["sum(op.f_idram)"] = false;
    fReportGrid->fIncludes["sum(op.f_prepaid)"] = false;
    fReportGrid->fIncludes["sum(op.f_discount)"] = false;
    fReportGrid->fIncludes["sum(op.f_debt)"] = false;
    fReportGrid->fIncludes["sum(op.f_coupon)"] = false;
    fReportGrid->fIncludes["sum(op.f_couponbank)"] = false;
    fReportGrid->fIncludes["sum(op.f_couponservice)"] = false;
    fReportGrid->fIncludes["tl.f_special"] = false;
    fReportGrid->fIncludes["tl.f_partner"] = false;
    fReportGrid->fIncludes["tp.f_name"] = false;
    fReportGrid->fIncludes["od.f_fiscal"] = false;
    fReportGrid->fIncludes["br.f_name"] = false;
    fReportGrid->fIncludes["d.f_as"] = false;
}

FRestaurantTotal::~FRestaurantTotal()
{
    delete ui;
}

QString FRestaurantTotal::resolveAggregateIncludeField(const QString &field)
{
    if(field == QLatin1String("oh.f_total")) {
        return QString();
    }

    if(field == QLatin1String("op.f_cash")) {
        return QStringLiteral("sum(op.f_cash)");
    }

    if(field == QLatin1String("op.f_card")) {
        return QStringLiteral("sum(op.f_card)");
    }

    if(field == QLatin1String("op.f_idram")) {
        return QStringLiteral("sum(op.f_idram)");
    }

    if(field == QLatin1String("op.f_prepaid")) {
        return QStringLiteral("sum(op.f_prepaid)");
    }

    if(field == QLatin1String("op.f_discount")) {
        return QStringLiteral("sum(op.f_discount)");
    }

    if(field == QLatin1String("op.f_debt")) {
        return QStringLiteral("sum(op.f_debt)");
    }

    if(field == QLatin1String("op.f_coupon")) {
        return QStringLiteral("sum(op.f_coupon)");
    }

    if(field == QLatin1String("op.f_couponbank")) {
        return QStringLiteral("sum(op.f_couponbank)");
    }

    if(field == QLatin1String("op.f_couponservice")) {
        return QStringLiteral("sum(op.f_couponservice)");
    }

    return field;
}

void FRestaurantTotal::syncColumnIncludesFromCheckboxes(bool countAmount)
{
    static const QStringList aggregateFields = {
        QStringLiteral("count(oh.f_id)"),
        QStringLiteral("sum(oh.f_total)"),
        QStringLiteral("sum(od.f_qty)"),
        QStringLiteral("sum(od.f_total)"),
        QStringLiteral("sum(op.f_cash)"),
        QStringLiteral("sum(op.f_card)"),
        QStringLiteral("sum(op.f_idram)"),
        QStringLiteral("sum(op.f_prepaid)"),
        QStringLiteral("sum(op.f_discount)"),
        QStringLiteral("sum(op.f_debt)"),
        QStringLiteral("sum(op.f_coupon)"),
        QStringLiteral("sum(op.f_couponbank)"),
        QStringLiteral("sum(op.f_couponservice)")
    };

    for(const QString &field : aggregateFields) {
        fReportGrid->fIncludes[field] = false;
    }

    const QObjectList ol = children();

    for(QObject *o : ol) {
        QWidget *w = qobject_cast<QWidget *>(o);

        if(!w || !isCheckBox(w)) {
            continue;
        }

        EQCheckBox *check = static_cast<EQCheckBox *>(w);
        const QString name = check->objectName();

        if(name == QLatin1String("chOnlyZeroes") || name == QLatin1String("chCouponOfService")) {
            continue;
        }

        const QString fieldSpec = check->getField().trimmed();

        if(fieldSpec.isEmpty()) {
            continue;
        }

        const QStringList groupFields = fieldSpec.split(QLatin1Char(';'), Qt::SkipEmptyParts);

        for(QString s : groupFields) {
            if(check->getRequireLang()) {
                s += def_lang;
            }

            QString includeField = s;

            if(countAmount) {
                const QString mapped = resolveAggregateIncludeField(s);

                if(mapped.isEmpty()) {
                    continue;
                }

                includeField = mapped;
            }

            fReportGrid->fIncludes[includeField] = check->isChecked();
        }
    }
}

void FRestaurantTotal::groupCheckClicked(bool value)
{
    Q_UNUSED(value);

    EQCheckBox *check = qobject_cast<EQCheckBox *>(sender());

    if(check && check->objectName() == QLatin1String("chShowDiscount") && check->isChecked()) {
        ui->chOrderNum->setChecked(true);
    }
}

void FRestaurantTotal::apply(WReportGrid *rg)
{
    QString order;
    bool countAmount = !ui->chDish->isChecked()
                       && !ui->rb500No->isChecked()
                       && !ui->rb500Yes->isChecked()
                       && !ui->chStore->isChecked()
                       && !ui->chDishType->isChecked()
                       && !ui->chDishState->isChecked()
                       && ui->leDish->fHiddenText.isEmpty()
                       && ui->leStore->text().isEmpty()
                       && ui->leDishType->text().isEmpty();;

    if(ui->chPaymentMode->isChecked()) {
        countAmount = true;
    }

    syncColumnIncludesFromCheckboxes(countAmount);

    fReportGrid->fIncludes["sum(od.f_total)"] = !countAmount;
    fReportGrid->fIncludes["sum(od.f_qty)"] = !countAmount;
    fReportGrid->fIncludes["count(oh.f_id)"] = countAmount;
    fReportGrid->fIncludes["sum(oh.f_total)"] = countAmount;

    if(countAmount && ui->chPaymentMode->isChecked()) {
        fReportGrid->fIncludes["sum(oh.f_total)"] = true;
        fReportGrid->fIncludes["sum(op.f_cash)"] = true;
        fReportGrid->fIncludes["sum(op.f_card)"] = true;
        fReportGrid->fIncludes["sum(op.f_idram)"] = true;
        fReportGrid->fIncludes["sum(op.f_prepaid)"] = true;
        fReportGrid->fIncludes["sum(op.f_discount)"] = true;
        fReportGrid->fIncludes["sum(op.f_debt)"] = true;
        fReportGrid->fIncludes["sum(op.f_coupon)"] = true;
        fReportGrid->fIncludes["sum(op.f_couponbank)"] = true;
        fReportGrid->fIncludes["sum(op.f_couponservice)"] = true;
    }

    rg->fFieldsWidths.clear();
    rg->fFieldsWidths[tr("Order #")] = 100;
    rg->fFieldsWidths[tr("State code")] = 0;
    rg->fFieldsWidths[tr("State")] = 80;
    rg->fFieldsWidths[tr("Date")] = 100;
    rg->fFieldsWidths[tr("Opened")] = 120;
    rg->fFieldsWidths[tr("Closed")] = 120;
    rg->fFieldsWidths[tr("Hall code")] = 0;
    rg->fFieldsWidths[tr("Hall")] = 100;
    rg->fFieldsWidths[tr("Table code")] = 0;
    rg->fFieldsWidths[tr("Table")] = 50;
    rg->fFieldsWidths[tr("Staff code")] = 0;
    rg->fFieldsWidths[tr("Staff")] = 150;
    rg->fFieldsWidths[tr("City ledger code")] = 0;
    rg->fFieldsWidths[tr("City ledger")] = 150;
    rg->fFieldsWidths[tr("Store code")] = 0;
    rg->fFieldsWidths[tr("Store")] = 100;
    rg->fFieldsWidths[tr("Dish state code")] = 0;
    rg->fFieldsWidths[tr("Dish state")] = 100;
    rg->fFieldsWidths[tr("Dish type code")] = 0;
    rg->fFieldsWidths[tr("Dish type")] = 150;
    rg->fFieldsWidths[tr("Dish code")] = 80;
    rg->fFieldsWidths[tr("Dish")] = 200;
    rg->fFieldsWidths[tr("Price")] = 80;
    rg->fFieldsWidths[tr("Lisence plate")] = 80;
    rg->fFieldsWidths[tr("Tax")] = 80;
    rg->fFieldsWidths[tr("Payment mode code")] = 0;
    rg->fFieldsWidths[tr("P/M")] = 150;
    rg->fFieldsWidths["Մեկնաբանություն"] = 100;
    rg->fFieldsWidths["Կանխավճար"] = 100;
    rg->fFieldsWidths[tr("Discount card")] = 100;
    rg->fFieldsWidths[tr("Discount amount")] = 80;
    rg->fFieldsWidths[tr("Qty")] = 80;
    rg->fFieldsWidths[tr("Total")] = 80;
    rg->fFieldsWidths[tr("Cash")] = 80;
    rg->fFieldsWidths[tr("Card")] = 80;
    rg->fFieldsWidths["Idram"] = 80;
    rg->fFieldsWidths["Փոխանցում"] = 80;
    rg->fFieldsWidths[tr("Discount")] = 80;
    rg->fFieldsWidths["Նվեր քարտ"] = 80;
    rg->fFieldsWidths["Նվեր փոխանցում"] = 80;
    rg->fFieldsWidths["Ավտոկտրոն"] = 80;
    rg->fFieldsWidths["Հատուկ"] = 80;
    rg->fFieldsWidths[tr("Talon partner code")] = 0;
    rg->fFieldsWidths[tr("Talon partner")] = 150;
    rg->fFields.clear();
    rg->fFields << "oh.f_id"
                << "oh.f_state"
                << "os.f_" + def_lang << "oh.f_datecash"
                << "oh.f_dateopen"
                << "oh.f_dateclose"
                << "br.f_name"
                << "oh.f_hall"
                << "h.f_name"
                << "oh.f_table"
                << "t.f_name"
                << "oh.f_staff"
                << "concat(u.f_firstname,' ',u.f_lastname)"
                << "oh.f_cityledger"
                << "cl.f_name"
                << "od.f_store"
                << "od.f_state"
                << "s.f_name"
                << "ds.f_en"
                << "d.f_type"
                << "dt.f_" + def_lang << "od.f_dish"
                << "d.f_" + def_lang << "od.f_price"
                << "d.f_as"
                << "od.f_fiscal"
                << "oh.f_paymentMode"
                << "pm.f_" + def_lang << "oc.f_govnumber"
                << "oh.f_comment"
                << "op.f_discountcard"
                << "tl.f_special"
                << "tl.f_partner"
                << "tp.f_name";

    if(countAmount) {
        if(!ui->chPaymentMode->isChecked()) {
            rg->fFields
                    << "count(oh.f_id)"
                    << "sum(oh.f_total)";
        } else {
            rg->fFields << "count(oh.f_id)"
                        << "sum(oh.f_total)"
                        << "sum(op.f_cash)"
                        << "sum(op.f_card)"
                        << "sum(op.f_idram)"
                        << "sum(op.f_prepaid)"
                        << "sum(op.f_debt)"
                        << "sum(op.f_discount)"
                        << "sum(op.f_coupon)"
                        << "sum(op.f_couponbank)"
                        << "sum(op.f_couponservice)";
        }
    } else {
        rg->fFields
                << "sum(od.f_qty)"
                << "sum(od.f_total)";
    }

    rg->fFieldTitles.clear();
    rg->fFieldTitles["oh.f_id"] = tr("Order #");
    rg->fFieldTitles["oh.f_state"] = tr("State code");
    rg->fFieldTitles["os.f_" + def_lang] = tr("State");
    rg->fFieldTitles["oh.f_datecash"] = tr("Date");
    rg->fFieldTitles["oh.f_dateopen"] = tr("Opened");
    rg->fFieldTitles["oh.f_dateclose"] = tr("Closed");
    rg->fFieldTitles["oh.f_hall"] = tr("Hall code");
    rg->fFieldTitles["h.f_name"] = tr("Hall");
    rg->fFieldTitles["br.f_name"] = tr("Branch");
    rg->fFieldTitles["oh.f_table"] = tr("Table code");
    rg->fFieldTitles["t.f_name"] = tr("Table");
    rg->fFieldTitles["oh.f_staff"] = tr("Staff code");
    rg->fFieldTitles["concat(u.f_firstname,' ',u.f_lastname)"] = tr("Staff");
    rg->fFieldTitles["oh.f_cityledger"] = tr("City ledger code");
    rg->fFieldTitles["cl.f_name"] = tr("City ledger");
    rg->fFieldTitles["od.f_store"] = tr("Store code");
    rg->fFieldTitles["s.f_name"] = tr("Store");
    rg->fFieldTitles["od.f_state"] = tr("Dish state code");
    rg->fFieldTitles["ds.f_en"] = tr("Dish state");
    rg->fFieldTitles["d.f_type"] = tr("Dish type code");
    rg->fFieldTitles["dt.f_" + def_lang] = tr("Dish type");
    rg->fFieldTitles["od.f_dish"] = tr("Dish code");
    rg->fFieldTitles["d.f_" + def_lang] = tr("Dish");
    rg->fFieldTitles["od.f_price"] = tr("Price");
    rg->fFieldTitles["d.f_as"] = tr("ArmSoft");
    rg->fFieldTitles["od.f_fiscal"] = tr("Tax");
    rg->fFieldTitles["oh.f_paymentMode"] = tr("Payment mode code");
    rg->fFieldTitles["pm.f_" + def_lang] = tr("P/M");
    rg->fFieldTitles["oh.f_comment"] = "Մեկնաբանություն";
    rg->fFieldTitles["oc.f_govnumber"] = tr("Lisence plate");
    rg->fFieldTitles["op.f_discountcard"] = tr("Discount card");
    rg->fFieldTitles["sum(op.f_discount)"] = tr("Discount amount");
    rg->fFieldTitles["count(oh.f_id)"] = tr("Qty");
    rg->fFieldTitles["sum(oh.f_total)"] = tr("Total");
    rg->fFieldTitles["sum(od.f_qty)"] = tr("Qty");
    rg->fFieldTitles["sum(od.f_total)"] = tr("Total");
    rg->fFieldTitles["sum(op.f_cash)"] = tr("Cash");
    rg->fFieldTitles["sum(op.f_card)"] = tr("Card");
    rg->fFieldTitles["sum(op.f_idram)"] = "Idram";
    rg->fFieldTitles["sum(op.f_prepaid)"] = "Կանխավճար";
    rg->fFieldTitles["sum(op.f_debt)"] = "Փոխանցում";
    rg->fFieldTitles["sum(op.f_discount)"] = tr("Discount");
    rg->fFieldTitles["sum(op.f_coupon)"] = "Նվեր քարտ";
    rg->fFieldTitles["sum(op.f_couponbank)"] = "Նվեր փոխանցում";
    rg->fFieldTitles["sum(op.f_couponservice)"] = "Ավտոկտրոն";
    rg->fFieldTitles["tl.f_special"] = "Հատուկ";
    rg->fFieldTitles["tl.f_partner"] = tr("Talon partner code");
    rg->fFieldTitles["tp.f_name"] = tr("Talon partner");
    rg->fTables.clear();
    rg->fTables << "o_header oh"
                << "o_dish od"
                << "o_state os"
                << "r_hall h"
                << "r_table t"
                << "o_car oc"
                << "r_store s"
                << "r_dish_type dt"
                << "r_dish d"
                << "users u"
                << "f_city_ledger cl"
                << "f_payment_type pm"
                << "o_dish_state ds"
                << "o_header_payment op"
                << "r_branch br"
                << "talon_service tl"
                << "r_partners tp";
    rg->fJoins.clear();
    rg->fJoins << "from"  //od
               << "inner" //oh
               << "inner" //os
               << "inner" //h
               << "inner" //t
               << "left"  //oc
               << "inner" //s
               << "inner" //dt
               << "inner" // d
               << "inner" //u
               << "left"  //cl
               << "inner" //pm
               << "left"  //ds
               << "left"  //op
               << "left"  //br
               << "left"  //tl
               << "left"  //tp
        ;
    rg->fJoinConds.clear();
    rg->fJoinConds << ""
                   << "od.f_header=oh.f_id"
                   << "os.f_id=oh.f_state"
                   << "h.f_id=oh.f_hall"
                   << "t.f_id=oh.f_table"
                   << "oc.f_order=oh.f_id "
                   << "s.f_id=od.f_store"
                   << "dt.f_id=d.f_type"
                   << "d.f_id=od.f_dish"
                   << "u.f_id=oh.f_staff"
                   << "cl.f_id=oh.f_cityLedger"
                   << "pm.f_id=oh.f_paymentMode"
                   << "ds.f_id=od.f_state"
                   << "op.f_id=oh.f_id"
                   << "br.f_id=oh.f_branch"
                   << "tl.f_order=oh.f_id"
                   << "tp.f_id=tl.f_partner";
    QString where = "where (oh.f_dateCash between '" + ui->deStart->date().toString(def_mysql_date_format) + "' "
                    + " and '" + ui->deEnd->date().toString(def_mysql_date_format) + "') ";

    if(!ui->leOrder->text().isEmpty()) {
        where += " and oh.f_id in (" + ui->leOrder->text() + ") ";
    }

    if(!ui->leState->text().isEmpty()) {
        where += " and oh.f_state in (" + ui->leState->fHiddenText + ") ";
    }

    if(!ui->leHall->text().isEmpty()) {
        where += " and oh.f_hall in (" + ui->leHall->fHiddenText + ") ";
    }

    if(!ui->leTable->text().isEmpty()) {
        where += " and oh.f_table in (" + ui->leTable->fHiddenText + ") ";
    }

    if(!ui->leStaff->text().isEmpty()) {
        where += " and oh.f_staff in (" + ui->leStaff->fHiddenText + ") ";
    }

    if(!countAmount) {
        if(!ui->leDishState->text().isEmpty()) {
            where += " and od.f_state in (" + ui->leDishState->fHiddenText + ") ";
        }
    }

    if(!countAmount) {
        if(ui->chOnlyZeroes->isChecked()) {
            where += " and od.f_total=0 ";
        }
    } else {
        if(ui->chOnlyZeroes->isChecked()) {
            where += " and oh.f_total=0 ";
        }
    }

    if(ui->rb500Yes->isChecked()) {
        where += " and od.f_dish in (159,171,158,169,153,165,386,387,388,389,390,391) ";
    } else if(ui->rb500No->isChecked()) {
        where += " and od.f_dish not in (159,171,158,169,153,165,386,387,388,389,390,391) ";
    }

    if(!ui->leStore->text().isEmpty()) {
        where += " and od.f_store in (" + ui->leStore->fHiddenText + ") ";
    }

    if(!ui->leDishType->text().isEmpty()) {
        where += " and d.f_type in (" + ui->leDishType->fHiddenText + ") ";
    }

    if(!ui->leDish->text().isEmpty()) {
        where += " and od.f_dish in (" + ui->leDish->fHiddenText + ") ";
    }

    if(ui->leTax->text() == "+") {
        where += " and od.f_fiscal>0 ";
    }

    if(ui->leTax->text() == "-") {
        where += " and coalesce(od.f_fiscal, 0)=0 ";
    }

    if(!ui->leBranch->isEmpty()) {
        where += " and oh.f_branch in(" + ui->leBranch->fHiddenText + ") ";
    }

    if(ui->chCouponOfService->isChecked()) {
        where += " and oh.f_couponservice=1 ";
    }

    if(!ui->lePMComment->text().isEmpty()) {
        where += " and upper(oh.f_paymentModeComment) like '" + ui->lePMComment->text() + "%' ";
    }

    if(!ui->leTax->text().isEmpty() && ui->leTax->text() != "+" && ui->leTax->text() != "-") {
        where += " and od.f_fiscal in (" + ui->leTax->text() + ") ";
    }

    if(ui->chOrderNum->isChecked()) {
        order += "oh.f_id,";
    }

    if(ui->rbWithCard->isChecked()) {
        where += " and length(op.f_discountcard)>0 ";
    }

    if(ui->rbNoCard->isChecked()) {
        where += " and length(op.f_discountcard)=0 ";
    }

    QString group;
    QObjectList ol(children());
    bool first = true;

    foreach(QObject *o, ol) {
        if(isCheckBox(static_cast<QWidget* >(o))) {
            EQCheckBox *check = static_cast<EQCheckBox*>(o);

            if(check->isChecked()) {
                if(first) {
                    first = false;
                } else {
                    group += ",";
                }

                group += check->getField().replace(";", ",");

                if(check->getRequireLang()) {
                    group += def_lang;
                }
            }
        }
    }

    group = group.replace("op.f_cash,op.f_card,op.f_discount,op.f_debt,op.f_coupon,op.f_couponbank,op.f_couponservice,,",
                          "");

    if(group.length() > 0) {
        if(group.at(group.length() - 1) == ",") {
            group.remove(group.length() - 1, 1);
        }
    }

    if(!group.isEmpty()) {
        group = " group by " + group;
    }

    if(!order.isEmpty()) {
        order = " order by " + order;
    }

    where += group;
    order.remove(order.length() - 1, 1);
    where += order;
    buildQuery(rg, where);
    QList<int> colsTotal;

    if(!ui->chPaymentMode->isChecked()) {
        colsTotal << rg->fModel->columnIndex(tr("Qty"))
                  << rg->fModel->columnIndex(tr("Total"))
                  ;
    } else {
        colsTotal << rg->fModel->columnIndex(tr("Qty"))
                  << rg->fModel->columnIndex(tr("Total"))
                  << rg->fModel->columnIndex(tr("Cash"))
                  << rg->fModel->columnIndex(tr("Card"))
                  << rg->fModel->columnIndex("Idram")
                  << rg->fModel->columnIndex("Կանխավճար")
                  << rg->fModel->columnIndex("Փոխանցում")
                  << rg->fModel->columnIndex(tr("Discount"))
                  << rg->fModel->columnIndex("Նվեր քարտ")
                  << rg->fModel->columnIndex("Նվեր փոխանցում")
                  << rg->fModel->columnIndex("Ավտոկտրոն");
    }

    QList<double> valsTotal;
    rg->fModel->sumOfColumns(colsTotal, valsTotal);
    rg->setTblTotalData(colsTotal, valsTotal);

    if(ui->chOrderNum->isChecked()) {
        QColor dark = COLOR_DARK_ROW;
        QColor currColor = Qt::white;
        QString currId ;

        for(int i = 0, count = rg->fModel->rowCount(); i < count; i++) {
            if(rg->fModel->data(i, 0).toString() != currId) {
                currId = rg->fModel->data(i, 0).toString();
                currColor = (currColor == Qt::white ? dark : Qt::white);
            }

            rg->fModel->setBackgroundColor(i, currColor);
        }
    }

    rg->fTableView->resizeColumnsToContents();
    rg->syncTotalsWithMain();

    // Totals can be wider than main body values, so expand main columns if needed.
    const int cc = rg->fTableTotal->columnCount();
    bool changed = false;
    for (int i = 0; i < cc; i++) {
        if (rg->fTableView->columnWidth(i) == 0) {
            continue;
        }
        int totalsWidth = rg->fTableView->columnWidth(i);
        if (rg->fTableTotal->item(0, i)) {
            QFontMetrics fm(rg->fTableTotal->font());
            // Text width + cell paddings/sort of style margin reserve.
            totalsWidth = fm.horizontalAdvance(rg->fTableTotal->item(0, i)->text()) + 24;
        }
        if (totalsWidth > rg->fTableView->columnWidth(i)) {
            rg->fTableView->setColumnWidth(i, totalsWidth);
            changed = true;
        }
    }
    if (changed) {
        rg->syncTotalsWithMain();
    }
}

QWidget* FRestaurantTotal::firstElement()
{
    return ui->deStart;
}

QString FRestaurantTotal::reportTitle()
{
    return QString("%1 %2-%3")
           .arg(tr("Earnings"))
           .arg(ui->deStart->text())
           .arg(ui->deEnd->text());
}

void FRestaurantTotal::open()
{
    WReportGrid *rg = addTab<WReportGrid>();
    rg->setupTabTextAndIcon(tr("Earnings"), ":/images/cutlery.png");
    FRestaurantTotal *fr = new FRestaurantTotal(rg);
    rg->addFilterWidget(fr);
}

void FRestaurantTotal::selector(int selectorNumber, const QVariant &value)
{
    switch(selectorNumber) {
    case sn_order_state:
        dockResponse<CI_OrderState, CacheOrderState>(ui->leState, value.value<CI_OrderState*>());
        break;

    case sn_table:
        dockResponse<CI_RestTable, CacheRestTable>(ui->leTable, value.value<CI_RestTable*>());
        break;

    case sn_dish:
        dockResponse<CI_Dish, CacheDish>(ui->leDish, value.value<CI_Dish*>());
        break;

    case sn_dish_state:
        dockResponse<CI_DishState, CacheDishState>(ui->leDishState, value.value<CI_DishState*>());
        break;
    }
}

void FRestaurantTotal::printNewPage(int& top, int& left, int& page, PPrintPreview *pp, PPrintScene*& ps, int nextHeight)
{
    int footerTop = sizePortrait.height() - 200;
    QBrush b(Qt::white, Qt::SolidPattern);
    PTextRect trFooter;
    trFooter.setBrush(b);
    QFont ffooter(qApp->font().family(), 20);
    trFooter.setFont(ffooter);
    trFooter.setBorders(false, false, false, false);
    trFooter.setTextAlignment(Qt::AlignLeft);

    if(top + nextHeight > sizePortrait.height() - 300) {
        if(left == 20) {
            left = 1100;
        } else {
            left = 20;
            ps->addTextRect(20, footerTop, 1800, 60, QString("%1: %2 %3")
                            .arg(tr("Printed"))
                            .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                            .arg(WORKING_USERNAME), &trFooter);
            trFooter.setTextAlignment(Qt::AlignRight);
            ps->addTextRect(1800, footerTop, 200, 60, QString("%1 %2")
                            .arg(tr("Page"))
                            .arg(page), &trFooter);
            trFooter.setTextAlignment(Qt::AlignLeft);
            ps = pp->addScene(0, Portrait);
            page++;
        }

        top = 20;
    }
}

void FRestaurantTotal::branchEditDoubleClick(bool v)
{
    QString id, name;

    if(DlgGetIDName::get(id, name, idname_branch, this)) {
        ui->leBranch->setText(name);
        ui->leBranch->fHiddenText = id;
    }
}

void FRestaurantTotal::printReceipt()
{
    if(!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }

    QModelIndexList sel = fReportGrid->fTableView->selectionModel()->selectedIndexes();

    if(sel.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }

    QSet<QString> orders;

    foreach(QModelIndex m, sel) {
        orders.insert(fReportGrid->fModel->data(m.row(), 0, Qt::EditRole).toString());
    }

    if(orders.count() == 0) {
        return;
    }

    int left = 20;
    int top = 20;
    int page = 1;
    PTextRect prTempl;
    prTempl.setWrapMode(QTextOption::NoWrap);
    prTempl.setFont(QFont(qApp->font().family(), 17));
    prTempl.setBorders(true, true, true, true);
    QPen pen;
    pen.setWidth(2);
    prTempl.setRectPen(pen);
    PTextRect prHead(prTempl, "");
    prHead.setFont(QFont(qApp->font().family(), 20));
    prHead.setBrush(QBrush(QColor::fromRgb(215, 215, 215), Qt::SolidPattern));
    prHead.setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    PPrintPreview pp(this);
    PPrintScene *ps = pp.addScene(0, Portrait);
    PTextRect th;
    QFont f("Arial", 30);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    th.setTextAlignment(Qt::AlignLeft);
    int rowHeight = 60;
    CacheRestHall *hall = CacheRestHall::instance();
    CacheRestTable *table = CacheRestTable::instance();
    CacheUsers *users = CacheUsers::instance();

    //CachePaymentMode *pm = CachePaymentMode::instance();
    foreach(QString s, orders) {
        printNewPage(top, left, page, &pp, ps, 250);
        PImage *logo = new PImage("logo_print.png");
        ps->addItem(logo);
        logo->setRect(QRectF(left + 150, top, 400, 250));
        top += 260;
        printNewPage(top, left, page, &pp, ps);
        DatabaseResult dh;
        fDbBind[":f_id"] = s;
        dh.select(fDb, "select * from o_header where f_id=:f_id", fDbBind);
        top += ps->addTextRect(left + 10, top, 680, rowHeight, hall->get(dh.value("f_hall").toString())->fName, &prHead)
               ->textHeight();
        QString receiptNum = QString("%1 %2").arg(tr("Receipt S/N")).arg(s);
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(left + 10, top, 680, rowHeight, receiptNum, &prHead)->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Table"), &th, f));
        ps->addTextRect(new PTextRect(left + 160, top, 200, rowHeight, table->get(dh.value("f_table").toString())->fName, &th,
                                      f));
        ps->addTextRect(new PTextRect(left + 340, top, 230, rowHeight, tr("Date"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(450, top, 250, rowHeight,
                                             dh.value("f_datecash").toDate().toString(def_date_format), &th, f))
               ->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Time"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 200, rowHeight, QTime::currentTime().toString(def_time_format),
                                             &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Waiter"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 500, rowHeight, users->get(dh.value("f_staff").toString())->fFull,
                                             &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Opened"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 350, rowHeight,
                                             dh.value("f_dateOpen").toDateTime().toString(def_date_time_format), &th, f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 150, rowHeight, tr("Closed"), &th, f));
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += ps->addTextRect(new PTextRect(left + 160, top, 350, rowHeight,
                                             dh.value("f_dateClose").toDateTime().toString(def_date_time_format), &th, f))->textHeight();
        printNewPage(top, left, page, &pp, ps, rowHeight);
        top += 2;
        ps->addLine(left + 10, top, left + 680, top);
        top ++;
        ps->addTextRect(new PTextRect(left + 10, top, 100, rowHeight, tr("Qty"), &th, f));
        ps->addTextRect(new PTextRect(left + 110, top, 390, rowHeight, tr("Description"), &th, f));
        top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, tr("Amount"), &th, f))->textHeight();
        ps->addLine(left + 10, top, left + 680, top);
        top ++;

        DatabaseResult ddish;
        fDbBind[":f_header"] = s;
        ddish.select(fDb,
                     "select od.f_id, od.f_dish, d.f_en, d.f_ru, d.f_am, od.f_qty, od.f_qtyPrint, od.f_price, "
                     "od.f_svcValue, od.f_svcAmount, od.f_dctValue, od.f_dctAmount, od.f_total, "
                     "od.f_print1, od.f_print2, od.f_comment, od.f_staff, od.f_state, od.f_complex, od.f_complexId, "
                     "od.f_adgt, od.f_complexRec "
                     "from o_dish od "
                     "left join r_dish d on d.f_id=od.f_dish "
                     "where od.f_header=:f_header and f_state=1 "
                     "order by od.f_id ",
                     fDbBind);

        for(int i = 0; i < ddish.rowCount(); i++) {
            ps->addTextRect(new PTextRect(left + 10, top, 100, rowHeight, float_str(ddish.value(i, "f_qty").toDouble(), 1), &th,
                                          f));
            ps->addTextRect(new PTextRect(left + 110, top, 390, rowHeight, ddish.value(i, "f_en").toString(), &th, f));
            top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, float_str(ddish.value(i, "f_total").toDouble(),
                                                 2), &th, f))->textHeight();
            printNewPage(top, left, page, &pp, ps);
        }

        ps->addLine(left + 10, top, left + 680, top);
        top += 2;
        f.setPointSize(24);
        th.setFont(f);
        ps->addTextRect(new PTextRect(left + 10, top, 400, rowHeight, tr("Total, AMD"), &th, f));
        top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight, float_str(dh.value("f_total").toDouble(), 0), &th,
                                             f))->textHeight();
        ps->addTextRect(new PTextRect(left + 10, top, 400, rowHeight, tr("Total, USD"), &th, f));
        top += ps->addTextRect(new PTextRect(left + 500, top, 200, rowHeight,
                                             float_str(dh.value("f_total").toDouble() / def_usd, 2), &th, f))->textHeight();
        top += rowHeight;
        f.setPointSize(28);
        th.setFont(f);
        th.setTextAlignment(Qt::AlignHCenter);
        printNewPage(top, left, page, &pp, ps);

        if(!dh.value("f_roomComment").toString().isEmpty()) {
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, dh.value("f_roomComment").toString(), &th,
                                                 f))->textHeight();
            top += rowHeight;
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("Signature"), &th, f))->textHeight();
            top += rowHeight + 2;
            ps->addLine(left + 150, top, left + 680, top);
        }

        printNewPage(top, left, page, &pp, ps);

        if(dh.value("f_paymentMode").toInt() == PAYMENT_COMPLIMENTARY) {
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("COMPLIMENTARY"), &th, f))->textHeight();
        } else {
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("SALES"), &th, f))->textHeight();
        }

        if(dh.value("f_paymentMode").toInt()) {
            top += rowHeight;
            top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("Mode Of Payment"), &th, f))->textHeight();

            switch(dh.value("f_paymentMode").toInt()) {
            case PAYMENT_CASH:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, tr("CASH"), &th, f))->textHeight();
                break;

            case PAYMENT_CARD:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight,
                                                     tr("CARD") + "/" + dh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
                break;

            case PAYMENT_ROOM:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, dh.value("f_paymentModeComment").toString(), &th,
                                                     f))->textHeight();
                break;

            case PAYMENT_CL:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight,
                                                     "CL/" + dh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
                break;

            case PAYMENT_COMPLIMENTARY:
                top += ps->addTextRect(new PTextRect(left + 10, top, 680, rowHeight, dh.value("f_paymentModeComment").toString(), &th,
                                                     f))->textHeight();
                break;
            }
        } else {
            th.setTextAlignment(Qt::AlignLeft);
            top += rowHeight;
            ps->addTextRect(left + 10, top, 400, rowHeight, tr("Room number:"), &th);
            top += 2;
            ps->addLine(left + 300, top + rowHeight, left + 600, top + rowHeight);
            top += rowHeight + 2;
            ps->addTextRect(left + 10, top, 200, rowHeight, tr("Signature"), &th);
            top += 2;
            ps->addLine(left + 300, top + rowHeight, left + 600, top + rowHeight);
        }

        top++;
        ps->addLine(left + 10, top, left + 680, top, QPen(QBrush(Qt::SolidPattern), 5));
        top += 20;
        printNewPage(top, left, page, &pp, ps);
    }

    int footerTop = sizePortrait.height() - 200;
    QBrush b(Qt::white, Qt::SolidPattern);
    PTextRect trFooter;
    trFooter.setBrush(b);
    QFont ffooter(qApp->font().family(), 20);
    trFooter.setFont(ffooter);
    trFooter.setBorders(false, false, false, false);
    trFooter.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(20, footerTop, 1800, 60, QString("%1: %2 %3")
                    .arg(tr("Printed"))
                    .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                    .arg(WORKING_USERNAME), &trFooter);
    trFooter.setTextAlignment(Qt::AlignRight);
    ps->addTextRect(1800, footerTop, 200, 60, QString("%1 %2")
                    .arg(tr("Page"))
                    .arg(page), &trFooter);
    trFooter.setTextAlignment(Qt::AlignLeft);
    pp.exec();
}

void FRestaurantTotal::recalculateStore()
{
    if(!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }

    if(message_yesnocancel(tr("Confirm to recalculate store outputs")) != RESULT_YES) {
        return;
    }

    QSet<int> ids;

    for(int i = 0; i < fReportGrid->fModel->rowCount(); i++) {
        ids.insert(fReportGrid->fModel->data(i, 0, Qt::EditRole).toInt());
    }

    if(ids.isEmpty()) {
        message_error(tr("No orders in the report."));
        return;
    }

#ifdef RESORT_AUDIT_LOG
    const QString runId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QJsonObject start;
    start[QStringLiteral("order_count")] = ids.count();
    start[QStringLiteral("source")] = QStringLiteral("restaurant_total_report");
    ResortLog::logJobEvent(runId, QStringLiteral("store_recalc_start"), start);
#else
    const QString runId;
#endif

    RecalculateStoreOutputs *r = new RecalculateStoreOutputs(ids, runId, this);
    r->exec();
    delete r;
}

void FRestaurantTotal::removeOrder()
{
    if(!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }

    QList<QVariant> val;

    if(fReportGrid->fillRowValuesOut(val) < 0) {
        message_error(tr("Nothing was selected"));
        return;
    }

    if(message_yesnocancel(tr("Confirm to remove the order")) != RESULT_YES) {
        return;
    }

    DlgPERemoveReason *d = new DlgPERemoveReason(this);
    const int dishRemoveState = d->exec();
    delete d;

    if(dishRemoveState == QDialog::Rejected) {
        return;
    }

    const int orderId = val.at(0).toInt();

    if(orderId <= 0) {
        message_error(tr("Invalid order id"));
        return;
    }

    if(!ensureDatabaseReady(fDb)) {
        return;
    }

    fDbBind[":f_id"] = orderId;

    if(!execSql(fDb,
                "select f_state from o_header where f_id=:f_id",
                fDbBind,
                fDbRows)) {
        return;
    }

    if(fDbRows.isEmpty()) {
        message_error(tr("Order not found"));
        return;
    }

    if(fDbRows.at(0).at(0).toInt() == ORDER_STATE_REMOVED) {
        message_error(tr("Order is already removed"));
        return;
    }

    abortRemoveTransaction(fDb);

    if(!fDb.fDb.transaction()) {
        message_error(fDb.fLastError);
        return;
    }

    fDbBind[":f_id"] = orderId;

    if(!execSql(fDb,
                "select f_discountcard, f_discount from o_header_payment where f_id=:f_id",
                fDbBind,
                fDbRows)) {
        abortRemoveTransaction(fDb);
        return;
    }

    if(fDbRows.count() > 0) {
        const QString disccard = fDbRows.at(0).at(0).toString();

        if(!disccard.isEmpty()) {
            const double discamount = fDbRows.at(0).at(1).toDouble();
            fDbBind[":f_card"] = disccard;

            if(!execSql(fDb,
                        "select f_mode from d_car_client where f_card=:f_card",
                        fDbBind,
                        fDbRows)) {
                abortRemoveTransaction(fDb);
                return;
            }

            if(fDbRows.count() > 0) {
                QStringList params = fDbRows.at(0).at(0).toString().split(";", Qt::SkipEmptyParts);

                if(params.count() == 4) {
                    params[2] = QString::number(params[2].toDouble() + discamount, 'f', 0);
                    fDbBind[":f_card"] = disccard;
                    fDbBind[":f_mode"] = params.join(";") + ";";

                    if(!execSql(fDb,
                                "update d_car_client set f_mode=:f_mode where f_card=:f_card",
                                fDbBind,
                                fDbRows)) {
                        abortRemoveTransaction(fDb);
                        return;
                    }
                }
            }
        }
    }

    StoreOutput so(fDb, orderId);
    so.rollbackSale(fDb, orderId);

    fDbBind[":f_header"] = orderId;

    if(!execSql(fDb, "delete from o_recipe where f_header=:f_header", fDbBind, fDbRows)) {
        abortRemoveTransaction(fDb);
        return;
    }

    fDbBind[":f_state"] = ORDER_STATE_REMOVED;
    fDbBind[":f_comment"] = "Canceled by " + WORKING_USERNAME;

    if(!fDb.update("o_header", fDbBind, where_id(orderId))) {
        abortRemoveTransaction(fDb);
        message_error(fDb.fLastError);
        return;
    }

    fDbBind[":f_state"] = dishRemoveState;
    fDbBind[":f_state_cond"] = DISH_STATE_READY;
    fDbBind[":f_header"] = orderId;
    fDbBind[":f_comment"] = "Canceled by " + WORKING_USERNAME;

    if(!execSql(fDb,
                "update o_dish set f_state=:f_state, f_comment=:f_comment, f_emark=null "
                "where f_header=:f_header and f_state=:f_state_cond",
                fDbBind,
                fDbRows)) {
        abortRemoveTransaction(fDb);
        return;
    }

    fDbBind[":f_id"] = orderId;
    fDbBind[":f_cancelReason"] = "Canceled by " + WORKING_USERNAME;

    if(!execSql(fDb,
                "update m_register set f_canceled=1, f_cancelReason=:f_cancelReason where f_id=:f_id",
                fDbBind,
                fDbRows)) {
        abortRemoveTransaction(fDb);
        return;
    }

    if(!fDb.fDb.commit()) {
        abortRemoveTransaction(fDb);
        message_error(fDb.fLastError);
        return;
    }

    fDbBind[":f_id"] = orderId;

    if(!execSql(fDb,
                "select f_state from o_header where f_id=:f_id",
                fDbBind,
                fDbRows)
            || fDbRows.isEmpty()
            || fDbRows.at(0).at(0).toInt() != ORDER_STATE_REMOVED) {
        message_error(tr("Order was not removed"));
        return;
    }

    message_info_tr("Please, refresh report to view the changes");
}

void FRestaurantTotal::removePermanently()
{
    if(!fReportGrid->fIncludes["oh.f_id"]) {
        message_error(tr("Order id must be included in the query"));
        return;
    }

    QList<QVariant> val;

    if(fReportGrid->fillRowValuesOut(val) < 0) {
        message_error(tr("Nothing was selected"));
        return;
    }

    if(message_yesnocancel(tr("<h1><b>Confirm to remove the order<br>PERMANENTLY</b></h1>")) != RESULT_YES) {
        return;
    }

    const int orderId = val.at(0).toInt();

    if(orderId <= 0) {
        message_error(tr("Invalid order id"));
        return;
    }

    if(!ensureDatabaseReady(fDb)) {
        return;
    }

    abortRemoveTransaction(fDb);

    if(!fDb.fDb.transaction()) {
        message_error(fDb.fLastError);
        return;
    }

    StoreOutput so(fDb, orderId);
    so.rollbackSale(fDb, orderId);

    fDbBind[":f_header"] = orderId;

    if(!execSql(fDb, "delete from o_recipe where f_header=:f_header", fDbBind, fDbRows)
            || !execSql(fDb, "delete from o_dish where f_header=:f_header", fDbBind, fDbRows)) {
        abortRemoveTransaction(fDb);
        return;
    }

    fDbBind[":f_id"] = orderId;

    if(!execSql(fDb, "delete from o_header where f_id=:f_id", fDbBind, fDbRows)
            || !execSql(fDb, "delete from m_register where f_id=:f_id", fDbBind, fDbRows)) {
        abortRemoveTransaction(fDb);
        return;
    }

    if(!fDb.fDb.commit()) {
        abortRemoveTransaction(fDb);
        message_error(fDb.fLastError);
        return;
    }

    message_info_tr("Please, refresh report to view the changes");
}

void FRestaurantTotal::hall(CI_RestHall *c)
{
    dockResponse<CI_RestHall, CacheRestHall>(ui->leHall, c);
}

void FRestaurantTotal::user(CI_User *c)
{
    dockResponse<CI_User, CacheUsers>(ui->leStaff, c);
}

void FRestaurantTotal::store(CI_RestStore *c)
{
    dockResponse<CI_RestStore, CacheRestStore>(ui->leStore, c);
}

void FRestaurantTotal::dishType(CI_RestDishType *c)
{
    dockResponse<CI_RestDishType, CacheRestDishType>(ui->leDishType, c);
}

void FRestaurantTotal::doubleClick(const QList<QVariant>& row)
{
    if(!ui->chOrderNum->isChecked()) {
        message_error(tr("Order number checkbox must be checked"));
        return;
    }

    if(row.count() == 0) {
        message_error(tr("Nothing is selected"));
        return;
    }

    DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(this);
    d->setOrder(row.at(0).toString());
    d->exec();
    delete d;
}

void FRestaurantTotal::on_btnPrevDate_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(-1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(-1));
}

void FRestaurantTotal::on_btnNextDate_clicked()
{
    ui->deStart->setDate(ui->deStart->date().addDays(1));
    ui->deEnd->setDate(ui->deEnd->date().addDays(1));
}

void FRestaurantTotal::compareFiscalInfo()
{
    QXLSX_USE_NAMESPACE

    const QString sourceFile = QFileDialog::getOpenFileName(this,
                                                            tr("Open portal fiscal file"),
                                                            QString(),
                                                            tr("Excel files (*.xlsx)"));

    if(sourceFile.isEmpty()) {
        return;
    }

    if(!ui->chOrderNum->isChecked()) {
        ui->chOrderNum->setChecked(true);
    }

    if(!ui->chTax->isChecked()) {
        ui->chTax->setChecked(true);
    }

    QMap<QString, QMap<QString, QString>> portalByFiscal;
    QString error;

    if(!readPortalFiscals(sourceFile, portalByFiscal, error)) {
        message_error(error);
        return;
    }

    QMultiMap<QString, int> dbOrdersByFiscal;
    const QString sql = QString(
                            "select distinct oh.f_id, od.f_fiscal "
                            "from o_header oh "
                            "inner join o_dish od on od.f_header=oh.f_id "
                            "where oh.f_dateCash between '%1' and '%2' "
                            "and coalesce(od.f_fiscal, 0) > 0 "
                            "order by oh.f_id, od.f_fiscal")
                            .arg(ui->deStart->date().toString(def_mysql_date_format),
                                 ui->deEnd->date().toString(def_mysql_date_format));

    if(fDb.select(sql, fDbBind, fDbRows) < 0) {
        message_error(fDb.fLastError);
        return;
    }

    QSet<QString> dbFiscals;

    for(const QList<QVariant> &row : qAsConst(fDbRows)) {
        if(row.count() < 2) {
            continue;
        }

        const QString fiscal = normalizeFiscalNumber(row.at(1));

        if(fiscal.isEmpty()) {
            continue;
        }

        dbFiscals.insert(fiscal);
        dbOrdersByFiscal.insert(fiscal, row.at(0).toInt());
    }

    QString saveFile = QFileDialog::getSaveFileName(this,
                                                    tr("Save fiscal comparison"),
                                                    QString(),
                                                    tr("Excel files (*.xlsx)"));

    if(saveFile.isEmpty()) {
        return;
    }

    if(!saveFile.endsWith(".xlsx", Qt::CaseInsensitive)) {
        saveFile += ".xlsx";
    }

    Document out;
    out.addSheet("Comparison");
    out.selectSheet("Comparison");
    Worksheet *sheet = out.currentWorksheet();

    if(!sheet) {
        message_error(tr("Cannot create excel sheet"));
        return;
    }

    Format headerFormat;
    headerFormat.setFontBold(true);
    headerFormat.setPatternForegroundColor(QColor::fromRgb(200, 200, 250));
    headerFormat.setPatternBackgroundColor(QColor::fromRgb(200, 200, 250));
    headerFormat.setFillPattern(Format::PatternSolid);
    headerFormat.setBorderStyle(Format::BorderThin);

    Format mismatchFormat;
    mismatchFormat.setPatternForegroundColor(QColor::fromRgb(255, 220, 220));
    mismatchFormat.setPatternBackgroundColor(QColor::fromRgb(255, 220, 220));
    mismatchFormat.setFillPattern(Format::PatternSolid);
    mismatchFormat.setBorderStyle(Format::BorderThin);

    Format matchFormat;
    matchFormat.setBorderStyle(Format::BorderThin);

    int matched = 0;
    int onlyPortal = 0;
    int onlyDb = 0;

    for(auto it = portalByFiscal.constBegin(); it != portalByFiscal.constEnd(); ++it) {
        if(dbFiscals.contains(it.key())) {
            ++matched;
        } else {
            ++onlyPortal;
        }
    }

    for(const QString &fiscal : qAsConst(dbFiscals)) {
        if(!portalByFiscal.contains(fiscal)) {
            ++onlyDb;
        }
    }

    sheet->write(1, 1, tr("Period"), headerFormat);
    sheet->write(1, 2, QString("%1 - %2").arg(ui->deStart->text(), ui->deEnd->text()));
    sheet->write(2, 1, tr("Portal file"), headerFormat);
    sheet->write(2, 2, sourceFile);
    sheet->write(3, 1, tr("Portal fiscal count"), headerFormat);
    sheet->write(3, 2, portalByFiscal.count());
    sheet->write(4, 1, tr("DB fiscal count"), headerFormat);
    sheet->write(4, 2, dbFiscals.count());
    sheet->write(5, 1, tr("Matched"), headerFormat);
    sheet->write(5, 2, matched);
    sheet->write(6, 1, tr("Only in portal"), headerFormat);
    sheet->write(6, 2, onlyPortal);
    sheet->write(7, 1, tr("Only in DB"), headerFormat);
    sheet->write(7, 2, onlyDb);

    const int headerRow = 9;
    const QStringList titles = {
        tr("Status"),
        tr("Fiscal"),
        tr("Order"),
        tr("Portal receipt"),
        tr("Portal datetime"),
        tr("Portal service"),
        tr("Portal amount")
    };

    for(int col = 0; col < titles.count(); ++col) {
        sheet->write(headerRow, col + 1, titles.at(col), headerFormat);
    }

    int outRow = headerRow + 1;

    auto writeRow = [&](const QString &status,
                        const QString &fiscal,
                        const QString &order,
                        const QMap<QString, QString> &info,
                        bool mismatch) {
        const Format &fmt = mismatch ? mismatchFormat : matchFormat;
        sheet->write(outRow, 1, status, fmt);
        sheet->write(outRow, 2, fiscal, fmt);
        sheet->write(outRow, 3, order, fmt);
        sheet->write(outRow, 4, info.value("receipt"), fmt);
        sheet->write(outRow, 5, info.value("datetime"), fmt);
        sheet->write(outRow, 6, info.value("service"), fmt);
        sheet->write(outRow, 7, info.value("amount"), fmt);
        ++outRow;
    };

    QStringList portalKeys = portalByFiscal.keys();
    std::sort(portalKeys.begin(), portalKeys.end());

    for(const QString &fiscal : qAsConst(portalKeys)) {
        const QMap<QString, QString> info = portalByFiscal.value(fiscal);

        if(dbFiscals.contains(fiscal)) {
            const QList<int> orders = dbOrdersByFiscal.values(fiscal);
            QStringList orderTexts;

            for(int orderId : orders) {
                orderTexts << QString::number(orderId);
            }

            writeRow(tr("Matched"),
                     fiscal,
                     orderTexts.join(", "),
                     info,
                     false);
        } else {
            writeRow(tr("Only in portal"),
                     fiscal,
                     QString(),
                     info,
                     true);
        }
    }

    QStringList dbOnlyKeys;

    for(const QString &fiscal : qAsConst(dbFiscals)) {
        if(!portalByFiscal.contains(fiscal)) {
            dbOnlyKeys.append(fiscal);
        }
    }

    std::sort(dbOnlyKeys.begin(), dbOnlyKeys.end());

    for(const QString &fiscal : qAsConst(dbOnlyKeys)) {
        const QList<int> orders = dbOrdersByFiscal.values(fiscal);
        QStringList orderTexts;

        for(int orderId : orders) {
            orderTexts << QString::number(orderId);
        }

        writeRow(tr("Only in DB"),
                 fiscal,
                 orderTexts.join(", "),
                 QMap<QString, QString>(),
                 true);
    }

    out.setColumnWidth(1, 18);
    out.setColumnWidth(2, 14);
    out.setColumnWidth(3, 12);
    out.setColumnWidth(4, 16);
    out.setColumnWidth(5, 22);
    out.setColumnWidth(6, 36);
    out.setColumnWidth(7, 12);

    if(!out.saveAs(saveFile)) {
        message_error(tr("Failed to save Excel file"));
        return;
    }

    message_info(tr("Fiscal comparison saved to %1").arg(saveFile));
}
