#ifndef DWSELECTORCARDEX_H
#define DWSELECTORCARDEX_H

#include "dwselector.h"
#include "cachecardex.h"

class DWSelectorCardex : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorCardex(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void cardex(CI_Cardex *c);
};

#endif // DWSELECTORCARDEX_H
