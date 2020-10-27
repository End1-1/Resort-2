#ifndef DWSELECTORRESTFULLMENU_H
#define DWSELECTORRESTFULLMENU_H

#include "dwtemplateselector.h"
#include "cacherestfullmenu.h"

class DWSelectorRestFullMenu : public DWTemplateSelector<CI_RestFullMenu, CacheRestFullMenu>
{
    Q_OBJECT
public:
    DWSelectorRestFullMenu(QWidget *parent = 0);
    int fMenu;
    virtual void configure();
    inline virtual bool otherFilter(CI_Base *b) {CI_RestFullMenu *r = static_cast<CI_RestFullMenu*>(b); return fMenu == 0 ? true : fMenu == r->fMenuCode;}
};

#endif // DWSELECTORRESTFULLMENU_H
