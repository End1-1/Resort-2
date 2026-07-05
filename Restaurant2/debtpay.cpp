#include "debtpay.h"
#include "base.h"
#include "database2.h"
#include "preferences.h"
#include <QDate>
#include <QDateTime>
#include <QObject>

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

static bool applyTalon(Database2 &db2, const QString &code, QString &error)
{
    QString normalized = code;
    normalized.replace("?", "").replace(";", "");
    normalized.replace("tel:", "", Qt::CaseInsensitive);
    normalized.replace("http://", "", Qt::CaseInsensitive);
    normalized = normalized.trimmed();

    if(normalized.isEmpty()) {
        error = QObject::tr("Talon code is empty");
        return false;
    }

    db2[":f_code"] = normalized;
    db2.exec("select t.*, p.f_name as f_partnername "
             "from talon_service t "
             "left join r_partners p on p.f_id=t.f_partner "
             "where f_code=:f_code");

    if(!db2.next()) {
        error = QObject::tr("Invalid talon");
        return false;
    }

    if(db2.integer("f_trsale") == 0) {
        error = QObject::tr("This coupon not sold");
        return false;
    }

    if(db2.integer("f_trback") > 0) {
        error = QObject::tr("This coupon used");
        return false;
    }

    const int partnerId = db2.integer("f_partner");
    const double price = db2.doubleValue("f_price");
    db2[":f_date"] = QDate::currentDate();
    db2[":f_partner"] = partnerId;
    db2[":f_amount"] = price;
    int docId = 0;

    if(!db2.insert("talon_documents_header", docId) || docId <= 0) {
        error = db2.lastDbError();
        return false;
    }

    db2[":f_doc"] = docId;
    db2[":f_group"] = "";
    db2[":f_first"] = normalized;
    db2[":f_last"] = normalized;
    db2[":f_qty"] = 1;
    db2[":f_price"] = price;
    db2[":f_total"] = price;

    if(!db2.insert("talon_body")) {
        error = db2.lastDbError();
        return false;
    }

    db2[":f_trback"] = docId;
    db2[":f_code"] = normalized;

    if(!db2.exec("update talon_service set f_trback=:f_trback where f_code=:f_code")) {
        error = db2.lastDbError();
        return false;
    }

    return true;
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
        if(!applyTalon(db2, talonCode, error)) {
            db2.rollback();
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

    return true;
}
