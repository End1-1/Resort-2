#ifndef CACHEDISCOUNTTYPE_H
#define CACHEDISCOUNTTYPE_H

#include "cachebase.h"

#define cid_discount_type 118

struct CI_DiscountType : public CI_Base
{};
Q_DECLARE_METATYPE(CI_DiscountType*)

class CacheDiscountType : public CacheBase<CI_DiscountType>
{
public:
    static CacheDiscountType *instance();
private:
    CacheDiscountType();
    ~CacheDiscountType();
    static CacheDiscountType *fInstance;
};

#endif // CACHEDISCOUNTTYPE_H
