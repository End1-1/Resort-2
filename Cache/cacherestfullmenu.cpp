#include "cacherestfullmenu.h"

CacheRestFullMenu *CacheRestFullMenu::fInstance = 0;

CacheRestFullMenu *CacheRestFullMenu::instance()
{
    if (!fInstance) {
        fInstance = new CacheRestFullMenu();
        fCacheOne[ci_rest_full_menu] = fInstance;
    }
    return fInstance;
}

void CacheRestFullMenu::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_RestFullMenu *ci = new CI_RestFullMenu();
        ci->fCode = q->value(0).toString();
        ci->fMenuCode = q->value(1).toInt();
        ci->fName = q->value(2).toString();
        ci->fDishCode = q->value(3).toInt();
        ci->fDishName["en"] = q->value(4).toString();
        ci->fDishName["am"] = q->value(5).toString();
        ci->fDishName["ru"] = q->value(6).toString();
        ci->fPrice = q->value(7).toDouble();
        ci->fStore = q->value(8).toInt();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

CacheRestFullMenu::CacheRestFullMenu()
{
    fQuery = "select m.f_id, m.f_menu, mn.f_en, m.f_dish, d.f_en, d.f_am, d.f_ru, m.f_price, m.f_store "
            "from r_menu m "
            "left join r_dish d on d.f_id=m.f_dish "
            "left join r_menu_names mn on mn.f_id=m.f_menu "
            "where m.f_state=1 "
            "order by 1, 4 ";
    fCacheId = ci_rest_full_menu;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheRestFullMenu::~CacheRestFullMenu()
{
    fInstance = 0;
}
