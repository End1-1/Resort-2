#ifndef CACHESTOREDOC_H
#define CACHESTOREDOC_H

#include "cachebase.h"

#define cid_store_doc_type 102

struct CI_StoreDocType : public CI_Base
{};
Q_DECLARE_METATYPE(CI_StoreDocType*)

class CacheStoreDocType : public CacheBase<CI_StoreDocType>
{
public:
    static CacheStoreDocType *instance();
    virtual void load();
private:
    CacheStoreDocType();
    ~CacheStoreDocType();
    static CacheStoreDocType *fInstance;
};

#endif // CACHESTOREDOC_H
