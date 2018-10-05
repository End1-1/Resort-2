#include "cachecreditcard.h"

CacheCreditCard *CacheCreditCard::fInstance;

CacheCreditCard *CacheCreditCard::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheCreditCard();
        fCacheOne[cid_credit_card] = fInstance;
    }
    return fInstance;
}

CacheCreditCard::CacheCreditCard() :
    CacheBase()
{
    fCacheId = cid_credit_card;
    fQuery = "select f_id, f_name from f_credit_card";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCreditCard::~CacheCreditCard()
{
    fInstance = 0;
}
