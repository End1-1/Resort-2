#include "cachecashdoc.h"

CacheCashDoc *CacheCashDoc::fInstance = 0;

CacheCashDoc *CacheCashDoc::instance()
{
    if (!fInstance) {
        fInstance = new CacheCashDoc();
        fCacheOne[cid_cash_doc] = fInstance;
    }
    return fInstance;
}

CacheCashDoc::CacheCashDoc() :
    CacheBase()
{
    fQuery = "select f_id, f_name from c_doc_type";
    fCacheId = cid_cash_doc;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCashDoc::~CacheCashDoc()
{
    fInstance = 0;
}
