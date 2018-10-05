#ifndef CACHERESTTABLE_H
#define CACHERESTTABLE_H

#include "cachebase.h"

#define cid_rest_table 27

typedef struct : CI_Base {
}CI_RestTable;
Q_DECLARE_METATYPE(CI_RestTable*)

class CacheRestTable : public CacheBase<CI_RestTable>
{
public:
    static CacheRestTable *instance();
private:
    static CacheRestTable *fInstance;
    CacheRestTable();
    ~CacheRestTable();
};

#endif // CACHERESTTABLE_H
