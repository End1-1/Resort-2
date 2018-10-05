#ifndef DWSELECTORROOMINVENTORY_H
#define DWSELECTORROOMINVENTORY_H

#include "dwtemplateselector.h"
#include "cacheroominventory.h"

class DWSelectorRoomInventory : public DWTemplateSelector<CI_RoomInventory, CacheRoomInventory>
{
public:
    DWSelectorRoomInventory(QWidget *parent = 0);
};

#endif // DWSELECTORROOMINVENTORY_H
