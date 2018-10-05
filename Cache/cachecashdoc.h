#ifndef CACHECASHDOC_H
#define CACHECASHDOC_H

#include "cachebase.h"

#define cid_cash_doc 111

#define CASHDOC_STORE 1
#define CASHDOC_SALE 2
#define CASHDOC_SALARY 3
#define CASHDOC_MOVE 4
#define CASHDOC_DEBT 5

typedef struct : CI_Base {

} CI_CashDoc;
Q_DECLARE_METATYPE(CI_CashDoc*)

class CacheCashDoc : public CacheBase<CI_CashDoc>
{
public:
    static CacheCashDoc *instance();
private:
    static CacheCashDoc *fInstance;
    CacheCashDoc();
    ~CacheCashDoc();
};

#endif // CACHECASHDOC_H
