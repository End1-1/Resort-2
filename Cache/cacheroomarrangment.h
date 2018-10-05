#ifndef CACHEROOMARRANGMENT_H
#define CACHEROOMARRANGMENT_H

#include "cachebase.h"

#define cid_room_arrangment 37

typedef struct : CI_Base {

} CI_RoomArrangment;
Q_DECLARE_METATYPE(CI_RoomArrangment*)

class CacheRoomArrangment : public CacheBase<CI_RoomArrangment>
{
public:
    static CacheRoomArrangment *instance();
    virtual void load();
private:
    static CacheRoomArrangment *fInstance;
    CacheRoomArrangment();
    ~CacheRoomArrangment();
};

#endif // CACHEROOMARRANGMENT_H
