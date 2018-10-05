#ifndef CACHEVAUCHER_H
#define CACHEVAUCHER_H

#include "cachebase.h"

#define cid_vaucher 59

typedef struct : CI_Base {

} CI_Vaucher;
Q_DECLARE_METATYPE(CI_Vaucher*)

class CacheVaucher : public CacheBase<CI_Vaucher>
{
public:
    static CacheVaucher *instance();
private:
    static CacheVaucher *fInstance;
    CacheVaucher();
    ~CacheVaucher();
};

#endif // CACHEVAUCHER_H
