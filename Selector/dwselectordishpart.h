#ifndef DWSELECTORDISHPART_H
#define DWSELECTORDISHPART_H

#include "dwselector.h"
#include "cacherestdishpart.h"

class DWSelectorDishPart : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorDishPart(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void dishPart(CI_RestDishPart *d);
};

#endif // DWSELECTORDISHPART_H
