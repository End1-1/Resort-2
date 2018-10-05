#include "cachereservestatus.h"

CacheReserveStatus *CacheReserveStatus::fInstance = 0;

CacheReserveStatus *CacheReserveStatus::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheReserveStatus();
        fCacheOne[cid_reserve_status] = fInstance;
    }
    return fInstance;
}

void CacheReserveStatus::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_ReserveStatus *c = new CI_ReserveStatus();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheReserveStatus::CacheReserveStatus() :
    CacheBase()
{
    fCacheId = cid_reserve_status;
    fQuery = "select f_id, f_" + def_lang + " from f_reservation_status";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheReserveStatus::~CacheReserveStatus()
{
    fInstance = 0;
}
