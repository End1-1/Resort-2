#ifndef FDEBTHOLDERS_H
#define FDEBTHOLDERS_H

#include "wfilterbase.h"

namespace Ui {
class FDebtHolders;
}

class FDebtHolders : public WFilterBase
{
    Q_OBJECT

public:
    explicit FDebtHolders(QWidget *parent = 0);
    ~FDebtHolders();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FDebtHolders *ui;
};

#endif // FDEBTHOLDERS_H
