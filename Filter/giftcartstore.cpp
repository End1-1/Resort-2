#include "giftcartstore.h"
#ifdef RESORT_AUDIT_LOG
#include "resortlog.h"
#endif
#include <QDateTime>
#include <QObject>
#include <QSqlError>
#include "base.h"
#include "defstore.h"
#include "message.h"
#include "storeoutput.h"

namespace {
int amountKey(double amount)
{
    return qRound(amount);
}
}

bool GiftCartStore::isSoldStatus(int status)
{
    return status == SOLD_CARD_STATUS;
}

bool GiftCartStore::isWarehouseId(Database &db, int storeId)
{
    if(storeId <= 0 || isSoldStatus(storeId)) {
        return false;
    }
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > rows;
    bind[":f_id"] = storeId;
    return db.select("select f_id from r_store where f_id=:f_id and f_state=1", bind, rows) >= 0
            && !rows.isEmpty();
}

double GiftCartStore::storeBalance(Database &db, int storeId, int goodsId)
{
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > rows;
    bind[":f_store"] = storeId;
    bind[":f_goods"] = goodsId;
    if(db.select("select coalesce(sum(b.f_qty * b.f_sign), 0) "
                 "from r_store_acc b "
                 "inner join r_docs d on d.f_id = b.f_doc "
                 "where b.f_store = :f_store and b.f_goods = :f_goods and d.f_state = 1",
                 bind, rows) < 0 || rows.isEmpty()) {
        return 0;
    }
    return rows.at(0).at(0).toDouble();
}

int GiftCartStore::dishForAmount(double amount)
{
    switch(amountKey(amount)) {
    case 10000: return 468;
    case 20000: return 450;
    case 25000: return 472;
    case 30000: return 451;
    case 40000: return 452;
    case 50000: return 453;
    default: return 0;
    }
}

int GiftCartStore::insertDocHeader(Database &db, int docType, double amount, const QString &remarks, QString &error)
{
    QMap<QString, QVariant> bind;
    bind[":f_date"] = QDate::currentDate();
    bind[":f_type"] = docType;
    bind[":f_state"] = 1;
    bind[":f_partner"] = 0;
    bind[":f_inv"] = "";
    bind[":f_invDate"] = QVariant();
    bind[":f_amount"] = amount;
    bind[":f_remarks"] = remarks;
    bind[":f_op"] = Base::fPreferences.getLocal(def_working_user_id).toInt();
    bind[":f_fullDate"] = QDateTime::currentDateTime();
    bind[":f_payment"] = 1;

    const int docId = db.insert("r_docs", bind);
    if(docId <= 0) {
        error = db.fLastError;
        return 0;
    }
    return docId;
}

bool GiftCartStore::createInDoc(Database &db, int toStore, int dishId, double qty, double price,
                                const QString &remarks, QString &error)
{
    const double total = price * qty;
    const int docId = insertDocHeader(db, STORE_DOC_IN, total, remarks, error);
    if(docId <= 0) {
        return false;
    }

    QMap<QString, QVariant> bind;
    bind[":f_doc"] = docId;
    bind[":f_store"] = toStore;
    bind[":f_material"] = dishId;
    bind[":f_sign"] = 1;
    bind[":f_qty"] = qty;
    bind[":f_price"] = price;
    bind[":f_total"] = total;
    bind[":f_vat"] = 0;

    const int bodyId = db.insert("r_body", bind);
    if(bodyId < 0) {
        error = db.fLastError;
        return false;
    }

    bind[":f_doc"] = docId;
    bind[":f_docrow"] = bodyId;
    bind[":f_base"] = bodyId;
    bind[":f_store"] = toStore;
    bind[":f_goods"] = dishId;
    bind[":f_qty"] = qty;
    bind[":f_price"] = price;
    bind[":f_sign"] = 1;

    if(db.insert("r_store_acc", bind) < 0) {
        error = db.fLastError;
        return false;
    }

    return true;
}

