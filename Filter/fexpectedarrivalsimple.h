#ifndef FEXPECTEDARRIVALSIMPLE_H
#define FEXPECTEDARRIVALSIMPLE_H

#include "wfilterbase.h"

namespace Ui {
class FExpectedArrivalSimple;
}

class FExpectedArrivalSimple : public WFilterBase
{
    Q_OBJECT

public:
    explicit FExpectedArrivalSimple(QWidget *parent = 0);
    ~FExpectedArrivalSimple();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
private:
    Ui::FExpectedArrivalSimple *ui;
};

#endif // FEXPECTEDARRIVALSIMPLE_H
