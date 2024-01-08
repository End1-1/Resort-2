#ifndef FREPORTBYPAYMENT_H
#define FREPORTBYPAYMENT_H

#include "wfilterbase.h"

namespace Ui {
class FReportByPayment;
}

class FReportByPayment : public WFilterBase
{
    Q_OBJECT

public:
    explicit FReportByPayment(QWidget *parent = 0);
    ~FReportByPayment();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
private:
    Ui::FReportByPayment *ui;

private slots:
    void branchEditDoubleClick(bool v);
    void hallEditDoubleClick(bool v);
};

#endif // FREPORTBYPAYMENT_H
