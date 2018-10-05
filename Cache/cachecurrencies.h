#ifndef CACHECURRENCIES_H
#define CACHECURRENCIES_H

#include "cachebase.h"

#define cid_currencies 51

typedef struct : CI_Base {
    QString fDescription;
    double fRate;
} CI_Currency;

class CacheCurrencies : public CacheBase<CI_Currency>
{
public:
    static CacheCurrencies *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheCurrencies *fInstance;
    CacheCurrencies();
    ~CacheCurrencies();
    void fetchItem(QSqlQuery *q, CI_Currency *c);
};

#endif // CACHECURRENCIES_H
