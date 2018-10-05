#ifndef CACHEDISCOUNTTYPE_H
#define CACHEDISCOUNTTYPE_H

#include "cachebase.h"

#define cid_discount_type 118

typedef struct : CI_Base {

} CI_DiscountType;
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
