#include "dwselectorpaymentmode.h"

DWSelectorPaymentMode::DWSelectorPaymentMode(QWidget *parent) :
    DWSelector(cid_payment_mode, parent)
{
    setWindowTitle(tr("Payment mode"));
}

void DWSelectorPaymentMode::configure()
{
    simpleTable<CI_PaymentMode, CachePaymentMode>();
}

void DWSelectorPaymentMode::select(const QVariant &value)
{
    emit paymentMode(value.value<CI_PaymentMode*>());
}

void DWSelectorPaymentMode::selectorFocusOut()
{
    CI_PaymentMode *c = 0;
    if (fLineSelector) {
        c = CachePaymentMode::instance()->get(fLineSelector->text());
    }
    emit paymentMode(c);
}
