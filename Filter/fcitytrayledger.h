#ifndef FCITYTRAYLEDGER_H
#define FCITYTRAYLEDGER_H

#include "wfilterbase.h"

namespace Ui {
class FCityTrayLedger;
}

class FCityTrayLedger : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCityTrayLedger(QWidget *parent = 0);
    ~FCityTrayLedger();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FCityTrayLedger *ui;
};

#endif // FCITYTRAYLEDGER_H
