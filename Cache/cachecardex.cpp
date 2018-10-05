#include "cachecardex.h"

CacheCardex *CacheCardex::fInstance = 0;

CacheCardex::CacheCardex() :
    CacheBase()
{
    fCacheId = cid_cardex;
    fQuery = "select c.f_cardex, c.f_name, c.f_vatMode, v.f_" + def_lang +","
              "c.f_commission, f_dateTo, f_bookingRequired, c.f_group "
              "from f_cardex c "
              "inner join f_vat_mode v on v.f_id=c.f_vatMode "
              "where f_dateTo > '" + fCacheDate.toString(def_mysql_date_format) + "'";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCardex::~CacheCardex()
{
    fInstance = 0;
}

CacheCardex *CacheCardex::instance()
{
    if (!fInstance) {
        fInstance = new CacheCardex();
        fCacheOne[cid_cardex] = fInstance;
    }
    return fInstance;
}

void CacheCardex::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_Cardex *c = new CI_Cardex();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fVatMode = q->value(2).toString();
        c->fVatName = q->value(3).toString();
        c->fCommission = q->value(4).toString();
        c->fDateTo = q->value(5).toDate().toString(def_date_format);
        c->fBookingRequired = q->value(6).toBool();
        c->fGroup = q->value(7).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

void CacheCardex::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " and c.f_id=" + id);
    CI_Cardex *c = get(id);
    if (q->next()) {
        if (!c) {
            c = new CI_Cardex();
        }
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fVatMode = q->value(2).toString();
        c->fVatName = q->value(3).toString();
        c->fCommission = q->value(4).toString();
        c->fDateTo = q->value(5).toDate().toString(def_date_format);
        c->fBookingRequired = q->value(6).toBool();
        c->fGroup = q->value(7).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    closeDb(q);
}
