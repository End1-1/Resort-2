#include "cacheusers.h"

CacheUsers *CacheUsers::fInstance = 0;

CacheUsers *CacheUsers::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheUsers();
        fCacheOne[cid_users] = fInstance;
    }
    return fInstance;
}

void CacheUsers::load()
{
    QSqlQuery *q = prepareDb();
    while (q->next()) {
        CI_User *c = new CI_User();
        c->fCode = q->value(0).toString();
        c->fName = q->value(1).toString();
        c->fFull = q->value(2).toString();
        c->fGroup = q->value(3).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
    closeDb(q);
}

CacheUsers::CacheUsers() :
    CacheBase()
{
    fCacheId = cid_users;
    fQuery = "select f_id, concat(f_firstName, ' ', f_lastName), f_userName,  f_group from users";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheUsers::~CacheUsers()
{
    fInstance = 0;
}
