#include "debtpay.h"
#include "base.h"
#include "database2.h"
#include "defines.h"
#include "dishestable.h"
#include "orderlog.h"
#include "preferences.h"
#include "printtaxno.h"
#include "talonservice.h"
#include <QCoreApplication>
#include <QDate>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSettings>

static bool openDb(Database2 &db2)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    return db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
}

static void readOpenDebtRows(Database2 &db2, QList<OpenDebtRow> &rows)
{
    while(db2.next()) {
        OpenDebtRow row;
        row.orderId = db2.integer("f_order");
        row.govNumber = db2.string("f_govnumber");
        row.dateTime = db2.dateTimeValue("f_datetime");
        row.balance = db2.doubleValue("f_balance");
        rows.append(row);
    }
}

QString DebtPay::normalizeGovNumber(const QString &govNumber)
{
    return govNumber.trimmed().remove(' ').toUpper();
}

bool DebtPay::loadOpenDebts(const QString &govNumber, QList<OpenDebtRow> &rows)
{
    rows.clear();
    const QString normalized = normalizeGovNumber(govNumber);

    if(normalized.isEmpty()) {
        return true;
    }

    Database2 db2;

    if(!openDb(db2)) {
        return false;
    }

    db2[":f_govnumber"] = normalized;
    db2.exec("select d.f_order, d.f_govnumber, max(d.f_datetime) as f_datetime, sum(d.f_debt) as f_balance "
             "from o_header_debt d "
             "where upper(replace(d.f_govnumber, ' ', ''))=:f_govnumber "
             "group by d.f_order, d.f_govnumber "
             "having sum(d.f_debt) > 0.001 "
             "order by f_datetime desc");

    readOpenDebtRows(db2, rows);
    return true;
}

bool DebtPay::loadAllOpenDebts(int branch, QList<OpenDebtRow> &rows)
{
    rows.clear();
    Database2 db2;

    if(!openDb(db2)) {
        return false;
    }

    db2[":f_branch"] = branch;
    db2.exec("select d.f_order, d.f_govnumber, max(d.f_datetime) as f_datetime, sum(d.f_debt) as f_balance "
             "from o_header_debt d "
             "inner join o_header oh on oh.f_id=d.f_order "
             "where oh.f_branch=:f_branch "
             "group by d.f_order, d.f_govnumber "
             "having sum(d.f_debt) > 0.001 "
             "order by f_datetime desc");

    readOpenDebtRows(db2, rows);
    return true;
}

static double currentBalance(Database2 &db2, int orderId)
{
    db2[":f_order"] = orderId;
    db2.exec("select sum(f_debt) as f_balance from o_header_debt where f_order=:f_order group by f_order");

    if(db2.next()) {
        return db2.doubleValue("f_balance");
    }

    return 0;
}

static void appendEmarkIfValid(PrintTaxNO &pn, const QString &emark, const QString &adgt)
{
    if(emark.isEmpty() || emark == adgt || !isValidEmarkCode(emark)) {
        return;
    }

    if(!pn.fEmarks.contains(emark)) {
        pn.fEmarks.append(emark);
    }
}

static bool loadFiscalMachineForOrder(Database2 &db2, int orderId, QMap<QString, QVariant> &machine, QString &error)
{
    QSettings settings(QString("%1\\fiscal.ini").arg(QCoreApplication::applicationDirPath()), QSettings::IniFormat);
    QMap<QString, QMap<QString, QVariant>> machines;
    QString defaultMachine;

    for(const QString &group : settings.childGroups()) {
        settings.beginGroup(group);
        QMap<QString, QVariant> params;

        for(const QString &key : settings.childKeys()) {
            params[key] = settings.value(key);
        }

        if(settings.value("default").toBool()) {
            defaultMachine = group;
        }

        machines[group] = params;
        settings.endGroup();
    }

    if(machines.isEmpty()) {
        error = QObject::tr("Fiscal not found");
        return false;
    }

    db2[":f_id"] = orderId;
    db2.exec("select f_hall from o_header where f_id=:f_id");
    const int hall = db2.next() ? db2.integer("f_hall") : 0;

    for(auto it = machines.constBegin(); it != machines.constEnd(); ++it) {
        if(it.value().value("hall").toInt() == hall) {
            machine = it.value();
            return true;
        }
    }

    if(!defaultMachine.isEmpty() && machines.contains(defaultMachine)) {
        machine = machines.value(defaultMachine);
        return true;
    }

    if(machines.count() == 1) {
        machine = machines.constBegin().value();
        return true;
    }

    error = QObject::tr("Fiscal not found");
    return false;
}

