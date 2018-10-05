#ifndef CACHERESTHALL_H
#define CACHERESTHALL_H

#include "cachebase.h"

#define cid_rest_hall 25

typedef struct : CI_Base {
} CI_RestHall;
Q_DECLARE_METATYPE(CI_RestHall*)

class CacheRestHall : public CacheBase<CI_RestHall>
{
public:
    static CacheRestHall *instance();
    virtual void load();
private:
    static CacheRestHall *fInstance;
    CacheRestHall();
    ~CacheRestHall();
};

#endif // CACHERESTHALL_H
