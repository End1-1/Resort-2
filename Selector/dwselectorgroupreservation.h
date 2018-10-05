#ifndef DWSELECTORGROUPRESERVATION_H
#define DWSELECTORGROUPRESERVATION_H

#include "dwtemplateselector.h"
#include "cachegroupreservations.h"

class DWSelectorGroupReservation : public DWTemplateSelector<CI_GroupReservation, CacheGroupReservations>
{
public:
    DWSelectorGroupReservation(QWidget *parent = 0);
};

#endif // DWSELECTORGROUPRESERVATION_H
