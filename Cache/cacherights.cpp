#include "cacherights.h"
#include "doubledatabase.h"
#include "message.h"
#include "database2.h"

UserPermssions *UserPermssions::fInstance= nullptr;

UserPermssions::UserPermssions(int group)
{
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_group"] = group;
    db.exec("select f_right from users_rights where f_group=:f_group and f_flag=1");
    while (db.next()) {
        fPermissions.append(db.integer("f_right"));
    }
}

void UserPermssions::init(int group)
{
    if (fInstance) {
        fInstance->deleteLater();
    }
    fInstance = new UserPermssions(group);
}

bool UserPermssions::checkpermission(int permission)
{
    if (fInstance->fGroup == 1) {
        return true;
    }
    return fInstance->fPermissions.contains(permission);
}
