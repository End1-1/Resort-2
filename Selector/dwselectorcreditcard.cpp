#include "dwselectorcreditcard.h"

DWSelectorCreditCard::DWSelectorCreditCard(QWidget *parent) :
    DWSelector(cid_credit_card, parent)
{
    setWindowTitle(tr("Credit card"));
}

void DWSelectorCreditCard::configure()
{
    simpleTable<CI_CreditCard, CacheCreditCard>();
}

void DWSelectorCreditCard::select(const QVariant &value)
{
    emit creditCard(value.value<CI_CreditCard*>());
}

void DWSelectorCreditCard::selectorFocusOut()
{
    CI_CreditCard *c = 0;
    if (fLineSelector) {
        c = CacheCreditCard::instance()->get(fLineSelector->text());
    }
    emit creditCard(c);
}
