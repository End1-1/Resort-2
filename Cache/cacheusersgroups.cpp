#include "cacheusersgroups.h"

CacheUsersGroups *CacheUsersGroups::fInstance = 0;

CacheUsersGroups *CacheUsersGroups::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheUsersGroups();
        fCacheOne[cid_users_group] = fInstance;
    }
    return fInstance;
}

void CacheUsersGroups::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " where f_id=" + id);
    CI_UsersGroups *u = get(id);
    if (q->next()) {
        if (!u) {
            u = new CI_UsersGroups();
        }
        u->fCode = q->value(0).toString();
        u->fName = q->value(1).toString();
    } else {
        if (u) {
            u->fValid = false;
        }
    }
    if (u) {
        fStaticCache[fCacheId][u->fCode] = u;
    }
}

CacheUsersGroups::CacheUsersGroups() :
    CacheBase()
{
    fCacheId = cid_users_group;
    fQuery = "select f_id, f_" + def_lang + " from users_groups";
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheUsersGroups::~CacheUsersGroups()
{
    fInstance = 0;
}
