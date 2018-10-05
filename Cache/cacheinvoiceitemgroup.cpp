#include "cacheinvoiceitemgroup.h"

CacheInvoiceItemGroup *CacheInvoiceItemGroup::fInstance = 0;

CacheInvoiceItemGroup *CacheInvoiceItemGroup::instance()
{
    if (!fInstance) {
        fInstance = new CacheInvoiceItemGroup();
        fCacheOne[cid_invoice_group] = fInstance;
    }
    return fInstance;
}

void CacheInvoiceItemGroup::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_InvoiceGroup *c = 0;
    if (q->next()) {
        c = get(q->value(0).toString());
        if (!c) {
            c = new CI_InvoiceGroup();
        }
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    closeDb(q);
}

CacheInvoiceItemGroup::CacheInvoiceItemGroup() :
    CacheBase()
{
    fQuery = "select f_id, f_" + def_lang + " from f_invoice_item_group";
    fCacheId = cid_invoice_group;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheInvoiceItemGroup::~CacheInvoiceItemGroup()
{
    fInstance = 0;
}
