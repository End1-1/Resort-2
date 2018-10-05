#ifndef FNOSHOWCANCELATIONFEE_H
#define FNOSHOWCANCELATIONFEE_H

#include "wfilterbase.h"

namespace Ui {
class FNoShowCancelationFee;
}

class FNoShowCancelationFee : public WFilterBase
{
    Q_OBJECT

public:
    explicit FNoShowCancelationFee(QWidget *parent = 0);
    ~FNoShowCancelationFee();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FNoShowCancelationFee *ui;
private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
    void on_btnBack_clicked();
    void on_btnForward_clicked();
};

#endif // FNOSHOWCANCELATIONFEE_H
