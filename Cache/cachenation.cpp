#include "cachenation.h"

CacheNation *CacheNation::fInstance = 0;

CacheNation *CacheNation::instance()
{
    if (!fInstance) {
        fInstance = new CacheNation();
        fCacheOne[cid_nation] = fInstance;
    }
    return fInstance;
}

CacheNation::CacheNation() :
    CacheBase()
{
    fCacheId = cid_nation;
    fQuery = "select f_short, f_name from f_nationality";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheNation::~CacheNation()
{
    fInstance = 0;
}
