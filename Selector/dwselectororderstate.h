#ifndef DWSELECTORORDERSTATE_H
#define DWSELECTORORDERSTATE_H

#include "dwtemplateselector.h"
#include "cacheorderstate.h"

class DWSelectorOrderState : public DWTemplateSelector<CI_OrderState, CacheOrderState>
{
    Q_OBJECT
public:
    DWSelectorOrderState(QWidget *parent = 0);
    virtual void configure();
};

#endif // DWSELECTORORDERSTATE_H
