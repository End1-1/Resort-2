#ifndef REUSERS_H
#define REUSERS_H

#include "roweditordialog.h"
#include "dwselectorusersgroups.h"

namespace Ui {
class REUsers;
}

class REUsers : public RowEditorDialog
{
    Q_OBJECT
public:
    explicit REUsers(QList<QVariant> &values, QWidget *parent = 0);
    ~REUsers();

private slots:
    void userGroup(CI_UsersGroups *c);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::REUsers *ui;
    DWSelectorUsersGroups *fDockGroup;

};

#endif // REUSERS_H
