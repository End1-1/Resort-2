#include "cachsex.h"

CachSex *CachSex::fInstance = 0;

CachSex *CachSex::instance()
{
    if (!fInstance) {
        fInstance = new CachSex();
        fCacheOne[cid_sex] = fInstance;
    }
    return fInstance;
}

CachSex::CachSex() :
    CacheBase()
{
    fCacheId = cid_sex;
    fQuery = "select f_id, f_" + def_lang + " from f_guests_sex";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CachSex::~CachSex()
{
    fInstance = 0;
}
