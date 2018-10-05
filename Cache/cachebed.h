#ifndef CACHEBED_H
#define CACHEBED_H

#include "cachebase.h"

#define cid_bed 47

typedef struct : CI_Base {

} CI_Bed;
Q_DECLARE_METATYPE(CI_Bed*)

class CacheBed : public CacheBase<CI_Bed>
{
public:
    static CacheBed *instance();
private:
    static CacheBed *fInstance;
    CacheBed();
    ~CacheBed();
};

#endif // CACHEBED_H
