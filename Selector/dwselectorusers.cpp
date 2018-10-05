#include "dwselectorusers.h"

DWSelectorUsers::DWSelectorUsers(QWidget *parent) :
    DWSelector(cid_users, parent)
{
    setWindowTitle(tr("Users"));
}

void DWSelectorUsers::configure()
{
    simpleTable<CI_User, CacheUsers>();
}

void DWSelectorUsers::select(const QVariant &value)
{
    emit user(value.value<CI_User*>());
}

void DWSelectorUsers::selectorFocusOut()
{
    emit user(0);
}
