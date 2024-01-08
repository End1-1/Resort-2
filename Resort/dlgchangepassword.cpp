#include "dlgchangepassword.h"
#include "ui_dlgchangepassword.h"
#include "message.h"

DlgChangePassword::DlgChangePassword(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgChangePassword)
{
    ui->setupUi(this);
}

DlgChangePassword::~DlgChangePassword()
{
    delete ui;
}

void DlgChangePassword::on_btnOK_clicked()
{
    if (ui->leNewPassword->text() != ui->leConfirmPassword->text()) {
        message_error(tr("Password does not match"));
        return;
    }
    if (ui->leNewPassword->text().isEmpty()) {
        message_error(tr("The password cannot be empty"));
        return;
    }
    fDbBind[":f_id"] = fPreferences.getLocal(def_working_user_id);
    fDbBind[":f_password"] = ui->leOldPassword->text();
    fDb.select("select * from users where f_id=:f_id and f_password=md5(:f_password)", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error(tr("The old password is incorrect"));
        return;
    }
    fDbBind[":f_id"] = fPreferences.getLocal(def_working_user_id);
    fDbBind[":f_password"] = ui->leNewPassword->text();
    fDb.select("update users set f_password=md5(:f_password) where f_id=:f_id", fDbBind, fDbRows);
    accept();
    message_info(tr("Password was changed"));
}

void DlgChangePassword::on_btnCancel_clicked()
{
    reject();
}
