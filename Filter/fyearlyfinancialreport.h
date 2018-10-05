#ifndef FYEARLYFINANCIALREPORT_H
#define FYEARLYFINANCIALREPORT_H

#include "wfilterbase.h"

namespace Ui {
class FYearlyFinancialReport;
}

class FYearlyFinancialReport : public WFilterBase
{
    Q_OBJECT

public:
    explicit FYearlyFinancialReport(QWidget *parent = 0);
    ~FYearlyFinancialReport();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void openReport();
private:
    Ui::FYearlyFinancialReport *ui;
};

#endif // FYEARLYFINANCIALREPORT_H
