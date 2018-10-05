#include "cacheresttable.h"

CacheRestTable *CacheRestTable::fInstance = 0;

CacheRestTable::CacheRestTable() :
    CacheBase()
{
    fQuery = "select f_id, f_name from r_table";
    fCacheId = cid_rest_table;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestTable::~CacheRestTable()
{
    fInstance = 0;
}

CacheRestTable *CacheRestTable::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestTable();
        fCacheOne[cid_rest_table] = fInstance;
    }
    return fInstance;
}
