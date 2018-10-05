#include "cacheguest.h"

CacheGuest *CacheGuest::fInstance;

CacheGuest *CacheGuest::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheGuest();
        fCacheOne[cid_guest] = fInstance;
    }
    return fInstance;
}

void CacheGuest::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_Guest *c = new CI_Guest();
        c->fCode = q->value(0).toString();
        c->fTitle = q->value(1).toString();
        c->fName = q->value(2).toString();
        c->fNatShort = q->value(3).toString();
        c->fNatFull = q->value(4).toString();
        c->fPassport = q->value(5).toString();
        c->fFirstName = q->value(6).toString();
        c->fLastName = q->value(7).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

void CacheGuest::updateItem(const QString &id)
{
    if (fStaticCache[fCacheId].count() == 0) {
        return;
    }
    QSqlQuery *q = prepareDb(fQuery  + " where g.f_id=" + id);
    CI_Guest *c = get(id);
    if (q->next()) {
        if (!c) {
            c = new CI_Guest();
        }
        c->fCode = q->value(0).toString();
        c->fTitle = q->value(1).toString();
        c->fName = q->value(2).toString();
        c->fNatShort = q->value(3).toString();
        c->fNatFull = q->value(4).toString();
        c->fPassport = q->value(5).toString();
        c->fFirstName = q->value(6).toString();
        c->fLastName = q->value(7).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    closeDb(q);
}

CacheGuest::CacheGuest() :
    CacheBase()
{
    fQuery = "select g.f_id, g.f_title, concat(g.f_firstName, ' ', g.f_lastName), "
            "g.f_nation, n.f_name, g.f_passport, g.f_firstName, g.f_lastName "
            "from f_guests g "
            "left join f_nationality n on n.f_short=g.f_nation ";
    fCacheId = cid_guest;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheGuest::~CacheGuest()
{
    fInstance = 0;
}
