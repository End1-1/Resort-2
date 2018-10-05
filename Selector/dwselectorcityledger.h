#ifndef DWSELECTORCITYLEDGER_H
#define DWSELECTORCITYLEDGER_H

#include "dwselector.h"
#include "cachecityledger.h"

class DWSelectorCityLedger : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorCityLedger(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void cityLedger(CI_CityLedger *c);
};

#endif // DWSELECTORCITYLEDGER_H
