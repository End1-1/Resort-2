#ifndef CACHEPAYMENTMODE_H
#define CACHEPAYMENTMODE_H

#include "cachebase.h"

#define cid_payment_mode 11

typedef struct h : CI_Base {
} CI_PaymentMode;
Q_DECLARE_METATYPE(CI_PaymentMode*)

class CachePaymentMode : public CacheBase<CI_PaymentMode>
{
public:
    static CachePaymentMode *instance();
    virtual void load();
private:
    static CachePaymentMode *fInstance;
    CachePaymentMode();
    ~CachePaymentMode();
};

#endif // CACHEPAYMENTMODE_H
