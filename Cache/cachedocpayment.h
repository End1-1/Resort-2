#ifndef CACHEDOCPAYMENT_H
#define CACHEDOCPAYMENT_H

#include "cachebase.h"

#define cid_doc_payment 108

struct CI_DocPayment : public CI_Base
{};
Q_DECLARE_METATYPE(CI_DocPayment*)

class CacheDocPayment : public CacheBase<CI_DocPayment>
{
public:
    static CacheDocPayment *instance();
private:
    static CacheDocPayment *fInstance;
    CacheDocPayment();
    ~CacheDocPayment();
};

#endif // CACHEDOCPAYMENT_H
