#ifndef CACHEDISHSTATE_H
#define CACHEDISHSTATE_H

#include "cachebase.h"

#define cid_dish_state 62

struct CI_DishState : public CI_Base
{};
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
