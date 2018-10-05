#ifndef DWSELECTORDISH_H
#define DWSELECTORDISH_H

#include "dwtemplateselector.h"
#include "cachedish.h"

class DWSelectorDish : public DWTemplateSelector<CI_Dish, CacheDish>
{
    Q_OBJECT
public:
    DWSelectorDish(QWidget *parent = 0);
    virtual void configure();
};

#endif // DWSELECTORDISH_H
