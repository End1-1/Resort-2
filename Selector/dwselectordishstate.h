#ifndef DWSELECTORDISHSTATE_H
#define DWSELECTORDISHSTATE_H

#include "dwtemplateselector.h"
#include "cachedishstate.h"

class DWSelectorDishState : public DWTemplateSelector<CI_DishState, CacheDishState>
{
    Q_OBJECT
public:
    DWSelectorDishState(QWidget *parent = 0);
};

#endif // DWSELECTORDISHSTATE_H
