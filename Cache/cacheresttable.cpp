#include "cacheresttable.h"

CacheRestTable *CacheRestTable::fInstance = 0;

CacheRestTable::CacheRestTable() :
    CacheBase()
{
    fQuery = "select f_id, f_hall, f_name from r_table";
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

void CacheRestTable::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestTable *ci = new CI_RestTable();
        ci->fCode = q->value(0).toString();
        ci->fHall = q->value(1).toInt();
        ci->fName = q->value(2).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}
