#ifndef FINHOUSEDETAILBALANCE_H
#define FINHOUSEDETAILBALANCE_H

#include "wfilterbase.h"

namespace Ui {
class FInhouseDetailBalance;
}

class FInhouseDetailBalance : public WFilterBase
{
    Q_OBJECT

public:
    explicit FInhouseDetailBalance(QWidget *parent = 0);
    ~FInhouseDetailBalance();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FInhouseDetailBalance *ui;
};

#endif // FINHOUSEDETAILBALANCE_H
