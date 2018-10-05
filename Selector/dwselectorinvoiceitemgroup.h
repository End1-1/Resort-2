#ifndef DWSELECTORINVOICEITEMGROUP_H
#define DWSELECTORINVOICEITEMGROUP_H

#include "dwselector.h"
#include "cacheinvoiceitemgroup.h"

class DWSelectorInvoiceItemGroup : public DWSelector
{

   Q_OBJECT
public:
    DWSelectorInvoiceItemGroup(QWidget *parent = 0);
    virtual void configure();
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void invoiceGroup(CI_InvoiceGroup *c);
};

#endif // DWSELECTORINVOICEITEMGROUP_H
