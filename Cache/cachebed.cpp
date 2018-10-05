#include "cachebed.h"

CacheBed *CacheBed::fInstance;

CacheBed *CacheBed::instance()
{
    if (!fInstance) {
        fInstance = new CacheBed();
        fCacheOne[cid_bed] = fInstance;
    }
    return fInstance;
}

CacheBed::CacheBed() :
    CacheBase()
{
    fCacheId = cid_bed;
    fQuery = "select f_id, f_name  from f_room_bed";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheBed::~CacheBed()
{
    fInstance = 0;
}
