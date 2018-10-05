#include "cacherestmenu.h"

CacheRestMenu *CacheRestMenu::fInstance = 0;

CacheRestMenu::CacheRestMenu() :
    CacheBase()
{
    fCacheId = cid_rest_menu;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestMenu::~CacheRestMenu()
{
    fInstance = 0;
}

CacheRestMenu *CacheRestMenu::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestMenu();
        fCacheOne[cid_rest_menu] = fInstance;
    }
    return fInstance;
}

void CacheRestMenu::load()
{
    QString query = "select f_id, f_" + def_lang + " from r_menu_names";
    clear();
    if (!fDb.open()) {
        return;
    }
    QSqlQuery *q = fDb.select(query);
    if (!q) {
        return;
    }
    while (q->next()) {
        CI_RestMenu *c = new CI_RestMenu();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    fDb.close();
    delete q;
}
