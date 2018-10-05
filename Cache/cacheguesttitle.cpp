#include "cacheguesttitle.h"

CacheGuestTitle *CacheGuestTitle::fInstance = 0;

CacheGuestTitle::CacheGuestTitle() :
    CacheBase()
{
    fCacheId = cid_guest_title;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheGuestTitle::CacheGuestTitle(CacheGuestTitle &g)
{
    Q_UNUSED(g)
}

CacheGuestTitle::~CacheGuestTitle()
{
    fInstance = 0;
}

CacheGuestTitle *CacheGuestTitle::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheGuestTitle();
        fCacheOne.insert(cid_guest_title, fInstance);
    }
    return fInstance;
}

void CacheGuestTitle::load()
{
    QString query = "select f_" + def_lang + ", f_" + def_lang + " from f_guests_title";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_GuestTitle *g = new CI_GuestTitle();
        g->fCode = q->value(0).toString();
        g->fName = q->value(1).toString();
        fStaticCache[fCacheId][g->fCode] = g;
    }
    fDb.close();
    delete q;

}

void CacheGuestTitle::updateItem(const QString &id)
{
    QString query = "select f_" + def_lang + ", f_" + def_lang + " from f_guests_title where f_id=" + id;
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    CI_GuestTitle *g = get(id);
    if (q->next()) {
        if (!g) {
            g = new CI_GuestTitle();
        }
        g->fCode = q->value(0).toString();
        g->fName = q->value(1).toString();
        fStaticCache[fCacheId][g->fCode] = g;
    } else {
        if (g) {
            g->fValid = false;
        }
    }
    fDb.close();
    delete q;
}
