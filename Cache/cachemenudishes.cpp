#include "cachemenudishes.h"

CacheMenuDishes *CacheMenuDishes::fInstance = 0;

CacheMenuDishes *CacheMenuDishes::instance()
{
    if (!fInstance) {
        fInstance = new CacheMenuDishes();
        fCacheOne[cid_menu_dishes] = fInstance;
    }
    return fInstance;
}

void CacheMenuDishes::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_MenuDishes *c = new CI_MenuDishes();
        c->fCode = q->value(0).toString();
        c->fMenuId = q->value(1).toString();
        c->fMenu = q->value(2).toString();
        c->fTypeName = q->value(3).toString();
        c->fName = q->value(5).toString();
        c->fPrice = q->value(6).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheMenuDishes::CacheMenuDishes() :
    CacheBase()
{
    fCacheId = cid_menu_dishes;
    fQuery = "select m.f_id, m.f_menu, mn.f_" + def_lang + ", t.f_en, "
            "m.f_dish, d.f_" + def_lang + ", m.f_price "
            "from r_menu m "
            "inner join r_dish d on d.f_id=m.f_dish "
            "inner join r_dish_type t on t.f_id=d.f_type "
            "inner join r_menu_names mn on mn.f_id=m.f_menu "
            "where m.f_state=1 ";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheMenuDishes::~CacheMenuDishes()
{
    fInstance = 0;
}