static bool insertDebtPayment(Database2 &db2,
                              int orderId,
                              const QString &govNumber,
                              double amount,
                              int paymentMode,
                              QString &error)
{
    db2[":f_order"] = orderId;
    db2[":f_govnumber"] = govNumber;
    db2[":f_debt"] = amount * -1;
    db2[":f_datetime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    db2[":f_payment_mode"] = paymentMode;

    int debtId = 0;

    if(!db2.insert("o_header_debt", debtId)) {
        error = db2.lastDbError();
        return false;
    }

    return true;
}

bool DebtPay::payDebt(int orderId,
                      const QString &govNumber,
                      double balance,
                      int paymentMode,
                      const QString &talonCode,
                      QString &error)
{
    if(orderId <= 0 || balance <= 0.001) {
        error = QObject::tr("Invalid debt");
        return false;
    }

    Database2 db2;

    if(!openDb(db2)) {
        error = QObject::tr("Database error");
        return false;
    }

    if(!db2.startTransaction()) {
        error = db2.lastDbError();
        return false;
    }

    const double openBalance = currentBalance(db2, orderId);

    if(openBalance <= 0.001) {
        db2.rollback();
        error = QObject::tr("Debt is already closed");
        return false;
    }

    if(qAbs(openBalance - balance) > 0.01) {
        db2.rollback();
        error = QObject::tr("Debt amount has changed, refresh the list");
        return false;
    }

    if(paymentMode == PAYMENT_TALON) {
        TalonRedeemInfo info;
        const QString code = TalonService::normalizeCode(talonCode);

        if(!TalonService::redeemForOrderInTx(db2, orderId, code, info, error)) {
            db2.rollback();
            return false;
        }
    } else if(paymentMode == PAYMENT_PREPAID) {
        const QString code = talonCode.trimmed().replace(";", "").replace("?", "");

        if(code.isEmpty()) {
            db2.rollback();
            error = QObject::tr("Enter prepaid card code");
            return false;
        }

        db2[":f_code"] = code;
        db2.exec("select di.f_info, sum(du.f_amount) as f_sum "
                 "from d_gift_cart_use du "
                 "inner join d_gift_cart di on di.f_code=du.f_code "
                 "where du.f_code=:f_code "
                 "group by di.f_info "
                 "having sum(du.f_amount) is not null");

        if(!db2.next()) {
            db2.rollback();
            error = QObject::tr("Unknown card");
            return false;
        }

        const double cardBalance = db2.doubleValue("f_sum");

        if(cardBalance <= 0.001) {
            db2.rollback();
            error = QObject::tr("Card amount spent");
            return false;
        }

        if(cardBalance + 0.01 < openBalance) {
            db2.rollback();
            error = QObject::tr("Insufficient card balance");
            return false;
        }

        db2[":f_code"] = code;
        db2[":f_amount"] = openBalance * -1;
        db2[":f_order"] = orderId;

        if(!db2.insert("d_gift_cart_use")) {
            db2.rollback();
            error = db2.lastDbError();
            return false;
        }
    }

    if(!insertDebtPayment(db2, orderId, govNumber, openBalance, paymentMode, error)) {
        db2.rollback();
        return false;
    }

    if(!db2.commit()) {
        db2.rollback();
        error = db2.lastDbError();
        return false;
    }

    if(paymentMode == PAYMENT_TALON) {
        const QString code = TalonService::normalizeCode(talonCode);
        OrderLog::write(orderId, OrderLog::ACTION_DISCOUNT_OK,
                        QString("code=%1;result=ok;reason=debt_talon;amount=%2")
                        .arg(code)
                        .arg(balance));
    } else if(paymentMode == PAYMENT_PREPAID) {
        const QString code = talonCode.trimmed().replace(";", "").replace("?", "");
        OrderLog::write(orderId, OrderLog::ACTION_DISCOUNT_OK,
                        QString("code=%1;result=ok;reason=debt_prepaid;amount=%2")
                        .arg(code)
                        .arg(balance));
    }

    return true;
}

bool DebtPay::printOrderFiscalIfNeeded(int orderId, double amount, int paymentMode, QString &error)
{
    if(orderId <= 0 || amount <= 0.001) {
        return true;
    }

    Database2 db2;

    if(!openDb(db2)) {
        error = QObject::tr("Database error");
        return false;
    }

    db2[":f_id"] = orderId;
    db2.exec("select f_tax from o_header where f_id=:f_id");

    if(!db2.next()) {
        error = QObject::tr("Not valid order id");
        return false;
    }

    if(db2.integer("f_tax") > 0) {
        return true;
    }

    QMap<QString, QVariant> fiscalParams;

    if(!loadFiscalMachineForOrder(db2, orderId, fiscalParams, error)) {
        return false;
    }

    PrintTaxNO pn(fiscalParams.value("ip").toString(),
                  fiscalParams.value("port").toInt(),
                  fiscalParams.value("password").toString(),
                  fiscalParams.value("extpos").toString(),
                  fiscalParams.value("opcode").toString(),
                  fiscalParams.value("oppin").toString());

    db2[":f_header"] = orderId;
    db2[":f_state"] = DISH_STATE_READY;
    db2.exec("select d.f_en, d.f_adgt, od.f_qty, od.f_price, od.f_dctvalue, d.f_taxdebt, d.f_id, od.f_emark, od.f_id as f_od_id "
             "from o_dish od "
             "left join r_dish d on d.f_id=od.f_dish "
             "where od.f_header=:f_header and od.f_state=:f_state");

    QList<int> dishIds;
    bool hasGoods = false;

    while(db2.next()) {
        if(db2.doubleValue("f_price") < 0.01) {
            continue;
        }

        appendEmarkIfValid(pn, db2.string("f_emark"), db2.string("f_adgt"));
        dishIds.append(db2.integer("f_od_id"));
        hasGoods = true;
        pn.addGoods(db2.string("f_taxdebt").toInt(),
                    db2.string("f_adgt"),
                    db2.string("f_id"),
                    db2.string("f_en"),
                    db2.doubleValue("f_price"),
                    db2.doubleValue("f_qty"),
                    db2.doubleValue("f_dctvalue"));
    }

    if(!hasGoods) {
        return true;
    }

    double cash = 0;
    double card = 0;
    double prepaid = 0;

    switch(paymentMode) {
    case PAYMENT_CARD:
        card = amount;
        break;
    case PAYMENT_PREPAID:
        prepaid = amount;
        break;
    default:
        cash = amount;
        break;
    }

    QString in;
    QString out;
    QString err;
    const int result = pn.makeJsonAndPrint(cash, card, prepaid, in, out, err);

    if(result != pt_err_ok) {
        error = QObject::tr("Fiscal error.") + "\r\n" + err;
        return false;
    }

    const QJsonObject jo = QJsonDocument::fromJson(out.toUtf8()).object();
    const int fiscalNumber = jo["rseq"].toInt();
    int fiscalRecId = 0;

    db2[":f_order"] = orderId;
    db2[":f_in"] = QByteArray(in.toUtf8()).toBase64();
    db2[":f_out"] = out;
    db2[":f_err"] = err;

    if(!db2.insert("o_tax_log", fiscalRecId)) {
        error = db2.lastDbError();
        return false;
    }

    db2[":f_fiscal"] = fiscalNumber;
    db2.update("o_tax_log", "f_id", fiscalRecId);
    db2[":f_tax"] = fiscalNumber;

    if(!db2.update("o_header", "f_id", orderId)) {
        error = db2.lastDbError();
        return false;
    }

    for(const int dishId : dishIds) {
        db2[":f_fiscal"] = fiscalNumber;
        db2[":f_id"] = dishId;
        db2.exec("update o_dish set f_fiscal=:f_fiscal where f_id=:f_id");
    }

    return true;
}

QString DebtPay::paymentModeName(int paymentMode)
{
    switch(paymentMode) {
    case PAYMENT_CASH:
        return QObject::tr("Cash");
    case PAYMENT_CARD:
        return QObject::tr("Card");
    case PAYMENT_TALON:
        return QObject::tr("Talon");
    case PAYMENT_PREPAID:
        return QObject::tr("Prepaid card");
    default:
        return QString::number(paymentMode);
    }
}
