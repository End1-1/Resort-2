#ifndef CACHERESERVESTATE_H
#define CACHERESERVESTATE_H

#include "cachebase.h"

#define cid_reserve_state 35
#define CACHERESERVESTATE_NAME(x) CacheReserveState::instance()->get(x)->fName

typedef struct : CI_Base {

} CI_ReserveState;
Q_DECLARE_METATYPE(CI_ReserveState*)

class CacheReserveState : public CacheBase<CI_ReserveState>
{
public:
    static CacheReserveState *instance();
    virtual void load();
private:
    static CacheReserveState *fInstance;
    CacheReserveState();
    ~CacheReserveState();
};

#endif // CACHERESERVESTATE_H
