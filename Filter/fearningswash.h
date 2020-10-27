#ifndef FEARNINGSWASH_H
#define FEARNINGSWASH_H

#include "wfilterbase.h"

namespace Ui {
class FEarningsWash;
}

class FEarningsWash : public WFilterBase
{
    Q_OBJECT

public:
    explicit FEarningsWash(QWidget *parent = nullptr);
    ~FEarningsWash();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    void setHall(int h);

private:
    Ui::FEarningsWash *ui;
    QString fTitle;
};

#endif // FEARNINGSWASH_H
