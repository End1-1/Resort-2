#ifndef CACHSEX_H
#define CACHSEX_H

#include "cachebase.h"

#define cid_sex 52

typedef struct : CI_Base {

} CI_Sex;
Q_DECLARE_METATYPE(CI_Sex*)

class CachSex : public CacheBase<CI_Sex>
{
public:
    static CachSex *instance();
private:
    static CachSex *fInstance;
    CachSex();
    ~CachSex();
};

#endif // CACHSEX_H
