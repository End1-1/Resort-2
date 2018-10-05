#include "cachedebtcostumer.h"

CacheDebtCostumer *CacheDebtCostumer::fInstance = 0;

CacheDebtCostumer *CacheDebtCostumer::instance()
{
    if (!fInstance) {
        fInstance = new CacheDebtCostumer();
        fCacheOne[cid_debt_costumer] = fInstance;
    }
    return fInstance;
}

CacheDebtCostumer::CacheDebtCostumer() :
    CacheBase()
{
    fQuery = "select f_id, f_name from o_debt_holder ";
    fCacheId = cid_debt_costumer;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheDebtCostumer::~CacheDebtCostumer()
{
    fInstance = 0;
}
