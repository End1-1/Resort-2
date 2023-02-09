#include "dlgaddattendance.h"
#include "ui_dlgaddattendance.h"
#include "database2.h"
#include "dlggetidname.h"

DlgAddAttendance::DlgAddAttendance(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgAddAttendance)
{
    ui->setupUi(this);

    DWSelectorUsers *du = new DWSelectorUsers();
    du->configure();
    du->setSelector(ui->leWorker);
    ui->leWorker->setSelector(du);
    connect(du, SIGNAL(user(CI_User*)), this, SLOT(user(CI_User*)));

    DWSelectorUsersGroups *dp = new DWSelectorUsersGroups();
    dp->configure();
    dp->setSelector(ui->lePos);
    ui->lePos->setSelector(dp);
    connect(dp, SIGNAL(userGroup(CI_UsersGroups*)), this, SLOT(userGroup(CI_UsersGroups*)));

    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.exec("select f_id, f_name from r_branch");
    while (db.next()) {
        ui->cbBranch->addItem(db.string("f_name"), db.integer("f_id"));
    }
}

DlgAddAttendance::~DlgAddAttendance()
{
    delete ui;
}


void DlgAddAttendance::user(CI_User *u)
{
    dockResponse<CI_User, CacheUsers>(ui->leWorker, ui->leWorkername, u);
    if (u) {
        CI_UsersGroups *ug = CacheUsersGroups::instance()->get(u->fGroup);
        if (ug) {
            userGroup(ug);
        }
    }
}

void DlgAddAttendance::userGroup(CI_UsersGroups *u)
{
    dockResponse<CI_UsersGroups, CacheUsersGroups>(ui->lePos, ui->lePosName, u);
}


void DlgAddAttendance::on_btnAdd_clicked()
{
    if (ui->cbBranch->currentIndex() < 0) {
        message_error(tr("Branch was not selected"));
        return;
    }
    if (ui->leWorker->asInt() == 0) {
        message_error(tr("Worker was not selected"));
        return;
    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_date"] = ui->leDate->date();
    db[":f_branch"] = ui->cbBranch->currentData();
    db[":f_employee"] = ui->leWorker->asInt();
    db[":f_amount"] = 0;
    db.insert("salary2", fRecId);
    accept();
}
