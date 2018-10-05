#include "wusersgroups.h"
#include "dlguserrights.h"
#include "dlgusergroup.h"

WUsersGroups::WUsersGroups(QWidget *parent) :
    WReportGrid(parent)
{
    fModel->setColumn(0, "f_id", tr("Code"))
            .setColumn(tr_col("am", 200), "f_am", tr("Group"))
            .setColumn(tr_col("en", 200), "f_en", tr("Group"))
            .setColumn(tr_col("ru", 200), "f_ru", tr("Group"));

    fModel->setSqlQuery("select f_id, f_am, f_en, f_ru from users_groups order by 2 ");
    fModel->apply(this);

    fRowEditorDialog = new DlgUserGroup(fRowValues, this);
    setBtnNewVisible();
}

void WUsersGroups::setupTab()
{
    setupTabTextAndIcon(tr("Users groups"), ":/images/guests.png");
    addToolBarButton(":/images/controls.png", tr("Permissions"), SLOT(groupsPermissions()));
}

void WUsersGroups::processValues(int row, bool isNew)
{
    if (fRowValues.count() == 0 && !isNew) {
        return;
    }
    if (!isNew) {
        message_info_tr("This is not editable group");
        return;
    }
    WReportGrid::processValues(row, isNew);
}

void WUsersGroups::groupsPermissions()
{
    if (fRowValues.count() == 0) {
        return;
    }
    if (fRowValues.at(0).toInt() == 1) {
        message_info_tr("This is not editable group");
        return;
    }
    DlgUserRights *ur = new DlgUserRights(this);
    ur->setup(fRowValues);
    ur->exec();
    delete ur;
}
