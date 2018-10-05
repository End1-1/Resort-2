#ifndef DWSELECTORDEBTCOSTUMER_H
#define DWSELECTORDEBTCOSTUMER_H

#include "cachedebtcostumer.h"
#include "dwtemplateselector.h"

class DWSelectorDebtCostumer : public DWTemplateSelector<CI_DebtCostumer, CacheDebtCostumer>
{
    Q_OBJECT
public:
    DWSelectorDebtCostumer(QWidget *parent =0);
};

#endif // DWSELECTORDEBTCOSTUMER_H
