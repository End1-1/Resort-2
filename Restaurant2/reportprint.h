#ifndef REPORTPRINT_H
#define REPORTPRINT_H

#include "base.h"

class ReportPrint : public QObject, public Base
{
    Q_OBJECT
public:
    ReportPrint();
    static void printTotal(const QDate &date, const QString &printedBy, const QString &prn,
                           const QString &reportTitle = QString());
    static void printSessionCloseTotal(const QString &printedBy, const QString &prn);
    static void printTotalShort(const QDate &date, const QString &printedBy, const QString &prn);
    static void printDebtPaymentReceipt(int orderId,
                                        const QString &govNumber,
                                        double amount,
                                        int paymentMode);
    static double totalx500(const QDate &date);

private:
    static QString debtPaymentModeName(int paymentMode);
};

#endif // REPORTPRINT_H
