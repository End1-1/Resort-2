#ifndef CACHEDEBTCOSTUMER_H
#define CACHEDEBTCOSTUMER_H

#include "cachebase.h"

#define cid_debt_costumer 112

struct CI_DebtCostumer : public CI_Base
{};
Q_DECLARE_METATYPE(CI_DebtCostumer *)


class CacheDebtCostumer : public CacheBase<CI_DebtCostumer>
{
public:
    static CacheDebtCostumer *instance();
private:
    static CacheDebtCostumer *fInstance;
    CacheDebtCostumer();
    ~CacheDebtCostumer();
};

#endif // CACHEDEBTCOSTUMER_H
