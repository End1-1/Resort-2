#include "cachecar.h"

CacheCar *CacheCar::fInstance = 0;

CacheCar *CacheCar::instance()
{
    if (!fInstance) {
        fInstance = new CacheCar();
        fCacheOne[cid_car] = fInstance;
    }
    return fInstance;
}

CacheCar::CacheCar()
{
    fCacheId = cid_car;
    fQuery = "select f_id, concat(f_model, ' ', f_class) from d_car_model";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCar::~CacheCar()
{
    fInstance = 0;
}
