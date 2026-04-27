#ifndef CACHEORDERSTATE_H
#define CACHEORDERSTATE_H

#include "cachebase.h"

#define cid_order_state  60

struct CI_OrderState : public CI_Base
{};
Q_DECLARE_METATYPE(CI_OrderState*)

class CacheOrderState : public CacheBase<CI_OrderState>
{
public:
    static CacheOrderState *instance();
private:
    static CacheOrderState *fInstance;
    CacheOrderState();
    ~CacheOrderState();
};

#endif // CACHEORDERSTATE_H
