#ifndef FDISCOUNTFULLSTAT_H
#define FDISCOUNTFULLSTAT_H

#include "wfilterbase.h"

namespace Ui {
class FDiscountFullStat;
}

class FDiscountFullStat : public WFilterBase
{
    Q_OBJECT

public:
    explicit FDiscountFullStat(QWidget *parent = 0);
    ~FDiscountFullStat();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
    static void openReport(const QDate &d1, const QDate &d2, const QString discountCard);
private:
    Ui::FDiscountFullStat *ui;
};

#endif // FDISCOUNTFULLSTAT_H
