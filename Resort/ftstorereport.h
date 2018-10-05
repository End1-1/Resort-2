#ifndef FTSTOREREPORT_H
#define FTSTOREREPORT_H

#include "wfilterbase.h"

namespace Ui {
class FTStoreReport;
}

class FTStoreReport : public WFilterBase
{
    Q_OBJECT

public:
    explicit FTStoreReport(QWidget *parent = 0);
    ~FTStoreReport();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
    virtual void selector(int selectorNumber, const QVariant &value);
    void setParams(const QString &store, const QString &goods);
private:
    Ui::FTStoreReport *ui;
};

#endif // FTSTOREREPORT_H
