#ifndef DWSELECTORPAYMENTMODE_H
#define DWSELECTORPAYMENTMODE_H

#include "dwselector.h"
#include "cachepaymentmode.h"

class DWSelectorPaymentMode : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorPaymentMode(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void paymentMode(CI_PaymentMode *c);
};

#endif // DWSELECTORPAYMENTMODE_H
