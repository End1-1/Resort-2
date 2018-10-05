#ifndef CACHETAXMAP_H
#define CACHETAXMAP_H

#include "cachebase.h"

#define cid_tax_map 105

typedef struct : CI_Base {
} CI_TaxMap;
Q_DECLARE_METATYPE(CI_TaxMap*)

class CacheTaxMap : public CacheBase<CI_TaxMap>
{
public:
    static CacheTaxMap *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheTaxMap *fInstance;
    CacheTaxMap();
    ~CacheTaxMap();
};

#endif // CACHETAXMAP_H
