#ifndef FDEBTOFCOSTUMERS_H
#define FDEBTOFCOSTUMERS_H

#include "wfilterbase.h"

namespace Ui {
class FDebtOfCostumers;
}

class FDebtOfCostumers : public WFilterBase
{
    Q_OBJECT

public:
    explicit FDebtOfCostumers(QWidget *parent = 0);
    ~FDebtOfCostumers();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
    virtual void selector(int selectorNumber, const QVariant &value);
private:
    Ui::FDebtOfCostumers *ui;
private slots:
    void payment();
    void removePay();
    void dblClick(const QList<QVariant> &r);
};

#endif // FDEBTOFCOSTUMERS_H
