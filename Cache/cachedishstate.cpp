#include "cachedishstate.h"

CacheDishState *CacheDishState::fInstance = 0;

CacheDishState *CacheDishState::instance()
{
    if (!fInstance) {
        fInstance = new CacheDishState();
        fCacheOne[cid_dish_state] = fInstance;
    }
    return fInstance;
}

CacheDishState::CacheDishState()
{
    fQuery = "select f_id, f_" + def_lang + " from o_dish_state";
    fCacheId = cid_dish_state;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheDishState::~CacheDishState()
{
    fInstance = 0;
}
