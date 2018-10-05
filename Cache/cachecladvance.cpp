#include "cachecladvance.h"

CacheCLAdvance *CacheCLAdvance::fInstance = 0;

CacheCLAdvance *CacheCLAdvance::instance()
{
    if (!fInstance) {
        fInstance = new CacheCLAdvance();
        fCacheOne[cid_cache_cl_advance] = fInstance;
    }
    return fInstance;
}

void CacheCLAdvance::load()
{
    QString query = fQuery;
    query.replace(":cond", "");
    QSqlQuery *q = prepareDb(query);
    while (q->next()) {
        CI_CLAdvance *ci = new CI_CLAdvance();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fAmount = q->value(2).toDouble();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheCLAdvance::updateItem(const QString &id)
{
    QString query = fQuery;
    query.replace(":cond", " and m.f_cityLedger=" + id);
    QSqlQuery *q = prepareDb(query);
    CI_CLAdvance *ci = get(id);
    if (q->next()) {
        if (!ci) {
            ci = new CI_CLAdvance();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fAmount = abs(q->value(2).toDouble());
        fStaticCache[fCacheId][ci->fCode] = ci;
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
    closeDb(q);
}

CacheCLAdvance::CacheCLAdvance() :
    CacheBase()
{
    fCacheId = cid_cache_cl_advance;
    fQuery = "select m.f_cityLedger, c.f_name, sum(m.f_amountAmd*f_sign) as amount from m_register m "
            "inner join f_city_ledger c on c.f_id=m.f_cityLedger "
            "where m.f_source='TC' and f_canceled=0 :cond "
            "group by 1, 2 "
            "having sum(m.f_amountAmd*f_sign) > 0 ";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCLAdvance::~CacheCLAdvance()
{
    fInstance = 0;
}
