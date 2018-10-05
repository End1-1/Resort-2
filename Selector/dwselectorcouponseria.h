#ifndef DWSELECTORCOUPONSERIA_H
#define DWSELECTORCOUPONSERIA_H

#include "dwtemplateselector.h"
#include "cachecouponseria.h"

class DWSelectorCouponSeria : public DWTemplateSelector<CI_CouponSeria, CacheCouponSeria>
{
    Q_OBJECT
public:
    DWSelectorCouponSeria(QWidget *parent = 0);
};

#endif // DWSELECTORCOUPONSERIA_H
