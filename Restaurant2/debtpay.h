#ifndef DEBTPAY_H
#define DEBTPAY_H

#include <QDateTime>
#include <QList>
#include <QString>

struct OpenDebtRow
{
    int orderId = 0;
    QString govNumber;
    QDateTime dateTime;
    double balance = 0;
};

class DebtPay
{
public:
    static const int PAYMENT_CASH = 1;
    static const int PAYMENT_CARD = 2;
    static const int PAYMENT_TALON = 3;
    static const int PAYMENT_PREPAID = 4;

    static QString normalizeGovNumber(const QString &govNumber);
    static bool loadOpenDebts(const QString &govNumber, QList<OpenDebtRow> &rows);
    static bool loadAllOpenDebts(int branch, QList<OpenDebtRow> &rows);
    static bool payDebt(int orderId,
                        const QString &govNumber,
                        double balance,
                        int paymentMode,
                        const QString &talonCode,
                        QString &error);
    static bool printOrderFiscalIfNeeded(int orderId,
                                         double amount,
                                         int paymentMode,
                                         QString &error);
    static QString paymentModeName(int paymentMode);
};

#endif // DEBTPAY_H
