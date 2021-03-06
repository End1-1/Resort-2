#ifndef FTSTOREREPORT_H
#define FTSTOREREPORT_H

#include "wfilterbase.h"
#include "dwselectordish.h"

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
    DWSelectorDish *fDockDish;
    QString fStore;

private slots:
    void doubleClickOnRow(int row, int column, const QList<QVariant> &values);
};

#endif // FTSTOREREPORT_H
