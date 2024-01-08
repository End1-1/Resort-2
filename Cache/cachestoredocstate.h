#ifndef CACHESTOREDOCSTATE_H
#define CACHESTOREDOCSTATE_H

#include "cachebase.h"

#define cid_store_doc_state 103

typedef struct CI_StoreDocState : CI_Base {

} CI_StoreDocState;
Q_DECLARE_METATYPE(CI_StoreDocState*)

class CacheStoreDocState : public CacheBase<CI_StoreDocState>
{
public:
    static CacheStoreDocState *instance();
private:
    static CacheStoreDocState *fInstance;
    CacheStoreDocState();
    ~CacheStoreDocState();
};

#endif // CACHESTOREDOCSTATE_H
