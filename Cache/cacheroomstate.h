#ifndef CACHEROOMSTATE_H
#define CACHEROOMSTATE_H

#include "cachebase.h"

#define cid_room_state 46

typedef struct : CI_Base {

} CI_RoomState;
Q_DECLARE_METATYPE(CI_RoomState*)

class CacheRoomState : public CacheBase<CI_RoomState>
{
public:
    static CacheRoomState *instance();
private:
    static CacheRoomState *fInstance;
    CacheRoomState();
    ~CacheRoomState();
};

#endif // CACHEROOMSTATE_H
