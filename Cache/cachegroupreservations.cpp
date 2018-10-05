#include "cachegroupreservations.h"

CacheGroupReservations *CacheGroupReservations::fInstance = 0;

CacheGroupReservations *CacheGroupReservations::instance()
{
    if (!fInstance) {
        fInstance = new CacheGroupReservations();
        fCacheOne[cid_group_reservation] = fInstance;
    }
    return fInstance;
}

void CacheGroupReservations::updateItem(const QString &id)
{
    CI_GroupReservation *ci = get(id);
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + QString(id));
    if (q->next()) {
        if (!ci) {
            ci = new CI_GroupReservation();
            ci->fCode = q->value(0).toString();
            ci->fName = q->value(1).toString();
            fStaticCache[fCacheId][ci->fCode] = ci;
        }
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
}

CacheGroupReservations::CacheGroupReservations()
{
    fCacheId = cid_group_reservation;
    fQuery = "select f_id, f_name from f_reservation_group";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheGroupReservations::~CacheGroupReservations()
{
    fInstance = 0;
}
