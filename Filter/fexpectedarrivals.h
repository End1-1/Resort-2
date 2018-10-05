#ifndef FEXPECTEDARRIVALS_H
#define FEXPECTEDARRIVALS_H

#include "wfilterbase.h"
#include "goexpextedarrivals.h"

namespace Ui {
class FExpectedArrivals;
}

class FExpectedArrivals : public WFilterBase
{
    Q_OBJECT

public:
    explicit FExpectedArrivals(QWidget *parent = 0);
    ~FExpectedArrivals();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual GOWidget *gridOptionWidget();
private:
    Ui::FExpectedArrivals *ui;
    void printArrival(WReportGrid *rg);
    void printDeparture(WReportGrid *rg);
};

#endif // FEXPECTEDARRIVALS_H
