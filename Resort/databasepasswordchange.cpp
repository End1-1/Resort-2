#include "databasepasswordchange.h"
#include "ui_databasepasswordchange.h"

DatabasePasswordChange::DatabasePasswordChange(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DatabasePasswordChange)
{
    ui->setupUi(this);
}

DatabasePasswordChange::~DatabasePasswordChange()
{
    delete ui;
}

void DatabasePasswordChange::on_btnCancel_clicked()
{
    reject();
}

void DatabasePasswordChange::on_btnOk_clicked()
{
    if (ui->leOldPassword->text() != fPreferences.getString(def_preferences_password)) {
        message_error("Invalid message_error_trord");
        return;
    }
    fPreferences.set(def_preferences_password, ui->leNewPassword->text());
    message_info_tr("The database password was changed");
    accept();
}
