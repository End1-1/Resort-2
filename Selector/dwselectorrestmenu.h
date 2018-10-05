#ifndef DWSELECTORRESTMENU_H
#define DWSELECTORRESTMENU_H

#include "dwselector.h"
#include "cacherestmenu.h"

class DWSelectorRestMenu : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRestMenu(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void menu(CI_RestMenu*);
};

#endif // DWSELECTORRESTMENU_H
