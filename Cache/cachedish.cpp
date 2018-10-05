#include "cachedish.h"

CacheDish *CacheDish::fInstance = 0;

CacheDish *CacheDish::instance()
{
    if (!fInstance) {
        fInstance = new CacheDish();
        fCacheOne[cid_dish] = fInstance;
    }
    return fInstance;
}

void CacheDish::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_Dish *ci = new CI_Dish();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fUnitName = q->value(2).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

CacheDish::CacheDish()
{
    fQuery = "select d.f_id, f_" + def_lang + ", u.f_name from r_dish d left join r_unit u on u.f_id=d.f_unit ";
    fCacheId = cid_dish;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheDish::~CacheDish()
{
    fInstance = 0;
}
