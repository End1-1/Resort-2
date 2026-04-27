#ifndef CACHEINVOICEITEMGROUP_H
#define CACHEINVOICEITEMGROUP_H

#include "cachebase.h"

#define cid_invoice_group 57

struct CI_InvoiceGroup : CI_Base
{};
Q_DECLARE_METATYPE(CI_InvoiceGroup*)

class CacheInvoiceItemGroup : public CacheBase<CI_InvoiceGroup>
{
public:
    static CacheInvoiceItemGroup *instance();
    virtual void updateItem(const QString &id);
private:
    static CacheInvoiceItemGroup *fInstance;
    CacheInvoiceItemGroup();
    ~CacheInvoiceItemGroup();
};

#endif // CACHEINVOICEITEMGROUP_H
