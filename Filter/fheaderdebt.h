#ifndef FHEADERDEBT_H
#define FHEADERDEBT_H

#include "wfilterbase.h"

namespace Ui {
class FHeaderDebt;
}

class FHeaderDebt : public WFilterBase
{
    Q_OBJECT

public:
    explicit FHeaderDebt(QWidget *parent = nullptr);
    ~FHeaderDebt();
    virtual void apply(WReportGrid *rg) override;
    virtual QWidget *firstElement() override;
    virtual QString reportTitle() override;

private:
    Ui::FHeaderDebt *ui;

private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
};

#endif // FHEADERDEBT_H
