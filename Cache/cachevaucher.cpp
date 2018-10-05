#include "cachevaucher.h"

CacheVaucher *CacheVaucher::fInstance = 0;

CacheVaucher *CacheVaucher::instance()
{
    if (!fInstance) {
        fInstance = new CacheVaucher();
        fCacheOne[cid_vaucher] = fInstance;
    }
    return fInstance;
}

CacheVaucher::CacheVaucher()
{
    fQuery = "select f_code, f_description from m_vaucher";
    fCacheId = cid_vaucher;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheVaucher::~CacheVaucher()
{
    fInstance = 0;
}
