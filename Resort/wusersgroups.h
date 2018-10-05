#ifndef WUSERSGROUPS_H
#define WUSERSGROUPS_H

#include "wreportgrid.h"

class WUsersGroups : public WReportGrid
{
    Q_OBJECT
public:
    WUsersGroups(QWidget *parent = 0);
protected:
    virtual void setupTab();
    virtual void processValues(int row, bool isNew);
private slots:
    void groupsPermissions();
};

#endif // WUSERSGROUPS_H
