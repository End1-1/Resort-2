#ifndef FDISCOUNTTOTAL_H
#define FDISCOUNTTOTAL_H

#include "wfilterbase.h"

namespace Ui {
class FDiscountTotal;
}

class FDiscountTotal : public WFilterBase
{
    Q_OBJECT

public:
    explicit FDiscountTotal(QWidget *parent = 0);
    ~FDiscountTotal();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);

private:
    Ui::FDiscountTotal *ui;
};

#endif // FDISCOUNTTOTAL_H
