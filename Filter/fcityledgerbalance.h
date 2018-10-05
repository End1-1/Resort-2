#ifndef FCITYLEDGERBALANCE_H
#define FCITYLEDGERBALANCE_H

#include "wfilterbase.h"

namespace Ui {
class FCityLedgerBalance;
}

class FCityLedgerBalance : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCityLedgerBalance(QWidget *parent = 0);
    ~FCityLedgerBalance();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    static void open();
private:
    Ui::FCityLedgerBalance *ui;
};

#endif // FCITYLEDGERBALANCE_H
