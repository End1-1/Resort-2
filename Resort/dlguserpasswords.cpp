#include "dlguserpasswords.h"
#include "ui_dlguserpasswords.h"

DlgUserPasswords::DlgUserPasswords(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgUserPasswords)
{
    ui->setupUi(this);
    ui->leOldPass->hide();
    ui->leNewPass->setEnabled(false);
    ui->lePos->setEnabled(false);
}

DlgUserPasswords::~DlgUserPasswords()
{
    delete ui;
}

void DlgUserPasswords::setSelfMode()
{
    ui->chManager->hide();
    ui->chWaiter->hide();
    ui->leOldPass->show();
    ui->leNewPass->setEnabled(true);
    ui->lePos->hide();
    fUserId = WORKING_USERID;
    ui->lbUsername->setText(WORKING_USERNAME);
}

void DlgUserPasswords::setParams(int userId, const QString &name)
{
    ui->leOldPass->hide();
    fUserId = userId;
    ui->lbUsername->setText(name);
}

void DlgUserPasswords::on_btnCancel_clicked()
{
    reject();
}

void DlgUserPasswords::on_btnOk_clicked()
{
    if (ui->chManager->isChecked()) {
        if (ui->leNewPass->text().isEmpty()) {
            if (message_confirm_tr("Are you sure to clear password?") != QDialog::Accepted) {
                return;
            }
        } else {
            fDbBind[":f_password"] = ui->leNewPass->text();
            fDbBind[":f_id"] = fUserId;
            fDb.select("update users set f_password=md5(:f_password) where f_id=:f_id", fDbBind, fDbRows);
        }
    }
    if (ui->chWaiter->isChecked()) {
        fDbBind[":f_altPassword"] = ui->lePos->text();
        fDb.select("update users set f_altPassword='' where f_altPassword=md5(:f_altPassword)", fDbBind, fDbRows);
        fDbBind[":f_altPassword"]  = ui->lePos->text();
        fDbBind[":f_id"] = fUserId;
        fDb.select("update users set f_altPassword=md5(:f_altPassword) where f_id=:f_id", fDbBind, fDbRows);
    }
    if (ui->leOldPass->isVisible()) {
        if (ui->leNewPass->text().isEmpty()) {
            if (message_confirm_tr("Are you sure to clear password?") != QDialog::Accepted) {
                return;
            }
            fDbBind[":f_id"] = fUserId;
            fDb.select("update users set f_password='' where f_id=:f_id");
        } else {
            fDbBind[":f_password"] = ui->leOldPass->text();
            fDbBind[":f_id"] = fUserId;
            fDb.select("select f_id from users where f_password=md5(:f_password) and f_id=:f_id", fDbBind, fDbRows);
            if (fDbRows.count() == 0) {
                message_error_tr("Incorrect current password, try againg");
                return;
            }
            fDbBind[":f_password"] = ui->leNewPass->text();
            fDbBind[":f_id"] = fUserId;
            fDb.select("update users set f_password=md5(:f_password) where f_id=:f_id", fDbBind, fDbRows);
        }
    }
    TrackControl::insert(TRACK_USER, "Password changed", "", "", QString::number(fUserId));
    message_info_tr("Password was changed");
    accept();
}


void DlgUserPasswords::on_chManager_clicked(bool checked)
{
    ui->leNewPass->setEnabled(checked);
}


void DlgUserPasswords::on_chWaiter_clicked(bool checked)
{
    ui->lePos->setEnabled(checked);
}
