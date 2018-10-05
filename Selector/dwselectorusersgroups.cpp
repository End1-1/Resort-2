#include "dwselectorusersgroups.h"

DWSelectorUsersGroups::DWSelectorUsersGroups(QWidget *parent) :
    DWSelector(cid_users_group, parent)
{
    setWindowTitle(tr("Users groups"));
}

void DWSelectorUsersGroups::configure()
{
    simpleTable<CI_UsersGroups, CacheUsersGroups>();
}

void DWSelectorUsersGroups::select(const QVariant &value)
{
    emit userGroup(value.value<CI_UsersGroups*>());
}

void DWSelectorUsersGroups::selectorFocusOut()
{
    emit userGroup(0);
}
