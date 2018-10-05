#ifndef CACHERESERVEGROUP_H
#define CACHERESERVEGROUP_H

#include "cachebase.h"

#define cid_reserve_group 100

typedef struct : CI_Base {

} CI_ReserveGroup;
Q_DECLARE_METATYPE(CI_ReserveGroup*)

class CacheReserveGroup : public CacheBase<CI_ReserveGroup>
{
public:
    static CacheReserveGroup *instance();
    virtual void load();
private:
    static CacheReserveGroup *fInstance;
    CacheReserveGroup();
    ~CacheReserveGroup();
};

#endif // CACHERESERVEGROUP_H
