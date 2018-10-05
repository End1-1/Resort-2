#ifndef DWSELECTORCREDITCARD_H
#define DWSELECTORCREDITCARD_H

#include "dwselector.h"
#include "cachecreditcard.h"

class DWSelectorCreditCard : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorCreditCard(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void creditCard(CI_CreditCard *c);
};

#endif // DWSELECTORCREDITCARD_H
