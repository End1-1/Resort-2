#ifndef FTAXRETURN_H
#define FTAXRETURN_H

#include "wfilterbase.h"

namespace Ui {
class FTaxReturn;
}

class FTaxReturn : public WFilterBase
{
    Q_OBJECT

public:
    explicit FTaxReturn(QWidget *parent = nullptr);
    ~FTaxReturn();
    virtual void apply(WReportGrid *rg) override;
    virtual QWidget *firstElement() override;
    virtual QString reportTitle() override;

private:
    Ui::FTaxReturn *ui;

private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
};

#endif // FTAXRETURN_H
