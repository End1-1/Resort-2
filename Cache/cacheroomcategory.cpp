#include "cacheroomcategory.h"

CacheRoomCategory *CacheRoomCategory::fInstance = 0;

CacheRoomCategory *CacheRoomCategory::instance()
{
    if (!fInstance) {
        fInstance = new CacheRoomCategory();
        fCacheOne[cid_room_category] = fInstance;
    }
    return fInstance;
}

void CacheRoomCategory::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RoomCategory *c = new CI_RoomCategory();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fFullName = q->value(2).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheRoomCategory::CacheRoomCategory() :
    CacheBase()
{
    fCacheId = cid_room_category;
    fQuery = "select f_id, f_short, f_description from f_room_classes";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoomCategory::~CacheRoomCategory()
{
    fInstance = 0;
}
