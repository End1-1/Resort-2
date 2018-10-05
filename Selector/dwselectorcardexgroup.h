#ifndef DWSELECTORCARDEXGROUP_H
#define DWSELECTORCARDEXGROUP_H

#include "dwselector.h"
#include "cachecardexgroup.h"

class DWSelectorCardexGroup : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorCardexGroup(QWidget *parent = 0);
    virtual void configure();
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void cardexGroup(CI_CardexGroup *c);
};

#endif // DWSELECTORCARDEXGROUP_H
