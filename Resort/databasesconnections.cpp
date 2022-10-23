#include "databasesconnections.h"
#include "ui_databasesconnections.h"
#include "databaseconnection.h"
#include "databasepasswordchange.h"

DatabasesConnections::DatabasesConnections(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DatabasesConnections)
{
    ui->setupUi(this);
    loadDatabases();
    ui->chDirectConnection->setChecked(__s.value("db_direct_connection").toBool());
    ui->tableDb->setColumnHidden(4, true);
}

DatabasesConnections::~DatabasesConnections()
{
    delete ui;
}

void DatabasesConnections::on_btnAdd_clicked()
{
    DatabaseConnection d(this);
    if (d.exec() == QDialog::Accepted)
        loadDatabases();
}

void DatabasesConnections::loadDatabases()
{
    ui->tableDb->clearContents();
    ui->tableDb->setRowCount(0);
    QStringList dbNames;
    fPreferences.setDatabasesNames(dbNames);
    for (QStringList::const_iterator it = dbNames.constBegin(); it != dbNames.constEnd(); it++) {
        Db db = fPreferences.getDatabase(*it);
        QList<QVariant> values;
        values << db.dc_name
               << db.dc_main_host
               << db.dc_main_path
               << db.dc_main_user
               << db.dc_main_pass;
        Utils::tableAppendRowData(ui->tableDb, values, Qt::EditRole);
    }
}

void DatabasesConnections::on_btnEdit_clicked()
{
    QList<QVariant> data;
    if (Utils::tableGetCurrentRowData(ui->tableDb, data) > -1) {
        DatabaseConnection d(this);
        d.setConnection(data.at(0).toString());
        if (d.exec() == QDialog::Accepted)
            loadDatabases();
    }
}

void DatabasesConnections::on_btnRemove_clicked()
{
    QList<QVariant> data;
    int row = -1;
    if ((row = Utils::tableGetCurrentRowData(ui->tableDb, data)) != -1) {
        if (message_confirm_tr("Confirm to delete selected database") == QDialog::Accepted) {
            fPreferences.removeDatabase(data.at(0).toString());
            ui->tableDb->removeRow(row);
        }
    }
}

void DatabasesConnections::on_btnChangePassword_clicked()
{
    DatabasePasswordChange d(this);
    d.exec();
}

void DatabasesConnections::on_chDirectConnection_clicked(bool checked)
{
    __s.setValue("db_direct_connection", checked);
}
