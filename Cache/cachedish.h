#ifndef CACHEDISH_H
#define CACHEDISH_H

#include "cachebase.h"

#define cid_dish 61

typedef struct : CI_Base {
    QString fUnitName;
    QString fStore;
} CI_Dish;
Q_DECLARE_METATYPE(CI_Dish*)

class CacheDish : public CacheBase<CI_Dish>
{
public:
    static CacheDish *instance();
    virtual void load();
private:
    static CacheDish *fInstance;
    CacheDish();
    ~CacheDish();
};

#endif // CACHEDISH_H
