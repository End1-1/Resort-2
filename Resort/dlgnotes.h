#ifndef DLGNOTES_H
#define DLGNOTES_H

#include "roweditordialog.h"
#include "dwselectorguest.h"
#include "dwselectorusersgroups.h"
#include "dwselectorroom.h"

namespace Ui {
class DlgNotes;
}

class DlgNotes : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit DlgNotes(QList<QVariant> &values, QWidget *parent = 0);
    ~DlgNotes();
    virtual void setValues();
    void setScheduleVisible(bool value = true);

private slots:
    void guest(CI_Guest *c);
    void room(CI_Room *c);
    void usersGroup(CI_UsersGroups *c);
    void on_buttonBox_accepted();
    void on_btnSchedule_clicked();
    void on_btnDissmiss_clicked();
    void on_buttonBox_rejected();

private:
    Ui::DlgNotes *ui;
    DWSelectorGuest *fDockGuest;
    DWSelectorUsersGroups *fDockGroup;
    DWSelectorRoom *fDockRoom;
};

#endif // DLGNOTES_H
