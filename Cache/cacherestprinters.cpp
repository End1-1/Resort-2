#include "cacherestprinters.h"

CacheRestPrinters *CacheRestPrinters::fInstance = 0;

CacheRestPrinters::CacheRestPrinters() :
    CacheBase()
{
    fCacheId = cid_rest_printers;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestPrinters::~CacheRestPrinters()
{
    fInstance = 0;
}

CacheRestPrinters *CacheRestPrinters::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestPrinters();
        fCacheOne[cid_rest_printers] = fInstance;
    }
    return fInstance;
}

void CacheRestPrinters::load()
{
    QString query = "select f_id, f_name from r_printers";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_RestPrinter *c = new CI_RestPrinter();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    fDb.close();
    delete q;
}

void CacheRestPrinters::updateItem(const QString &id)
{
    QString query = "select f_id, f_name from r_printers where f_id=" + id;
    QSqlQuery *q = prepareDb(query);
    CI_RestPrinter *cp = get(id);
    if (!cp) {
        cp = new CI_RestPrinter();
    }
    if (q->next()) {
        cp->fCode = q->value(0).toString();
        cp->fName = q->value(1).toString();
        fStaticCache[fCacheId][cp->fCode] = cp;
    } else {
        cp->fValid = false;
    }
    closeDb(q);
}
