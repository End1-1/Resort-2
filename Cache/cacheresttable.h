#ifndef CACHERESTTABLE_H
#define CACHERESTTABLE_H

#include "cachebase.h"

#define cid_rest_table 27

struct CI_RestTable : public CI_Base
{
    int fHall;
};
Q_DECLARE_METATYPE(CI_RestTable*)

class CacheRestTable : public CacheBase<CI_RestTable>
{
public:
    static CacheRestTable *instance();
    virtual void load();
private:
    static CacheRestTable *fInstance;
    CacheRestTable();
    ~CacheRestTable();
};

#endif // CACHERESTTABLE_H
