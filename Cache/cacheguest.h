#ifndef CACHEGUEST_H
#define CACHEGUEST_H

#include "cachebase.h"

#define cid_guest 43

struct CI_Guest : public CI_Base
{
    QString fTitle;
    QString fNatShort;
    QString fNatFull;
    QString fPassport;
    QString fFirstName;
    QString fLastName;
};
Q_DECLARE_METATYPE(CI_Guest*)

class CacheGuest : public CacheBase<CI_Guest>
{
public:
    static CacheGuest *instance();
    virtual void load();
    virtual void updateItem(const QString &id);
private:
    static CacheGuest *fInstance;
    CacheGuest();
    ~CacheGuest();
};

#endif // CACHEGUEST_H
