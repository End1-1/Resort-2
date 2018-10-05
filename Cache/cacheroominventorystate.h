#ifndef CACHEROOMINVENTORYSTATE_H
#define CACHEROOMINVENTORYSTATE_H

#include "cachebase.h"

#define cid_room_inventory_state 116

typedef struct : CI_Base {

} CI_RoomInventoryState;
Q_DECLARE_METATYPE(CI_RoomInventoryState*)

class CacheRoomInventoryState : public CacheBase<CI_RoomInventoryState>
{
public:
    static CacheRoomInventoryState *instance();
private:
    CacheRoomInventoryState();
    ~CacheRoomInventoryState();
    static CacheRoomInventoryState *fInstance;
};

#endif // CACHEROOMINVENTORYSTATE_H
