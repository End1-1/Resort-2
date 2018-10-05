#ifndef CACHERESERVESTATUS_H
#define CACHERESERVESTATUS_H

#include "cachebase.h"

#define cid_reserve_status 36
#define CACHERESERVESTATUS_NAME(x) CacheReserveStatus::instance()->get(x)->fName

typedef struct : CI_Base {

} CI_ReserveStatus;
Q_DECLARE_METATYPE(CI_ReserveStatus*)

class CacheReserveStatus : public CacheBase<CI_ReserveStatus>
{
public:
    static CacheReserveStatus *instance();
    virtual void load();
private:
    static CacheReserveStatus *fInstance;
    CacheReserveStatus();
    ~CacheReserveStatus();
};

#endif // CACHERESERVESTATUS_H