bool GiftCartStore::createOutDoc(Database &db, int fromStore, int dishId, double qty, double price,
                                 const QString &remarks, QString &error)
{
    const double total = price * qty;
    const int docId = insertDocHeader(db, STORE_DOC_OUT, total, remarks, error);
    if(docId <= 0) {
        return false;
    }

    QMap<QString, QVariant> bind;
    bind[":f_doc"] = docId;
    bind[":f_store"] = fromStore;
    bind[":f_material"] = dishId;
    bind[":f_sign"] = -1;
    bind[":f_qty"] = qty;
    bind[":f_price"] = price;
    bind[":f_total"] = total;
    bind[":f_vat"] = 0;

    const int bodyId = db.insert("r_body", bind);
    if(bodyId < 0) {
        error = db.fLastError;
        return false;
    }

    QMap<int, double> priceList;
    StoreOutput so(db, docId);
    so.output(priceList);

    if(!priceList.contains(bodyId)) {
        error = QObject::tr("Not enough stock in warehouse %1 for material %2.")
                .arg(fromStore).arg(dishId);
        return false;
    }

    bind.clear();
    bind[":f_price"] = priceList[bodyId];
    bind[":f_total"] = priceList[bodyId] * qty;
    db.update("r_body", bind, where_id(bodyId));

    bind.clear();
    bind[":f_amount"] = priceList[bodyId] * qty;
    db.update("r_docs", bind, where_id(docId));

    return true;
}

bool GiftCartStore::createMoveDoc(Database &db, int fromStore, int toStore, int dishId,
                                  double qty, double price, const QString &remarks, QString &error)
{
    const double total = price * qty;
    const int docId = insertDocHeader(db, STORE_DOC_MOVE, total, remarks, error);
    if(docId <= 0) {
        return false;
    }

    QMap<QString, QVariant> bind;
    bind[":f_doc"] = docId;
    bind[":f_store"] = toStore;
    bind[":f_material"] = dishId;
    bind[":f_sign"] = 1;
    bind[":f_qty"] = qty;
    bind[":f_price"] = price;
    bind[":f_total"] = total;
    bind[":f_vat"] = 0;

    const int inBodyId = db.insert("r_body", bind);
    if(inBodyId < 0) {
        error = db.fLastError;
        return false;
    }

    bind.clear();
    bind[":f_doc"] = docId;
    bind[":f_store"] = fromStore;
    bind[":f_material"] = dishId;
    bind[":f_sign"] = -1;
    bind[":f_qty"] = qty;
    bind[":f_price"] = price;
    bind[":f_total"] = total;
    bind[":f_vat"] = 0;

    const int outBodyId = db.insert("r_body", bind);
    if(outBodyId < 0) {
        error = db.fLastError;
        return false;
    }

    bind.clear();
    bind[":f_doc"] = docId;
    bind[":f_docrow"] = inBodyId;
    bind[":f_base"] = inBodyId;
    bind[":f_store"] = toStore;
    bind[":f_goods"] = dishId;
    bind[":f_qty"] = qty;
    bind[":f_price"] = price;
    bind[":f_sign"] = 1;

    if(db.insert("r_store_acc", bind) < 0) {
        error = db.fLastError;
        return false;
    }

    QMap<int, double> priceList;
    StoreOutput so(db, docId);
    so.output(priceList);

    if(!priceList.contains(outBodyId)) {
        const double onHand = storeBalance(db, fromStore, dishId);
        error = QObject::tr("Cannot write off material %1 from warehouse %2 (on hand: %3). "
                            "Check store batches (FIFO) for this goods.")
                .arg(dishId).arg(fromStore).arg(QString::number(onHand, 'f', 3));
        return false;
    }

    bind.clear();
    bind[":f_price"] = priceList[outBodyId];
    bind[":f_total"] = priceList[outBodyId] * qty;
    db.update("r_body", bind, where_id(outBodyId));

    bind.clear();
    bind[":f_amount"] = priceList[outBodyId] * qty;
    db.update("r_docs", bind, where_id(docId));

    return true;
}

