#ifndef DWSELECTORRESERVESTATUS_H
#define DWSELECTORRESERVESTATUS_H

#include "dwselector.h"
#include "cachereservestatus.h"

class DWSelectorReserveStatus : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorReserveStatus(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void reserveStatus(CI_ReserveStatus *r);
};

#endif // DWSELECTORRESERVESTATUS_H
