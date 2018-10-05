#ifndef CACHERESTDISHMOD_H
#define CACHERESTDISHMOD_H

#include "cachebase.h"

#define cid_rest_dish_mod 32

typedef struct : CI_Base {
} CI_RestDishMod;
Q_DECLARE_METATYPE(CI_RestDishMod*)

class CacheRestDishMod : public CacheBase<CI_RestDishMod>
{
public:
    static CacheRestDishMod *instance();
    virtual void load();
private:
    static CacheRestDishMod *fInstance;
    CacheRestDishMod();
    ~CacheRestDishMod();
};

#endif // CACHERESTDISHMOD_H
