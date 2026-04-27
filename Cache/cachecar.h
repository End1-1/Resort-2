#ifndef CACHECAR_H
#define CACHECAR_H

#include "cachebase.h"

#define cid_car 106

struct CI_Car : public CI_Base
{};
Q_DECLARE_METATYPE(CI_Car*)

class CacheCar : public CacheBase<CI_Car>
{
public:
    static CacheCar *instance();
private:
    CacheCar();
    ~CacheCar();
    static CacheCar *fInstance;
};

#endif // CACHECAR_H
