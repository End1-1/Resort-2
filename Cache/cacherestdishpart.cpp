#include "cacherestdishpart.h"

CacheRestDishPart *CacheRestDishPart::fInstance = 0;

CacheRestDishPart::CacheRestDishPart() :
    CacheBase()
{
    fCacheId = cid_rest_dish_part;
    fQuery = "select f_id, f_" + def_lang + " from r_dish_part";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestDishPart::~CacheRestDishPart()
{
    fInstance = 0;
}

CacheRestDishPart *CacheRestDishPart::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestDishPart();
        fCacheOne[cid_rest_dish_part] = fInstance;
    }
    return fInstance;
}

void CacheRestDishPart::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestDishPart *c = new CI_RestDishPart();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

void CacheRestDishPart::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_RestDishPart *c = 0;
    if (q->next()) {
        if (fStaticCache[fCacheId].contains(q->value(0).toString())) {
            c = fStaticCache[fCacheId][q->value(0).toString()];
        } else {
            c = new CI_RestDishPart();
        }
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    closeDb(q);
}
