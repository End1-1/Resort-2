#ifndef CACHEROOMINVENTORY_H
#define CACHEROOMINVENTORY_H

#include "cachebase.h"

#define cid_room_inventory 115

typedef struct : CI_Base {

} CI_RoomInventory;
Q_DECLARE_METATYPE (CI_RoomInventory*)

class CacheRoomInventory : public CacheBase <CI_RoomInventory>
{
public:
    static CacheRoomInventory *instance();
private:
    CacheRoomInventory();
    ~CacheRoomInventory();
    static CacheRoomInventory *fInstance;
};

#endif // CACHEROOMINVENTORY_H
