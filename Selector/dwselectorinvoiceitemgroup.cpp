#include "dwselectorinvoiceitemgroup.h"

DWSelectorInvoiceItemGroup::DWSelectorInvoiceItemGroup(QWidget *parent) :
    DWSelector(cid_invoice_group, parent)
{
    setWindowTitle(tr("Invoice item group"));
}

void DWSelectorInvoiceItemGroup::configure()
{
    simpleTable<CI_InvoiceGroup, CacheInvoiceItemGroup>();
}

void DWSelectorInvoiceItemGroup::select(const QVariant &value)
{
    emit invoiceGroup(value.value<CI_InvoiceGroup*>());
}

void DWSelectorInvoiceItemGroup::selectorFocusOut()
{
    CI_InvoiceGroup *c = 0;
    if (fLineSelector) {
        c = CacheInvoiceItemGroup::instance()->get(fLineSelector->text());
    }
    emit invoiceGroup(c);
}
