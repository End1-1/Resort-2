#ifndef FDISCOUNTREPORT_H
#define FDISCOUNTREPORT_H

#include "wfilterbase.h"

namespace Ui {
class FDiscountReport;
}

class FDiscountReport : public WFilterBase
{
    Q_OBJECT

public:
    explicit FDiscountReport(QWidget *parent = 0);
    ~FDiscountReport();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);

private:
    Ui::FDiscountReport *ui;

private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
};

#endif // FDISCOUNTREPORT_H
