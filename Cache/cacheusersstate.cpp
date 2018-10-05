#include "cacheusersstate.h"

CacheUsersState *CacheUsersState::fInstance = 0;

CacheUsersState *CacheUsersState::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheUsersState();
        fCacheOne[cid_users_state] = fInstance;
    }
    return fInstance;
}

CacheUsersState::CacheUsersState() :
    CacheBase()
{
    fCacheId = cid_users_state;
    fQuery = "select f_id, f_" + def_lang + " from users_states";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheUsersState::~CacheUsersState()
{
    fInstance = 0;
}
