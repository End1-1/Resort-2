#include "cachereststore.h"

CacheRestStore *CacheRestStore::fInstance = 0;

CacheRestStore::CacheRestStore() :
    CacheBase()
{
    fQuery = "select f_id, f_name from r_store where f_state=1 order by f_name ";
    fCacheId = cid_rest_store;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestStore::~CacheRestStore()
{
    fInstance = 0;
}

CacheRestStore *CacheRestStore::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestStore();
        fCacheOne[cid_rest_store] = fInstance;
    }
    return fInstance;
}

void CacheRestStore::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestStore *c = new CI_RestStore();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}
