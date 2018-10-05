#ifndef DWSELECTORGUESTTITLE_H
#define DWSELECTORGUESTTITLE_H

#include "dwselector.h"
#include "cacheguesttitle.h"

class DWSelectorGuestTitle : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorGuestTitle(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void guestTitle(CI_GuestTitle *gt);
};

#endif // DWSELECTORGUESTTITLE_H
