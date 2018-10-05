#include "cachetrackingtables.h"

CacheTrackingTables *CacheTrackingTables::fInstance = 0;

CacheTrackingTables *CacheTrackingTables::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheTrackingTables();
        fCacheOne[cid_tracking_tables] = fInstance;
    }
    return fInstance;
}

void CacheTrackingTables::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_TrackingTables *c = new CI_TrackingTables();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheTrackingTables::CacheTrackingTables() :
    CacheBase()
{
    fCacheId = cid_tracking_tables;
    fQuery = "select f_id, f_name from f_changes_windows";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheTrackingTables::~CacheTrackingTables()
{
    fInstance = 0;
}
