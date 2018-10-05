#ifndef DWSELECTORCAR_H
#define DWSELECTORCAR_H

#include "dwtemplateselector.h"
#include "cachecar.h"

class DWSelectorCar : public DWTemplateSelector<CI_Car, CacheCar>
{
    Q_OBJECT
public:
    DWSelectorCar(QWidget *parent = 0);
};

#endif // DWSELECTORCAR_H
