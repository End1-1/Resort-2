#include "cacheorderstate.h"

CacheOrderState *CacheOrderState::fInstance = 0;

CacheOrderState *CacheOrderState::instance()
{
    if (!fInstance) {
        fInstance = new CacheOrderState();
        fCacheOne[cid_order_state] = fInstance;
    }
    return fInstance;
}

CacheOrderState::CacheOrderState()
{
    fQuery = "select f_id, f_" + def_lang + " from o_state";
    fCacheId = cid_order_state;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheOrderState::~CacheOrderState()
{
    fInstance = 0;
}
