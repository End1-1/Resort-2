#ifndef CACHETRACKINGTABLES_H
#define CACHETRACKINGTABLES_H

#include "cachebase.h"

#define cid_tracking_tables 38

typedef struct : CI_Base {

} CI_TrackingTables;
Q_DECLARE_METATYPE(CI_TrackingTables*)

class CacheTrackingTables : public CacheBase<CI_TrackingTables>
{
public:
    static CacheTrackingTables *instance();
    virtual void load();
private:
    static CacheTrackingTables *fInstance;
    CacheTrackingTables();
    ~CacheTrackingTables();
};

#endif // CACHETRACKINGTABLES_H
