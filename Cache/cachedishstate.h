#ifndef CACHEDISHSTATE_H
#define CACHEDISHSTATE_H

#include "cachebase.h"

#define cid_dish_state 62

typedef struct : CI_Base {

} CI_DishState;
Q_DECLARE_METATYPE(CI_DishState*)

class CacheDishState : public CacheBase<CI_DishState>
{
public:
    static CacheDishState *instance();
private:
    static CacheDishState *fInstance;
    CacheDishState();
    ~CacheDishState();
};

#endif // CACHEDISHSTATE_H
