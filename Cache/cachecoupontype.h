#ifndef CACHECOUPONTYPE_H
#define CACHECOUPONTYPE_H

#include "cachebase.h"

#define cid_coupon_type 104

#define COUPON_STATE_NEW 1
#define COUPON_STATE_DELIVERED 2
#define COUPON_STATE_USED 3
#define COUPON_STATE_EXPARED 4

typedef struct : CI_Base {

} CI_CouponType;
Q_DECLARE_METATYPE(CI_CouponType*)

class CacheCouponType : public CacheBase<CI_CouponType>
{
public:
    static CacheCouponType *instance();
private:
    static CacheCouponType *fInstance;
    CacheCouponType();
    ~CacheCouponType();
};

#endif // CACHECOUPONTYPE_H
