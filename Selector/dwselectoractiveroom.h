#ifndef DWSELECTORACTIVEROOM_H
#define DWSELECTORACTIVEROOM_H

#include "dwselector.h"
#include "cacheactiveroom.h"

class DWSelectorActiveRoom : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorActiveRoom(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
protected:
    virtual void selectorFocusOut();
signals:
    void roomChanged(CI_ActiveRoom *c);
};

#endif // DWSELECTORACTIVEROOM_H
