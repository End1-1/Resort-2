#ifndef CACHECREDITCARD_H
#define CACHECREDITCARD_H

#include "cachebase.h"

#define cid_credit_card 44

typedef struct : CI_Base {

} CI_CreditCard;
Q_DECLARE_METATYPE(CI_CreditCard*)

class CacheCreditCard : public CacheBase<CI_CreditCard>
{
public:
    static CacheCreditCard *instance();
private:
    static CacheCreditCard *fInstance;
    CacheCreditCard();
    ~CacheCreditCard();
};

#endif // CACHECREDITCARD_H
