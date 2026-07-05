#include "sessiondebtclose.h"
#include "base.h"
#include "baseorder.h"
#include "database2.h"
#include "defines.h"
#include "defrest.h"
#include "pprintreceipt.h"
#include "preferences.h"
#include "databaseresult.h"
#include <QDateTime>

static bool openDb(Database2 &db2)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    return db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
}

bool SessionDebtClose::loadOpenOrders(int branch, QList<SessionDebtOrder> &orders)
{
    orders.clear();
    Database2 db2;

    if(!openDb(db2)) {
        return false;
    }

    db2[":f_branch"] = branch;
    db2[":f_state"] = ORDER_STATE_OPENED;
    db2.exec("select o.f_id, t.f_id as f_table, t.f_name as f_table_name, h.f_id as f_hall, "
             "o.f_total, c.f_costumer, c.f_govnumber "
             "from r_table t "
             "inner join r_hall h on h.f_id=t.f_hall "
             "inner join o_header o on o.f_id=t.f_order "
             "left join o_car c on c.f_order=o.f_id "
             "where h.f_branch=:f_branch and o.f_state=:f_state "
             "order by t.f_name");

    while(db2.next()) {
        SessionDebtOrder item;
        item.orderId = db2.integer("f_id");
        item.tableId = db2.integer("f_table");
        item.tableName = db2.string("f_table_name");
        item.hallId = db2.integer("f_hall");
        item.total = db2.doubleValue("f_total");
        item.customerId = db2.integer("f_costumer");
        item.govNumber = db2.string("f_govnumber").trimmed().toUpper();
        orders.append(item);
    }

    return true;
}

bool SessionDebtClose::validateOrders(const QList<SessionDebtOrder> &orders, QString &error)
{
    QStringList problems;

    for(const SessionDebtOrder &item : orders) {
        if(item.govNumber.isEmpty()) {
            problems << QObject::tr("Table %1 (order %2)").arg(item.tableName).arg(item.orderId);
        } else if(item.customerId <= 0) {
            problems << QObject::tr("Table %1 (order %2): customer is not set")
                              .arg(item.tableName)
                              .arg(item.orderId);
        }
    }

    if(problems.isEmpty()) {
        return true;
    }

    error = QObject::tr("Car plate number is required for all open orders:") + "\n" + problems.join("\n");
    return false;
}

static bool closeSingleOrderAsDebt(Database &db, const SessionDebtOrder &item, int staffId, QString &error)
{
    QMap<QString, QVariant> bind;

    if(!db.fDb.transaction()) {
        error = db.fLastError;
        return false;
    }

    BaseOrder bo(item.orderId);
    bo.calculateOutput(db);

    bind[":f_id"] = item.orderId;
    DatabaseResult drPayment;
    drPayment.select(db, "select f_id from o_header_payment where f_id=:f_id", bind);

    bind[":f_cash"] = 0;
    bind[":f_card"] = 0;
    bind[":f_idram"] = 0;
    bind[":f_coupon"] = 0;
    bind[":f_couponservice"] = 0;
    bind[":f_discount"] = 0;
    bind[":f_couponSeria"] = "";
    bind[":f_couponNumber"] = "";
    bind[":f_discountCard"] = "";
    bind[":f_costumer"] = item.customerId;
    bind[":f_finalAmount"] = item.total;
    bind[":f_debt"] = item.total;
    bind[":f_debtHolder"] = item.customerId;

    if(drPayment.rowCount() > 0) {
        if(!db.update("o_header_payment", bind, where_id(ap(item.orderId)))) {
            db.fDb.rollback();
            error = db.fLastError;
            return false;
        }
    } else {
        bind[":f_id"] = item.orderId;

        if(!db.insertWithoutId("o_header_payment", bind)) {
            db.fDb.rollback();
            error = db.fLastError;
            return false;
        }
    }

    bind.clear();
    bind[":f_order"] = item.orderId;
    bind[":f_govnumber"] = item.govNumber;
    bind[":f_debt"] = item.total;
    bind[":f_datetime"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if(db.insert("o_header_debt", bind) <= 0) {
        db.fDb.rollback();
        error = db.fLastError;
        return false;
    }

    bind.clear();
    bind[":f_state"] = ORDER_STATE_CLOSED;
    bind[":f_dateCash"] = Preferences().getLocalDate(def_working_day);
    bind[":f_dateClose"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if(!db.update("o_header", bind, where_id(ap(item.orderId)))) {
        db.fDb.rollback();
        error = db.fLastError;
        return false;
    }

    bind.clear();
    bind[":f_order"] = 0;
    bind[":f_lockHost"] = "";

    if(!db.update("r_table", bind, where_id(ap(item.tableId)))) {
        db.fDb.rollback();
        error = db.fLastError;
        return false;
    }

    if(!db.fDb.commit()) {
        db.fDb.rollback();
        error = db.fLastError;
        return false;
    }

    const QString receiptPrinter = defrest(dr_first_receipt_printer);
    PPrintReceipt::printOrder(receiptPrinter, item.orderId, staffId);

    return true;
}

bool SessionDebtClose::closeOrdersAsDebt(Database &db,
                                         const QList<SessionDebtOrder> &orders,
                                         int staffId,
                                         QString &error)
{
    for(const SessionDebtOrder &item : orders) {
        if(!closeSingleOrderAsDebt(db, item, staffId, error)) {
            if(error.isEmpty()) {
                error = QObject::tr("Cannot close order %1 as debt").arg(item.orderId);
            }

            return false;
        }
    }

    return true;
}
