#ifndef FONLINEBREAKFAST_H
#define FONLINEBREAKFAST_H

#include "wfilterbase.h"
#include "wreportgrid.h"

namespace Ui {
class FOnlineBreakfast;
}

class FOnlineBreakfast : public WFilterBase
{
    Q_OBJECT

public:
    explicit FOnlineBreakfast(QWidget *parent = 0);
    ~FOnlineBreakfast();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FOnlineBreakfast *ui;
};

#endif // FONLINEBREAKFAST_H
