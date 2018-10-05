#include "cachecashdesk.h"

CacheCashDesk *CacheCashDesk::fInstance = 0;

CacheCashDesk *CacheCashDesk::instance()
{
    if (!fInstance) {
        fInstance = new CacheCashDesk();
        fCacheOne[cid_cash_desk] = fInstance;
    }
    return fInstance;
}

void CacheCashDesk::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_CashDesk *ci = get(id);
    if (q->next()) {
        if (!ci) {
            ci = new CI_CashDesk();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
    closeDb(q);
}

CacheCashDesk::CacheCashDesk() :
    CacheBase()
{
    fQuery = "select f_id, f_name from c_cash_names";
    fCacheId = cid_cash_desk;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCashDesk::~CacheCashDesk()
{
    fInstance = 0;
}
