#ifndef CACHEUNIT_H
#define CACHEUNIT_H


#include "cachebase.h"

#define cid_unit 119

struct CI_Unit : public CI_Base
{};
Q_DECLARE_METATYPE(CI_Unit*)

class CacheUnit : public CacheBase<CI_Unit>
{
public:
    static CacheUnit *instance();
private:
    static CacheUnit *fInstance;
    CacheUnit();
    ~CacheUnit();
};

#endif // CACHEUNIT_H
