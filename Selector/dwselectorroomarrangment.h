#ifndef DWSELECTORROOMARRANGMENT_H
#define DWSELECTORROOMARRANGMENT_H

#include "dwselector.h"
#include "cacheroomarrangment.h"

class DWSelectorRoomArrangment : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRoomArrangment(QWidget *parent);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void roomArrangment(CI_RoomArrangment *c);
};

#endif // DWSELECTORROOMARRANGMENT_H
