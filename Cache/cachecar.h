#ifndef CACHECAR_H
#define CACHECAR_H

#include "cachebase.h"

#define cid_car 106

typedef struct : CI_Base {

} CI_Car;
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
