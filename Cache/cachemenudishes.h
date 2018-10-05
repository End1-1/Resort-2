#ifndef CACHEMENUDISHES_H
#define CACHEMENUDISHES_H

#include "cachebase.h"

#define cid_menu_dishes 49

typedef struct : CI_Base {
    QString fMenuId;
    QString fTypeName;
    QString fMenu;
    QString fPrice;
} CI_MenuDishes;
Q_DECLARE_METATYPE(CI_MenuDishes*)

class CacheMenuDishes : public CacheBase<CI_MenuDishes>
{
public:
    static CacheMenuDishes *instance();
    virtual void load();
private:
    static CacheMenuDishes *fInstance;
    CacheMenuDishes();
    ~CacheMenuDishes();
};

#endif // CACHEMENUDISHES_H
