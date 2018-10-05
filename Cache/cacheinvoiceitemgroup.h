#ifndef CACHEINVOICEITEMGROUP_H
#define CACHEINVOICEITEMGROUP_H

#include "cachebase.h"

#define cid_invoice_group 57

typedef struct : CI_Base {

} CI_InvoiceGroup;
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
