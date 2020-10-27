#include "eqcombobox.h"
#include "cacheforcombobox.h"
#include "cacheusersstate.h"
#include "cachepaymentmode.h"
#include "cachevatmode.h"
#include "cachedocpayment.h"
#include "cachsex.h"
#include "cachecoupontype.h"
#include "cacheresthall.h"
#include <QWheelEvent>

EQComboBox::EQComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

void EQComboBox::setIndexForData(const QVariant &data)
{
    int index = findData(data);
    setCurrentIndex(index);
}
int EQComboBox::getCache()
{
    return fCache;
}

int EQComboBox::asInt()
{
    return currentData().toInt();
}

void EQComboBox::setCache(int cache)
{
    fCache  = cache;
    switch (fCache) {

    case cid_users_state:
        CacheForComboBox::configure<CacheUsersState, CI_UsersState>(this);
        break;
    case cid_payment_mode:
        CacheForComboBox::configure<CachePaymentMode, CI_PaymentMode>(this);
        break;
    case cid_sex:
        CacheForComboBox::configure<CachSex, CI_Sex>(this);
        break;
    case cid_vat_mode:
        CacheForComboBox::configure<CacheVatMode, CI_VATMode>(this);
        break;
    case cid_coupon_type:
        CacheForComboBox::configure<CacheCouponType, CI_CouponType>(this);
        break;
    case cid_doc_payment:
        CacheForComboBox::configure<CacheDocPayment, CI_DocPayment>(this);
        break;
    case cid_rest_hall:
        CacheForComboBox::configure<CacheRestHall, CI_RestHall>(this);
        break;
    default:
        //message_error(QString("Please contact with application developer. Message: unknown cache id %1 for EQComboBox").arg(cache));
        break;
    }
}

QString EQComboBox::getField()
{
    return fField;
}

void EQComboBox::setField(const QString &name)
{
    fField = name;
}

void EQComboBox::wheelEvent(QWheelEvent *e)
{
    e->ignore();
}
