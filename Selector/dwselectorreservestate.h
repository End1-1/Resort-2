#ifndef DWSELECTORRESERVESTATE_H
#define DWSELECTORRESERVESTATE_H

#include "dwselector.h"
#include "cachereservestate.h"

class DWSelectorReserveState : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorReserveState(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
private:
signals:
    void reserveState(CI_ReserveState *c);
};

#endif // DWSELECTORRESERVESTATE_H
