#ifndef DWSELECTORUSERSGROUPS_H
#define DWSELECTORUSERSGROUPS_H

#include "dwselector.h"
#include "cacheusersgroups.h"

class DWSelectorUsersGroups : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorUsersGroups(QWidget *parent = 0);
    virtual void configure();
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
signals:
    void userGroup(CI_UsersGroups *c);
};

#endif // DWSELECTORUSERSGROUPS_H
