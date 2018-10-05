#include "cacheroominventory.h"

CacheRoomInventory *CacheRoomInventory::fInstance = 0;

CacheRoomInventory *CacheRoomInventory::instance()
{
    if (!fInstance) {
        fInstance = new CacheRoomInventory();
        fCacheOne[cid_room_inventory] = fInstance;
    }
    return fInstance;
}

CacheRoomInventory::CacheRoomInventory()
{
    fCacheId = cid_room_inventory;
    fQuery = "select f_id, f_name from f_room_inventory";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoomInventory::~CacheRoomInventory()
{
    fInstance = 0;
}
