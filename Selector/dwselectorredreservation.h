#ifndef DWSELECTORREDRESERVATION_H
#define DWSELECTORREDRESERVATION_H

#include "dwselector.h"
#include "cacheredreservation.h"

class DWSelectorRedReservation : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorRedReservation(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void redReservation(CI_RedReservation *c);
};

#endif // DWSELECTORREDRESERVATION_H
