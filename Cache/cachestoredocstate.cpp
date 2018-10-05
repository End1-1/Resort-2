#include "cachestoredocstate.h"

CacheStoreDocState *CacheStoreDocState::fInstance = 0;

CacheStoreDocState *CacheStoreDocState::instance()
{
    if (!fInstance) {
        fInstance = new CacheStoreDocState();
        fCacheOne[cid_store_doc_state] = fInstance;
    }
    return fInstance;
}

CacheStoreDocState::CacheStoreDocState() :
    CacheBase()
{
    fCacheId = cid_store_doc_state;
    fQuery = "select f_id, f_name from r_doc_state";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheStoreDocState::~CacheStoreDocState()
{
    fInstance = 0;
}
