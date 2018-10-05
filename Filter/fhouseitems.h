#ifndef FHOUSEITEMS_H
#define FHOUSEITEMS_H

#include "wfilterbase.h"

namespace Ui {
class FHouseItems;
}

class FHouseItems : public WFilterBase
{
    Q_OBJECT

public:
    explicit FHouseItems(QWidget *parent = 0);
    ~FHouseItems();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FHouseItems *ui;
};

#endif // FHOUSEITEMS_H
