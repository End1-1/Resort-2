#ifndef FBALANCEONCARD_H
#define FBALANCEONCARD_H

#include "wfilterbase.h"

namespace Ui {
class FBalanceOnCard;
}

class FBalanceOnCard : public WFilterBase
{
    Q_OBJECT

public:
    explicit FBalanceOnCard(QWidget *parent = 0);
    ~FBalanceOnCard();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);

private:
    Ui::FBalanceOnCard *ui;
};

#endif // FBALANCEONCARD_H
