#include "cacheunit.h"

CacheUnit *CacheUnit::fInstance = 0;

CacheUnit *CacheUnit::instance()
{
    if (!fInstance) {
        fInstance = new CacheUnit();
        fCacheOne[cid_unit] = fInstance;
    }
    return fInstance;
}

CacheUnit::CacheUnit()
{
    fQuery = "select f_id, f_name from r_unit";
    fCacheId = cid_unit;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheUnit::~CacheUnit()
{
    fInstance = 0;
}
