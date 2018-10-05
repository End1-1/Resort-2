#ifndef DWSELECTORDISHTYPE_H
#define DWSELECTORDISHTYPE_H

#include "dwselector.h"
#include "cacherestdishtype.h"

class DWSelectorDishType : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorDishType(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void dishType(CI_RestDishType *d);
};

#endif // DWSELECTORDISHTYPE_H
