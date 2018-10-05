#ifndef CACHERESTDEBTHOLDER_H
#define CACHERESTDEBTHOLDER_H

#include "cachebase.h"

#define cid_rest_debt_holder 107

typedef struct : CI_Base {
    QString fGovNumber;
    QString fInfo;
} CI_RestDebtHolder;
Q_DECLARE_METATYPE(CI_RestDebtHolder*)

class CacheRestDebtHolder : public CacheBase<CI_RestDebtHolder>
{
public:
    static CacheRestDebtHolder *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheRestDebtHolder *fInstance;
    CacheRestDebtHolder();
    ~CacheRestDebtHolder();
};

#endif // CACHERESTDEBTHOLDER_H
