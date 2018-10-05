#ifndef FCOUPONSTATISTICS_H
#define FCOUPONSTATISTICS_H

#include "wfilterbase.h"

namespace Ui {
class FCouponStatistics;
}

class FCouponStatistics : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCouponStatistics(QWidget *parent = 0);
    ~FCouponStatistics();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FCouponStatistics *ui;
};

#endif // FCOUPONSTATISTICS_H
