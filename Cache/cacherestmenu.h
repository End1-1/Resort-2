#ifndef CACHERESTMENU_H
#define CACHERESTMENU_H

#include "cachebase.h"

#define cid_rest_menu 26

typedef struct : CI_Base {
}CI_RestMenu;
Q_DECLARE_METATYPE(CI_RestMenu*)

class CacheRestMenu : public CacheBase<CI_RestMenu>
{
public:
    static CacheRestMenu *instance();
    virtual void load();
private:
    static CacheRestMenu *fInstance;
    CacheRestMenu();
    ~CacheRestMenu();
};

#endif // CACHERESTMENU_H