bool GiftCartStore::moveStatus(Database &db, int cardId, int newStatus, QString &error)
{
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > rows;

    bind[":f_id"] = cardId;
    if(db.select("select f_code, f_status, f_initialamount, coalesce(f_fiscal, 0) as f_fiscal "
                 "from d_gift_cart where f_id=:f_id", bind, rows) < 0 || rows.isEmpty()) {
        error = QObject::tr("Gift card not found.");
        return false;
    }

    const QString code = rows.at(0).at(0).toString();
    const int oldStatus = rows.at(0).at(1).toInt();
    const double initialAmount = rows.at(0).at(2).toDouble();
    const int fiscal = rows.at(0).at(3).toInt();

    if(oldStatus == newStatus) {
        error = QObject::tr("Status was not changed.");
        return false;
    }

    if(fiscal != 0 && !isSoldStatus(newStatus)) {
        error = QObject::tr("Sold gift card can be moved only to warehouse/status %1.").arg(SOLD_CARD_STATUS);
        return false;
    }

    if(newStatus <= 0) {
        error = QObject::tr("Select a warehouse.");
        return false;
    }

    if(oldStatus <= 0 && isSoldStatus(newStatus)) {
        error = QObject::tr("Cannot mark as sold: card is not in warehouse.");
        return false;
    }

    if(!isSoldStatus(newStatus) && !isWarehouseId(db, newStatus)) {
        error = QObject::tr("Unknown warehouse id %1.").arg(newStatus);
        return false;
    }

    const int dishId = dishForAmount(initialAmount);
    if(dishId == 0) {
        error = QObject::tr("No store material mapped for amount %1.")
                .arg(QString::number(initialAmount, 'f', 0));
        return false;
    }

    bind.clear();
    bind[":f_id"] = dishId;
    rows.clear();
    if(db.select("select f_lastprice from r_dish where f_id=:f_id", bind, rows) < 0 || rows.isEmpty()) {
        error = QObject::tr("Store material %1 not found.").arg(dishId);
        return false;
    }

    const double price = rows.at(0).at(0).toDouble();
    const QString remarks = QObject::tr("Gift card %1").arg(code);

    const int fromStore = (isSoldStatus(newStatus) || oldStatus > 0) ? oldStatus : 0;
    if(fromStore > 0) {
        const double onHand = storeBalance(db, fromStore, dishId);
        if(onHand + 0.0001 < 1.0) {
            error = QObject::tr("Not enough stock in warehouse %1 for material %2 (on hand: %3). "
                                "Card status is the warehouse id; stock must be on that warehouse.")
                    .arg(fromStore)
                    .arg(dishId)
                    .arg(QString::number(onHand, 'f', 3));
            return false;
        }
    }

#ifdef RESORT_AUDIT_LOG
    const QString snapshotBefore = ResortLog::giftCardSnapshotJson(db, cardId);
#endif

    if(!db.fDb.transaction()) {
        error = db.fDb.lastError().text();
        return false;
    }

    bind.clear();
    bind[":f_status"] = newStatus;
    if(!db.update("d_gift_cart", bind, where_id(cardId))) {
        error = db.fLastError;
        db.fDb.rollback();
        return false;
    }

    bool docOk = false;
    if(isSoldStatus(newStatus)) {
        docOk = createMoveDoc(db, oldStatus, newStatus, dishId, 1.0, price, remarks, error);
    } else if(oldStatus <= 0) {
        docOk = createInDoc(db, newStatus, dishId, 1.0, price, remarks, error);
    } else {
        docOk = createMoveDoc(db, oldStatus, newStatus, dishId, 1.0, price, remarks, error);
    }

    if(!docOk) {
        db.fDb.rollback();
        return false;
    }

    if(!db.fDb.commit()) {
        error = db.fDb.lastError().text();
        db.fDb.rollback();
        return false;
    }

#ifdef RESORT_AUDIT_LOG
    const QString snapshotAfter = ResortLog::giftCardSnapshotJson(db, cardId);
    ResortLog::logSnapshot(QStringLiteral("gift_card"), QString::number(cardId),
                           QStringLiteral("gift_card_status"), snapshotBefore, snapshotAfter);
#endif

    return true;
}
