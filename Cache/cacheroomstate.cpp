#include "cacheroomstate.h"

CacheRoomState *CacheRoomState::fInstance;

CacheRoomState *CacheRoomState::instance()
{
    if (!fInstance) {
        fInstance = new CacheRoomState();
        fCacheOne[cid_room_state] = fInstance;
    }
    return fInstance;
}

CacheRoomState::CacheRoomState() :
    CacheBase()
{
    fCacheId = cid_room_state;
    fQuery = "select f_id, f_" + def_lang + " from f_room_state";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoomState::~CacheRoomState()
{
    fInstance = 0;
}
