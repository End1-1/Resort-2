#ifndef FCANCELEDRESERVATIONS_H
#define FCANCELEDRESERVATIONS_H

#include "wfilterbase.h"

namespace Ui {
class FCanceledReservations;
}

class FCanceledReservations : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCanceledReservations(QWidget *parent = 0);
    ~FCanceledReservations();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void openReport();
private:
    Ui::FCanceledReservations *ui;
};

#endif // FCANCELEDRESERVATIONS_H
