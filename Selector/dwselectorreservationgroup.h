#ifndef DWSELECTORRESERVATIONGROUP_H
#define DWSELECTORRESERVATIONGROUP_H

#include "dwselector.h"
#include "cachereservegroup.h"

class DWSelectorReservationGroup : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorReservationGroup(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void reserveGroup(CI_ReserveGroup *c);
};

#endif // DWSELECTORRESERVATIONGROUP_H
