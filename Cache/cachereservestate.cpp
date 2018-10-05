#include "cachereservestate.h"

CacheReserveState *CacheReserveState::fInstance = 0;

CacheReserveState *CacheReserveState::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheReserveState();
        fCacheOne.insert(cid_reserve_state, fInstance);
    }
    return fInstance;
}

void CacheReserveState::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_ReserveState *c = new CI_ReserveState();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheReserveState::CacheReserveState() :
    CacheBase()
{
    fCacheId = cid_reserve_state;
    fQuery = "select f_id, f_" + def_lang + " from f_reservation_state";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheReserveState::~CacheReserveState()
{
    fInstance = 0;
}
