#include "cachecouponseria.h"

CacheCouponSeria *CacheCouponSeria::fInstance = 0;

CacheCouponSeria *CacheCouponSeria::instance()
{
    if (!fInstance) {
        fInstance = new CacheCouponSeria();
        fCacheOne[cid_coupon_seria] = fInstance;
    }
    return fInstance;
}

void CacheCouponSeria::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_CouponSeria *ci = new CI_CouponSeria();
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fPrice = q->value(2).toDouble();
        ci->fItems = q->value(3).toString().split(",", QString::SkipEmptyParts);
        fStaticCache[fCacheId][ci->fCode] = ci;
    }
    closeDb(q);
}

void CacheCouponSeria::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + "where id=" + id);
    CI_CouponSeria *ci = get(id);
    if (q->next()) {
        if (!ci) {
            ci = new CI_CouponSeria();
        }
        ci->fCode = q->value(0).toString();
        ci->fName = q->value(1).toString();
        ci->fPrice = q->value(2).toDouble();
        ci->fItems = q->value(3).toString().split(",", QString::SkipEmptyParts);
        fStaticCache[fCacheId][ci->fCode] = ci;
    } else {
        if (ci) {
            ci->fValid = false;
        }
    }
    closeDb(q);
}

CacheCouponSeria::CacheCouponSeria() :
    CacheBase()
{
    fQuery = "select f_id, f_name, f_price, f_items from d_coupon_seria ";
    fCacheId = cid_coupon_seria;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCouponSeria::~CacheCouponSeria()
{
    fInstance = 0;
}
