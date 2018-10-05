#include "cachecurrencies.h"

CacheCurrencies *CacheCurrencies::fInstance;

void CacheCurrencies::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_Currency *c = new CI_Currency();
        fetchItem(q, c);
    }
    closeDb(q);
}

void CacheCurrencies::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_Currency *c = get(id);
    if (q->next()) {
        if (!c) {
            c = new CI_Currency();
        }
        fetchItem(q, c);
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    closeDb(q);
}

CacheCurrencies::CacheCurrencies() :
    CacheBase()
{
    fCacheId = cid_currencies;
    fQuery = "select f_id, f_short, f_name, f_rate from f_acc_currencies";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCurrencies::~CacheCurrencies()
{
    fInstance = 0;
}

void CacheCurrencies::fetchItem(QSqlQuery *q, CI_Currency *c)
{
    c->fCode = q->value(0).toString();
    c->fName = q->value(1).toString();
    c->fDescription = q->value(2).toString();
    c->fRate = q->value(3).toDouble();
}
