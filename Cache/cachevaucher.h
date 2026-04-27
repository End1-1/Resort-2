#ifndef CACHEVAUCHER_H
#define CACHEVAUCHER_H

#include "cachebase.h"

#define cid_vaucher 59

struct CI_Vaucher : public CI_Base
{};
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
