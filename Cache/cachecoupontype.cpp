#include "cachecoupontype.h"

CacheCouponType *CacheCouponType::fInstance = 0;

CacheCouponType *CacheCouponType::instance()
{
    if (!fInstance) {
        fInstance = new CacheCouponType();
        fCacheOne[cid_coupon_type] = fInstance;
    }
    return fInstance;
}

CacheCouponType::CacheCouponType() :
    CacheBase()
{
    fCacheId = cid_coupon_type;
    fQuery = "select f_id, f_name from d_type";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCouponType::~CacheCouponType()
{
    fInstance = 0;
}
