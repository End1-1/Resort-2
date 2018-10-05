#ifndef DWSELECTORRESERVATIONCARDEX_H
#define DWSELECTORRESERVATIONCARDEX_H

#include "dwtemplateselector.h"
#include "cachereservation.h"

class DWSelectorReservationCardex : public DWTemplateSelector<CI_Reservation, CacheReservation>
{
    Q_OBJECT
public:
    DWSelectorReservationCardex(QWidget *parent = 0);
    virtual void configure();
};

#endif // DWSELECTORRESERVATIONCARDEX_H
