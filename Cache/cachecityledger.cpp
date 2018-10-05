#include "cachecityledger.h"

CacheCityLedger *CacheCityLedger::fInstance;

CacheCityLedger::CacheCityLedger() :
    CacheBase()
{
    fCacheId = cid_city_ledger;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCityLedger::~CacheCityLedger()
{
    fInstance = 0;
}

CacheCityLedger *CacheCityLedger::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheCityLedger();
        fCacheOne[cid_city_ledger] = fInstance;
    }
    return fInstance;
}

void CacheCityLedger::load()
{
    QString query = "select f_id, f_name, f_extra1, f_extra2 from f_city_ledger";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_CityLedger *c = new CI_CityLedger();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
//        c->fVatMode = q->value(2).toString();
//        c->fVatName = q->value(3).toString();
//        c->fCommission = q->value(4).toString();
//        c->fDateTo = q->value(5).toDate().toString(def_date_format);
        c->fExtra1 = q->value(2).toString();
        c->fExtra2 = q->value(3).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    fDb.close();
    delete q;
}

void CacheCityLedger::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb("select f_id, f_name from f_city_ledger where f_id=" + id);
    CI_CityLedger *cl = get(id);;
    if (q->next()) {
        if (!cl) {
            cl = new CI_CityLedger();
        }
        cl->fCode = q->value(0).toString();
        cl->fName = q->value(1).toString();
        cl->fExtra1 = q->value(2).toString();
        cl->fExtra2 = q->value(3).toString();
        fStaticCache[fCacheId][cl->fCode] = cl;
    } else {
        if (cl) {
            cl->fValid = false;
        }
    }
    closeDb(q);
}
