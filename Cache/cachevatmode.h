#ifndef CACHEVATMODE_H
#define CACHEVATMODE_H

#include "cachebase.h"

#define cid_vat_mode 22

typedef struct : CI_Base {
} CI_VATMode;
Q_DECLARE_METATYPE(CI_VATMode*)

class CacheVatMode : public CacheBase<CI_VATMode>
{
public:
    static CacheVatMode *instance();
    virtual void load();
private:
    static CacheVatMode *fInstance;
    CacheVatMode();
    ~CacheVatMode();
};

#endif // CACHEVATMODE_H
