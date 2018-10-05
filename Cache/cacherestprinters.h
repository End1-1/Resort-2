#ifndef CACHERESTPRINTERS_H
#define CACHERESTPRINTERS_H

#include "cachebase.h"

#define cid_rest_printers 30

typedef struct : CI_Base {
} CI_RestPrinter;
Q_DECLARE_METATYPE(CI_RestPrinter*)

class CacheRestPrinters : public CacheBase<CI_RestPrinter>
{
public:
    static CacheRestPrinters *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheRestPrinters *fInstance;
    CacheRestPrinters();
    ~CacheRestPrinters();
};

#endif // CACHERESTPRINTERS_H
