#include "cacheroomarrangment.h"

CacheRoomArrangment *CacheRoomArrangment::fInstance = 0;

CacheRoomArrangment *CacheRoomArrangment::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheRoomArrangment();
        fCacheOne[cid_room_arrangment] = fInstance;
    }
    return fInstance;
}

void CacheRoomArrangment::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RoomArrangment *c = new CI_RoomArrangment();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
}

CacheRoomArrangment::CacheRoomArrangment() :
    CacheBase()
{
    fCacheId = cid_room_arrangment;
    fQuery = "select f_id, f_" + def_lang + " from f_room_arrangement";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRoomArrangment::~CacheRoomArrangment()
{
    fInstance = 0;
}
