#ifndef CACHECOUPONSERIA_H
#define CACHECOUPONSERIA_H

#include "cachebase.h"

#define cid_coupon_seria 109

struct CI_CouponSeria : public CI_Base
{
    double fPrice;
    QStringList fItems;
};
Q_DECLARE_METATYPE(CI_CouponSeria*)

class CacheCouponSeria : public CacheBase<CI_CouponSeria>
{
public:
    static CacheCouponSeria *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheCouponSeria *fInstance;
    CacheCouponSeria();
    ~CacheCouponSeria();
};

#endif // CACHECOUPONSERIA_H
