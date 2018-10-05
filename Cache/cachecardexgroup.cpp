#include "cachecardexgroup.h"

CacheCardexGroup *CacheCardexGroup::fInstance = 0;

CacheCardexGroup *CacheCardexGroup::instance()
{
    if (!fInstance) {
        fInstance = new CacheCardexGroup();
        fCacheOne[cid_cardex_group] = fInstance;
    }
    return fInstance;
}

void CacheCardexGroup::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" +id);
    CI_CardexGroup *c = get(id);
    if (q->next()) {
        if (!c) {
            c = new CI_CardexGroup();
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

CacheCardexGroup::CacheCardexGroup() :
    CacheBase()
{
    fQuery = "select f_id, f_code from f_cardex_group";
    fCacheId = cid_cardex_group;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCardexGroup::~CacheCardexGroup()
{
    fInstance = 0;
}
