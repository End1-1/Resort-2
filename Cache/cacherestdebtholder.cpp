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
    fStaticCache[fCacheId].clear();
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestDebtHolder *ci = new CI_RestDebtHolder();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fGovNumber = q->value(2).toString();
        ci->fInfo = q->value(3).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheRestDebtHolder::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where d.f_id=" + ap(id));
    if (q->next()) {
        CI_RestDebtHolder *ci = get(id);
        if (!ci) {
            ci = new CI_RestDebtHolder();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fGovNumber = q->value(2).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

CacheRestDebtHolder::CacheRestDebtHolder() :
    CacheBase()
{
    fCacheId = cid_rest_debt_holder;
    fQuery = "select d.f_id, d.f_name, g.f_gov, f_info from o_debt_holder d "
             "left join (select f_holder, group_concat(f_govNumber) as f_gov from o_debt_holder_car group by 1) g on g.f_holder=d.f_id ";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestDebtHolder::~CacheRestDebtHolder()
{
    fInstance = 0;
}
