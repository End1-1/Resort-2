#ifndef DWSELECTORDISCOUNTTYPE_H
#define DWSELECTORDISCOUNTTYPE_H

#include "dwtemplateselector.h"
#include "cachediscounttype.h"

class DWSelectorDiscountType : public DWTemplateSelector<CI_DiscountType, CacheDiscountType>
{
    Q_OBJECT
public:
    DWSelectorDiscountType(QWidget *parent = 0);
};

#endif // DWSELECTORDISCOUNTTYPE_H
