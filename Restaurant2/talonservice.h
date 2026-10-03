#ifndef TALONSERVICE_H
#define TALONSERVICE_H

#include <QString>

class Database2;

struct TalonRedeemInfo
{
    QString code;
    int partnerId = 0;
    QString partnerName;
    double price = 0;
};

class TalonService
{
public:
    static QString normalizeCode(const QString &raw);
    static bool talonExists(const QString &normalizedCode);
    static bool lookupForRedeem(const QString &rawCode, TalonRedeemInfo &info, QString &error);
    static bool loadRedeemedForOrder(int orderId, TalonRedeemInfo &info);
    static bool redeemForOrder(int orderId, const QString &rawCode, TalonRedeemInfo &info, QString &error);
    static bool redeemForOrderInTx(Database2 &db2,
                                 int orderId,
                                 const QString &normalizedCode,
                                 TalonRedeemInfo &info,
                                 QString &error);

private:
    static bool loadTalonForRedeem(Database2 &db2,
                                     const QString &normalizedCode,
                                     TalonRedeemInfo &info,
                                     QString &error);
};

#endif // TALONSERVICE_H
