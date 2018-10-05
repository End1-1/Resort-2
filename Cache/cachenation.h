#ifndef CACHENATION_H
#define CACHENATION_H

#include "cachebase.h"

#define cid_nation 53

typedef struct : CI_Base {
} CI_Nation;
Q_DECLARE_METATYPE(CI_Nation*)

class CacheNation : public CacheBase<CI_Nation>
{
public:
    static CacheNation *instance();
private:
    static CacheNation *fInstance;
    CacheNation();
    ~CacheNation();
};

#endif // CACHENATION_H
