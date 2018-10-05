#include "cachereservegroup.h"

CacheReserveGroup *CacheReserveGroup::fInstance = 0;

CacheReserveGroup *CacheReserveGroup::instance()
{
    if (!fInstance) {
        fInstance = new CacheReserveGroup();
        fCacheOne[cid_reserve_group] = fInstance;
    }
    return fInstance;
}

void CacheReserveGroup::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_ReserveGroup *c = new CI_ReserveGroup();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheReserveGroup::CacheReserveGroup() :
    CacheBase()
{
    fCacheId = cid_reserve_group;
    fQuery = "select f_id, f_name from f_reservation_group";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}


CacheReserveGroup::~CacheReserveGroup()
{
    fInstance = 0;
}

