#ifndef DWSELECTORNATION_H
#define DWSELECTORNATION_H

#include "dwselector.h"
#include "cachenation.h"

class DWSelectorNation : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorNation(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void nation(CI_Nation *c);
};

#endif // DWSELECTORNATION_H
