#ifndef FNATBYPERIOD_H
#define FNATBYPERIOD_H

#include "wfilterbase.h"

namespace Ui {
class FNatByPeriod;
}

class FNatByPeriod : public WFilterBase
{
    Q_OBJECT

public:
    explicit FNatByPeriod(QWidget *parent = 0);
    ~FNatByPeriod();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FNatByPeriod *ui;
};

#endif // FNATBYPERIOD_H
