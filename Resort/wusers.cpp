#include "wusers.h"
#include "reusers.h"
#include "dlguserpasswords.h"

WUsers::WUsers(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(0, "f_id", tr("Code"))
            .setColumn(0, "f_state", tr("State code"))
            .setColumn(100, "f_state_name", tr("State"))
            .setColumn(0, "f_group", tr("Group code"))
            .setColumn(150, "f_group_name", tr("Group"))
            .setColumn(150, "f_username", tr("Username"))
            .setColumn(150, "f_firstName", tr("First name"))
            .setColumn(150, "f_lastName", tr("Last name"));

    fModel->setSqlQuery("select u.f_id, u.f_state, s.f_" + def_lang + " as f_state_name, "
                        "u.f_group, g.f_" + def_lang + "  as f_group_name,"
                        "u.f_username, u.f_firstName, u.f_lastName "
                        "from users u "
                        "inner join users_states s on s.f_id=u.f_state "
                        "inner join users_groups g on g.f_id=u.f_group "
                        "order by u.f_group, u.f_firstName ");
    fModel->apply(this);

    fRowEditorDialog = new REUsers(fRowValues, this);
    setBtnNewVisible();
}

void WUsers::setupTab()
{
    setupTabTextAndIcon(tr("Users"), ":/images/boy.png");
    addToolBarButton(":/images/controls.png", tr("Change passwords"), SLOT(changePassword()));
}

void WUsers::processValues(int row, bool isNew)
{
    if (fRowValues.count() == 0 && !isNew) {
        return;
    }
    if (fRowValues.count() > 0 && fRowValues.at(0).toInt() == 1) {
        message_info_tr("This is not editable user");
        return;
    }
    WReportGrid::processValues(row, isNew);
}

void WUsers::changePassword()
{
    if (fRowValues.count() == 0) {
        return;
    }
    DlgUserPasswords *d = new DlgUserPasswords(this);
    d->setParams(fRowValues.at(0).toInt(), fRowValues.at(6).toString() + " " + fRowValues.at(7).toString());
    d->exec();
    delete d;
}
