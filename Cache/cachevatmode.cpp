#include "cachevatmode.h"

CacheVatMode *CacheVatMode::fInstance = 0;

CacheVatMode::CacheVatMode() :
    CacheBase()
{
    fCacheId = cid_vat_mode;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheVatMode::~CacheVatMode()
{
    fInstance = 0;
}

CacheVatMode *CacheVatMode::instance()
{
    if (!fInstance) {
        fInstance = new CacheVatMode();
        fCacheOne[cid_vat_mode] = fInstance;
    }
    return fInstance;
}

void CacheVatMode::load()
{
    QString query = "select f_id, f_" + def_lang + " from f_vat_mode";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_VATMode *v = new CI_VATMode();
        v->fCode = q->value(0).toString();
        v->fName = q->value(1).toString();
        fStaticCache[fCacheId][v->fCode] = v;
    }
    fDb.close();
    delete q;
}
