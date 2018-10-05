#ifndef DWSELECTORDISHMOD_H
#define DWSELECTORDISHMOD_H

#include "dwselector.h"
#include "cacherestdishmod.h"

class DWSelectorDishMod : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorDishMod(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void dishMod(CI_RestDishMod *m);
};

#endif // DWSELECTORDISHMOD_H
