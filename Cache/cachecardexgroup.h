#ifndef CACHECARDEXGROUP_H
#define CACHECARDEXGROUP_H

#include "cachebase.h"

#define cid_cardex_group 58

typedef struct : CI_Base {

} CI_CardexGroup;
Q_DECLARE_METATYPE(CI_CardexGroup*);

class CacheCardexGroup : public CacheBase<CI_CardexGroup>
{
public:
    static CacheCardexGroup *instance();
    virtual void updateItem(const QString &id);
private:
    static CacheCardexGroup *fInstance;
    CacheCardexGroup();
    ~CacheCardexGroup();
};

#endif // CACHECARDEXGROUP_H
