#include "cacheroomview.h"

CacheRoomView *CacheRoomView::fInstance = 0;

CacheRoomView *CacheRoomView::instance()
{
    if (!fInstance) {
        fInstance = new CacheRoomView();
        fCacheOne[cid_room_view] = fInstance;
    }
    return fInstance;
}

CacheRoomView::CacheRoomView() :
    CacheBase()
{
    fCacheId = cid_room_view;
    fQuery = "select f_id, f_short from f_room_view";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoomView::~CacheRoomView()
{
    fInstance = 0;
}
