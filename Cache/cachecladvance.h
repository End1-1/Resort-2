#ifndef CACHECLADVANCE_H
#define CACHECLADVANCE_H

#include "cachebase.h"

#define cid_cache_cl_advance 114

typedef struct : CI_Base {
    double fAmount;
} CI_CLAdvance;
Q_DECLARE_METATYPE(CI_CLAdvance*)

class CacheCLAdvance : public CacheBase<CI_CLAdvance>
{
public:
    static CacheCLAdvance *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheCLAdvance *fInstance;
    CacheCLAdvance();
    ~CacheCLAdvance();
};

#endif // CACHECLADVANCE_H
