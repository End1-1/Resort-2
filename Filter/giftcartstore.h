#ifndef GIFTCARTSTORE_H
#define GIFTCARTSTORE_H

#include "database.h"
#include <QString>

class GiftCartStore
{
public:
    /** d_gift_cart.f_status = warehouse id (same as r_store.f_id); labels in d_gift_cart_statuses use the same f_id. */
    static const int SOLD_CARD_STATUS = 28;

    static int dishForAmount(double amount);
    static bool moveStatus(Database &db, int cardId, int newStatus, QString &error);
    static double storeBalance(Database &db, int storeId, int goodsId);
    static bool isSoldStatus(int status);
    static bool isWarehouseId(Database &db, int storeId);

private:
    static int insertDocHeader(Database &db, int docType, double amount, const QString &remarks, QString &error);
    static bool createInDoc(Database &db, int toStore, int dishId, double qty, double price,
                            const QString &remarks, QString &error);
    static bool createOutDoc(Database &db, int fromStore, int dishId, double qty, double price,
                             const QString &remarks, QString &error);
    static bool createMoveDoc(Database &db, int fromStore, int toStore, int dishId,
                              double qty, double price, const QString &remarks, QString &error);
};

#endif // GIFTCARTSTORE_H
