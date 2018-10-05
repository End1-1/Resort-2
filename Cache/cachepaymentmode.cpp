#include "cachepaymentmode.h"

CachePaymentMode *CachePaymentMode::fInstance = 0;

CachePaymentMode::CachePaymentMode() :
    CacheBase()
{
    fCacheId = cid_payment_mode;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CachePaymentMode::~CachePaymentMode()
{
    fInstance = 0;
}

CachePaymentMode *CachePaymentMode::instance()
{
    if (!fInstance) {
        fInstance = new CachePaymentMode();
        fCacheOne[cid_payment_mode] = fInstance;
    }
    return fInstance;
}

void CachePaymentMode::load()
{
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select("select f_id, f_" + def_lang + " from f_payment_type");
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_PaymentMode  *ci = new CI_PaymentMode();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    fDb.close();
    delete q;
}
