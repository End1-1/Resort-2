#ifndef SESSIONDEBTCLOSE_H
#define SESSIONDEBTCLOSE_H

#include "database.h"
#include <QList>
#include <QString>

struct SessionDebtOrder
{
    int orderId = 0;
    int tableId = 0;
    QString tableName;
    int hallId = 0;
    double total = 0;
    int customerId = 0;
    QString govNumber;
};

class SessionDebtClose
{
public:
    static bool loadOpenOrders(int branch, QList<SessionDebtOrder> &orders);
    static bool validateOrders(const QList<SessionDebtOrder> &orders, QString &error);
    static bool closeOrdersAsDebt(Database &db, const QList<SessionDebtOrder> &orders, int staffId, QString &error);
};

#endif // SESSIONDEBTCLOSE_H
