#include "cacheresthall.h"

CacheRestHall *CacheRestHall::fInstance = 0;

CacheRestHall::CacheRestHall() :
    CacheBase()
{
    fCacheId = cid_rest_hall;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestHall::~CacheRestHall()
{
    fInstance = 0;
}

CacheRestHall *CacheRestHall::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestHall();
        fCacheOne[cid_rest_hall] = fInstance;
    }
    return fInstance;
}

void CacheRestHall::load()
{
    QString query = "select f_id, f_name, f_defaultMenu from r_hall";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_RestHall *c = new CI_RestHall();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fDefaultMenu = q->value(2).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    fDb.close();
    delete q;
}

void CacheRestHall::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_RestHall *ci = get(id);
    if (q->next()) {
        if (!ci) {
            ci = new CI_RestHall();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fDefaultMenu = q->value(2).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
}
