#ifndef CACHSEX_H
#define CACHSEX_H

#include "cachebase.h"

#define cid_sex 52

struct CI_Sex : public CI_Base
{};
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
