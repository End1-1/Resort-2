#ifndef DLGADDATTENDANCE_H
#define DLGADDATTENDANCE_H

#include "baseextendeddialog.h"
#include "dwselectorusers.h"
#include "dwselectorusersgroups.h"

namespace Ui {
class DlgAddAttendance;
}


class DlgAddAttendance : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgAddAttendance(QWidget *parent = nullptr);
    ~DlgAddAttendance();
    int fRecId;

private:
    Ui::DlgAddAttendance *ui;

private slots:
    void branchDoubleClicked(bool v);
    void user(CI_User *u);
    void userGroup(CI_UsersGroups* u);
    void on_btnAdd_clicked();
};

#endif // DLGADDATTENDANCE_H
