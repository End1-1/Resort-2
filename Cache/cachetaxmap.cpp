#include "cachetaxmap.h"
#include <QHostInfo>

CacheTaxMap *CacheTaxMap::fInstance = 0;

CacheTaxMap *CacheTaxMap::instance()
{
    if (!fInstance) {
        fInstance = new CacheTaxMap();
        fCacheOne[cid_tax_map] = fInstance;
    }
    return fInstance;
}

void CacheTaxMap::load()
{
    QSqlQuery *q = prepareDb();
    qDeleteAll(fStaticCache[fCacheId]);
    fStaticCache[fCacheId].clear();
    while (q->next()) {
        CI_TaxMap *ci = new CI_TaxMap();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheTaxMap::updateItem(const QString &id)
{
    Q_UNUSED(id);
    load();
}

CacheTaxMap::CacheTaxMap() :
    CacheBase()
{
    fCacheId = cid_tax_map;
    fQuery = "select f_item, f_dept from f_invoice_item_tax where upper(f_comp)=upper(" + ap(def_station + QHostInfo::localHostName()) + ")";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheTaxMap::~CacheTaxMap()
{
    fInstance = 0;
}
