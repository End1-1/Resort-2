#ifndef CACHEUSERS_H
#define CACHEUSERS_H

#include "cachebase.h"

#define cid_users 39

typedef struct : CI_Base {
    QString fFull;
    QString fGroup;
} CI_User;
Q_DECLARE_METATYPE(CI_User*)

class CacheUsers : public CacheBase<CI_User>
{
public:
    static CacheUsers *instance();
    virtual void load();
private:
    static CacheUsers *fInstance;
    CacheUsers();
    ~CacheUsers();
};

#endif // CACHEUSERS_H
