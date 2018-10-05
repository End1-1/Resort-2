#ifndef DWSELECTORGUEST_H
#define DWSELECTORGUEST_H

#include "dwselector.h"
#include "cacheguest.h"

class DWSelectorGuest : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorGuest(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void guest(CI_Guest *c);
};

#endif // DWSELECTORGUEST_H
