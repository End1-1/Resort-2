#ifndef CACHEUSERSGROUPS_H
#define CACHEUSERSGROUPS_H

#include "cachebase.h"

#define cid_users_group 40

struct CI_UsersGroups : public CI_Base
{};
Q_DECLARE_METATYPE(CI_UsersGroups*)

class CacheUsersGroups : public CacheBase<CI_UsersGroups>
{
public:
    static CacheUsersGroups *instance();
    virtual void updateItem(const QString &id);
private:
    static CacheUsersGroups *fInstance;
    CacheUsersGroups();
    ~CacheUsersGroups();
};

#endif // CACHEUSERSGROUPS_H
