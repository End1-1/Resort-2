#ifndef CACHECHECKOUTINVOICE_H
#define CACHECHECKOUTINVOICE_H

#include "cachebase.h"
#include <QMap>

#define cid_checkout_invoice 63

typedef struct : CI_Base {
    QString fReservation;
    QString fRoom;
    QString fGuest;
    QString fEntry;
    QDate fDeparture;
    QString fAmount;
    QString fOperator;
} CI_CheckoutInvoice;
Q_DECLARE_METATYPE(CI_CheckoutInvoice*)

class CacheCheckoutInvoice : public CacheBase<CI_CheckoutInvoice>
{
public:
    static CacheCheckoutInvoice *instance();
    virtual void load();
    //virtual void updateItem(const QString &id);
private:
    static CacheCheckoutInvoice *fInstance;
    CacheCheckoutInvoice();
    ~CacheCheckoutInvoice();
    void loadRecord(QSqlQuery *q);
};

#endif // CACHECHECKOUTINVOICE_H
