#ifndef DWSELECTORROOMSTATE_H
#define DWSELECTORROOMSTATE_H

#include "dwselector.h"
#include "cacheroomstate.h"

class DWSelectorRoomState : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRoomState(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void roomState(CI_RoomState *c);
};

#endif // DWSELECTORROOMSTATE_H
