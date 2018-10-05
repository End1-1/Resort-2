#ifndef CACHEINVOICEITEM_H
#define CACHEINVOICEITEM_H

#include "cachebase.h"

#define cid_invoice_item 20

typedef struct : CI_Base {
    int fGroup;
    QString fVaucher;
    QMap<QString, QString> fLang;
    QString fAdgt;
    QString fVatDept;
    QString fVatReception;
    QString fNoVatDept;
    QString fTaxName;
    double fPrice;
    bool fNoManual;
} CI_InvoiceItem;
Q_DECLARE_METATYPE(CI_InvoiceItem*)

class CacheInvoiceItem : public CacheBase<CI_InvoiceItem>
{
public:
    static CacheInvoiceItem *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheInvoiceItem *fInstance;
    CacheInvoiceItem();
    ~CacheInvoiceItem();
};

#endif // CACHEINVOICEITEM_H
