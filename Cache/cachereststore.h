#ifndef CACHERESTSTORE_H
#define CACHERESTSTORE_H

#include "cachebase.h"

#define cid_rest_store 31

struct CI_RestStore : public CI_Base
{};
Q_DECLARE_METATYPE(CI_RestStore*)

class CacheRestStore : public CacheBase<CI_RestStore>
{
public:
    static CacheRestStore *instance();
    virtual void load();
private:
    static CacheRestStore *fInstance;
    CacheRestStore();
    ~CacheRestStore();
};

#endif // CACHERESTSTORE_H
