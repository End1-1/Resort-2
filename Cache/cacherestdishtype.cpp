#include "cacherestdishtype.h"

CacheRestDishType *CacheRestDishType::fInstance = 0;

CacheRestDishType::CacheRestDishType() :
    CacheBase()
{
    fCacheId = cid_rest_dish_type;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestDishType::~CacheRestDishType()
{
    fInstance = 0;
}

CacheRestDishType *CacheRestDishType::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestDishType();
        fCacheOne[cid_rest_dish_type] = fInstance;
    }
    return fInstance;
}

void CacheRestDishType::load()
{
    QString query = "select f_id, f_" + def_lang + " from r_dish_type";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_RestDishType *c = new CI_RestDishType();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    fDb.close();
    delete q;
}

void CacheRestDishType::updateItem(const QString &id)
{
    QString query = "select f_id, f_" + def_lang + " from r_dish_type where f_id=" + id;
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    CI_RestDishType *c = get(id);
    if (q->next()) {
        if (!c) {
            c = new CI_RestDishType();
        }
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    fDb.close();
    delete q;
}
