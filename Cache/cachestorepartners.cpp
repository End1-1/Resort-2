#include "cachestorepartners.h"

CacheStorePartners *CacheStorePartners::fInstance = 0;

CacheStorePartners *CacheStorePartners::instance()
{
    if (!fInstance) {
        fInstance = new CacheStorePartners();
        fCacheOne[cid_store_partners] = fInstance;
    }
    return fInstance;
}

void CacheStorePartners::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_StorePartners *ci = new CI_StorePartners;
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fTaxCode = q->value(2).toString();
        ci->fPhone = q->value(3).toString();
        ci->fComments = q->value(4).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheStorePartners::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_StorePartners *ci = get(id);
    if (q->next()) {
        if (!ci) {
            ci = new CI_StorePartners();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fTaxCode = q->value(2).toString();
        ci->fPhone = q->value(3).toString();
        ci->fComments = q->value(4).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
}

CacheStorePartners::CacheStorePartners() :
    CacheBase()
{
    fCacheId = cid_store_partners;
    fQuery = "select f_id, f_name, f_taxcode, f_phone, f_comment from r_partners";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheStorePartners::~CacheStorePartners()
{
    fInstance = 0;
}
