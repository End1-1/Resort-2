#ifndef CACHERESTFULLMENU_H
#define CACHERESTFULLMENU_H

#include "cachebase.h"

#define ci_rest_full_menu 113

struct CI_RestFullMenu : public CI_Base
{
    int fMenuCode;
    int fDishCode;
    int fStore;
    QMap<QString, QString> fDishName;
    double fPrice;
};
Q_DECLARE_METATYPE(CI_RestFullMenu*)

class CacheRestFullMenu : public CacheBase<CI_RestFullMenu>
{
public:
    static CacheRestFullMenu *instance();
    virtual void load();
private:
    static CacheRestFullMenu *fInstance;
    CacheRestFullMenu();
    ~CacheRestFullMenu();
};

#endif // CACHERESTFULLMENU_H
