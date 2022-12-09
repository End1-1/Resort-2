#include "dlgusergroup.h"
#include "ui_dlgusergroup.h"
#include "cacheusersgroups.h"

DlgUserGroup::DlgUserGroup(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_USER_GROUP, parent),
    ui(new Ui::DlgUserGroup)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leNameAm, "Name, am");
    fTable = "users_groups";
    fCacheId = cid_users_group;
}

DlgUserGroup::~DlgUserGroup()
{
    delete ui;
}


void DlgUserGroup::on_btnCancel_clicked()
{
    reject();
}

void DlgUserGroup::on_btnOk_clicked()
{
    save();
}
