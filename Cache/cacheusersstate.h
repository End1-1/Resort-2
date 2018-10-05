#ifndef CACHEUSERSSTATE_H
#define CACHEUSERSSTATE_H

#include "cachebase.h"

#define cid_users_state 41

typedef struct : CI_Base {

} CI_UsersState;
Q_DECLARE_METATYPE(CI_UsersState*)

class CacheUsersState : public CacheBase<CI_UsersState>
{
public:
    static CacheUsersState *instance();
private:
    static CacheUsersState *fInstance;
    CacheUsersState();
    ~CacheUsersState();
};

#endif // CACHEUSERSSTATE_H
