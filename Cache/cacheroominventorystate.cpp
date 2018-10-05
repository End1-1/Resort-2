#include "cacheroominventorystate.h"

CacheRoomInventoryState *CacheRoomInventoryState::fInstance = 0;

CacheRoomInventoryState *CacheRoomInventoryState::instance()
{
    if (!fInstance) {
        fInstance = new CacheRoomInventoryState();
        fCacheOne[cid_room_inventory_state] = fInstance;
    }
    return fInstance;
}

CacheRoomInventoryState::CacheRoomInventoryState()
{
    fCacheId = cid_room_inventory_state;
    fQuery = "select f_id, f_name from f_room_inventory_state ";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoomInventoryState::~CacheRoomInventoryState()
{
    fInstance = 0;
}
