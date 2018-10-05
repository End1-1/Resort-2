#include "cacherestdebtholder.h"

CacheRestDebtHolder *CacheRestDebtHolder::fInstance = 0;

CacheRestDebtHolder *CacheRestDebtHolder::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestDebtHolder();
        fCacheOne[cid_rest_debt_holder] = fInstance;
    }
    return fInstance;
}

void CacheRestDebtHolder::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestDebtHolder *ci = new CI_RestDebtHolder();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fInfo = q->value(2).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheRestDebtHolder::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + ap(id));
    if (q->next()) {
        CI_RestDebtHolder *ci = get(id);
        if (!ci) {
            ci = new CI_RestDebtHolder();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fInfo = q->value(2).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

CacheRestDebtHolder::CacheRestDebtHolder() :
    CacheBase()
{
    fCacheId = cid_rest_debt_holder;
    fQuery = "select f_id, f_name, f_info from o_debt_holder";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestDebtHolder::~CacheRestDebtHolder()
{
    fInstance = 0;
}
