#ifndef DWSELECTORCHECKOUTINVOICE_H
#define DWSELECTORCHECKOUTINVOICE_H

#include "dwtemplateselector.h"
#include "cachecheckoutinvoice.h"

class DWSelectorCheckoutInvoice : public DWTemplateSelector<CI_CheckoutInvoice, CacheCheckoutInvoice>
{
    Q_OBJECT
public:
    DWSelectorCheckoutInvoice(QWidget *parent = 0);
    void configure();
    CI_CheckoutInvoice *nextRow(int &row);
    CI_CheckoutInvoice *prevRow(int &row);
};

#endif // DWSELECTORCHECKOUTINVOICE_H
