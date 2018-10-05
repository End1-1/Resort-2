#include "cachediscounttype.h"

CacheDiscountType *CacheDiscountType::fInstance =0;

CacheDiscountType *CacheDiscountType::instance()
{
    if (!fInstance) {
        fInstance = new CacheDiscountType();
        fCacheOne[cid_discount_type] = fInstance;
    }
    return fInstance;
}

CacheDiscountType::CacheDiscountType()
{
    fQuery = "select f_id, f_name from f_discount_type";
    fCacheId = cid_discount_type;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheDiscountType::~CacheDiscountType()
{
    fInstance = 0;
}
