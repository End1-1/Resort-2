#ifndef DWSELECTORROOMCATEGORY_H
#define DWSELECTORROOMCATEGORY_H

#include "dwselector.h"
#include "cacheroomcategory.h"

class DWSelectorRoomCategory : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRoomCategory(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    roomCategory(CI_RoomCategory *c);
};

#endif // DWSELECTORROOMCATEGORY_H
