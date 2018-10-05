#include "cacherestdishmod.h"

CacheRestDishMod *CacheRestDishMod::fInstance = 0;

CacheRestDishMod::CacheRestDishMod() :
    CacheBase()
{
    fQuery = "select f_id, f_" + def_lang + " from r_dish_mod";
    fCacheId = cid_rest_dish_mod;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestDishMod::~CacheRestDishMod()
{
    fInstance = 0;
}

CacheRestDishMod *CacheRestDishMod::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestDishMod();
        fCacheOne[cid_rest_dish_mod] = fInstance;
    }
    return fInstance;
}

void CacheRestDishMod::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestDishMod *c = new CI_RestDishMod();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}
