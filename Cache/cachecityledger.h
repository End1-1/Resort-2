#ifndef CACHECITYLEDGER_H
#define CACHECITYLEDGER_H

#include "cachebase.h"

#define cid_city_ledger 24

typedef struct : CI_Base {
    QString fVatMode;
    QString fVatName;
    QString fCommission;
    QString fDateTo;
    QString fExtra1;
    QString fExtra2;
} CI_CityLedger;
Q_DECLARE_METATYPE(CI_CityLedger*)

class CacheCityLedger : public CacheBase<CI_CityLedger>
{
public:
    static CacheCityLedger *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheCityLedger *fInstance;
    CacheCityLedger();
    ~CacheCityLedger();

};

#endif // CACHECITYLEDGER_H
