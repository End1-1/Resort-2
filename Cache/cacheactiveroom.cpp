#include "cacheactiveroom.h"

CacheActiveRoom *CacheActiveRoom::fInstance = 0;

CacheActiveRoom::CacheActiveRoom() :
    CacheBase()
{
    fCacheId = cid_active_room;
    fQuery = "select r.f_room, rm.f_short, r.f_invoice, concat(g.f_title, ' ' , g.f_firstName, ' ' , g.f_lastName), "
                     "r.f_id "
                    "from f_reservation r "
                    "inner join f_room rm on rm.f_id=r.f_room "
                    "inner join f_guests g on g.f_id=r.f_guest "
                    "where r.f_state=" + QString::number(RESERVE_CHECKIN);
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheActiveRoom::~CacheActiveRoom()
{
    fInstance = 0;
}

CacheActiveRoom *CacheActiveRoom::instance()
{
    if (!fInstance) {
        fInstance = new CacheActiveRoom();
        fCacheOne[cid_active_room] = fInstance;
    }
    return fInstance;
}

void CacheActiveRoom::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_ActiveRoom *r = new CI_ActiveRoom();
        r->fCode = q->value(4).toString();
        r->fName = q->value(1).toString();
        r->fRoomCode = q->value(0).toString();
        r->fRoomName = q->value(1).toString();
        r->fInvoice = q->value(2).toString();
        r->fGuestName = q->value(3).toString();
        fStaticCache[fCacheId][r->fRoomCode] = r;
    }
    closeDb(q);
}

void CacheActiveRoom::updateItem(const QString &id)
{
    CI_ActiveRoom *r = get(id);
    QSqlQuery *q = prepareDb(fQuery + " and r.f_room=" + id);
    if (q->next()) {
        if (!r) {
            r = new CI_ActiveRoom();
        }
        r->fCode = q->value(4).toString();
        r->fName = q->value(1).toString();
        r->fRoomCode = q->value(0).toString();
        r->fRoomName = q->value(1).toString();
        r->fInvoice = q->value(2).toString();
        r->fGuestName = q->value(3).toString();
        r->fValid = true;
        fStaticCache[fCacheId][r->fRoomCode] = r;
    } else {
        if (r) {
            r->fValid = false;
        }
    }
    closeDb(q);
}
