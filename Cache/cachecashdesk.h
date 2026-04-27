#ifndef CACHECASHDESK_H
#define CACHECASHDESK_H

#include "cachebase.h"

#define cid_cash_desk 110

struct CI_CashDesk : public CI_Base
{};
Q_DECLARE_METATYPE(CI_CashDesk*)

class CacheCashDesk : public CacheBase<CI_CashDesk>
{
public:
    static CacheCashDesk *instance();
    virtual void updateItem(const QString &id);
private:
    static CacheCashDesk *fInstance;
    CacheCashDesk();
    ~CacheCashDesk();
};

#endif // CACHECASHDESK_H
