#ifndef DWSELECTORROOMVIEW_H
#define DWSELECTORROOMVIEW_H

#include "dwselector.h"
#include "cacheroomview.h"

class DWSelectorRoomView : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRoomView(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void roomView(CI_RoomView *c);
};

#endif // DWSELECTORROOMVIEW_H
