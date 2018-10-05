#include "cachedocpayment.h"

CacheDocPayment *CacheDocPayment::fInstance = 0;

CacheDocPayment *CacheDocPayment::instance()
{
    if (!fInstance) {
        fInstance = new CacheDocPayment();
        fCacheOne[cid_doc_payment] = fInstance;
    }
    return fInstance;
}

CacheDocPayment::CacheDocPayment() :
    CacheBase()
{
    fCacheId = cid_doc_payment;
    fQuery = "select f_id, f_name from r_doc_payment order by f_id";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheDocPayment::~CacheDocPayment()
{
    fInstance = 0;
}
