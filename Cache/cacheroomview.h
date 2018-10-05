#ifndef CACHEROOMVIEW_H
#define CACHEROOMVIEW_H

#include "cachebase.h"

#define cid_room_view 55

typedef struct : CI_Base {

} CI_RoomView;
Q_DECLARE_METATYPE(CI_RoomView*)

class CacheRoomView : public CacheBase<CI_RoomView>
{
public:
    static CacheRoomView *instance();
private:
    static CacheRoomView *fInstance;
    CacheRoomView();
    ~CacheRoomView();
};

#endif // CACHEROOMVIEW_H
