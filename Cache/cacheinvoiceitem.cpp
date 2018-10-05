#include "cacheinvoiceitem.h"

CacheInvoiceItem *CacheInvoiceItem::fInstance = 0;

CacheInvoiceItem::CacheInvoiceItem() :
    CacheBase()
{
    fCacheId = cid_invoice_item;
    fQuery = "select f_id, f_group, f_" + def_lang + ", f_en, f_am, f_ru, f_adgt, "
            "f_vatDept, f_noVatDept, f_taxName, f_vaucher, f_auto, f_price, f_vatReception  from f_invoice_item";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheInvoiceItem::~CacheInvoiceItem()
{
    fInstance = 0;
}


CacheInvoiceItem *CacheInvoiceItem::instance()
{
    if (!fInstance) {
        fInstance = new CacheInvoiceItem();
        fCacheOne[cid_invoice_item] = fInstance;
    }
    return fInstance;
}

void CacheInvoiceItem::load()
{
    clear();
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_InvoiceItem  *ci = new CI_InvoiceItem();
        ci->fCode = q->value(0).toString();
        ci->fGroup = q->value(1).toInt();
        ci->fName = q->value(2).toString();
        ci->fLang["en"] = q->value(3).toString();
        ci->fLang["am"] = q->value(4).toString();
        ci->fLang["ru"] = q->value(5).toString();
        ci->fAdgt= q->value(6).toString();
        ci->fVatDept = q->value(7).toString();
        ci->fNoVatDept = q->value(8).toString();
        ci->fTaxName = q->value(9).toString();
        ci->fVaucher = q->value(10).toString();
        ci->fNoManual = q->value(11).toBool();
        ci->fPrice = q->value(12).toDouble();
        ci->fVatReception = q->value(13).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheInvoiceItem::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_InvoiceItem  *ci = get(id);
    if (q->next()) {
        if (!ci) {
            ci = new CI_InvoiceItem();
        }
        ci->fCode = q->value(0).toString();
        ci->fGroup = q->value(1).toInt();
        ci->fName = q->value(2).toString();
        ci->fLang["en"] = q->value(3).toString();
        ci->fLang["am"] = q->value(4).toString();
        ci->fLang["ru"] = q->value(5).toString();
        ci->fAdgt= q->value(6).toString();
        ci->fVatDept = q->value(7).toString();
        ci->fNoVatDept = q->value(8).toString();
        ci->fTaxName = q->value(9).toString();
        ci->fVaucher = q->value(10).toString();
        ci->fNoManual = q->value(11).toBool();
        ci->fPrice = q->value(12).toDouble();
        ci->fVatReception = q->value(13).toString();
        fStaticCache[fCacheId][ci->fCode] = ci;
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
    closeDb(q);
}
