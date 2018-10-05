#include "cachestoredoc.h"

CacheStoreDocType *CacheStoreDocType::fInstance = 0;

CacheStoreDocType::CacheStoreDocType() :
    CacheBase()
{
    fCacheId = cid_store_doc_type;
    fQuery = "select f_id, f_name from r_doc_type";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheStoreDocType *CacheStoreDocType::instance()
{
    if (!fInstance) {
        fInstance = new CacheStoreDocType();
        fCacheOne[cid_store_doc_type] = fInstance;
    }
    return fInstance;
}

void CacheStoreDocType::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_StoreDocType *c = new CI_StoreDocType();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheStoreDocType::~CacheStoreDocType()
{
    fInstance = 0;
}
