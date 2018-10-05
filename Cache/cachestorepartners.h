#ifndef CACHESTOREPARTNERS_H
#define CACHESTOREPARTNERS_H

#include "cachebase.h"

#define cid_store_partners 101

typedef struct : CI_Base {
    QString fTaxCode;
    QString fPhone;
    QString fComments;
} CI_StorePartners;
Q_DECLARE_METATYPE(CI_StorePartners*)

class CacheStorePartners : public CacheBase<CI_StorePartners>
{
public:
    static CacheStorePartners *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheStorePartners *fInstance;
    CacheStorePartners();
    ~CacheStorePartners();
};

#endif // CACHESTOREPARTNERS_H
