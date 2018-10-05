#ifndef FCOUPONSALES_H
#define FCOUPONSALES_H

#include "wfilterbase.h"

namespace Ui {
class FCouponSales;
}

class FCouponSales : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCouponSales(QWidget *parent = 0);
    ~FCouponSales();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
    virtual void selector(int selectorNumber, const QVariant &value);
private:
    Ui::FCouponSales *ui;
private slots:
    void newSale();
};

#endif // FCOUPONSALES_H
