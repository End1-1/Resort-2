#include "loginsettings.h"
#include "ui_loginsettings.h"

LoginSettings::LoginSettings(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::LoginSettings)
{
    ui->setupUi(this);
}

LoginSettings::~LoginSettings()
{
    delete ui;
}

void LoginSettings::on_btnCancel_clicked()
{
    reject();
}

void LoginSettings::on_btnOk_clicked()
{
    if (ui->lePassword->text() == fPreferences.getString(def_preferences_password)) {
        accept();
    } else {
        message_error_tr("Access denied");
    }
}
