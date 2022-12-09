#include "wglobaldbconfig.h"
#include "ui_wglobaldbconfig.h"
#include "message.h"
#include "defrest.h"
#include "dlgeditservtax.h"

void WGlobalDbConfig::getCompSettings()
{
    ui->lwHost->clear();
    fDb.select("select f_comp, f_hall from s_tax_print", fDbBind, fDbRows);
    foreach_rows {
        QListWidgetItem *item = new QListWidgetItem(ui->lwHost);
        item->setText(it->at(0).toString());
        item->setData(Qt::UserRole, it->at(1));
        ui->lwHost->addItem(item);
    }
    ui->leHost->clear();
    ui->leHallCode->clear();
}

void WGlobalDbConfig::getAccess()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    DatabaseResult drdb;
    drdb.select(db, "select * from f_db", fDbBind);
    ui->tblAccess->clearContents();
    ui->tblAccess->setRowCount(0);
    ui->tblAccess->setColumnCount(1 + drdb.rowCount());
    QStringList colCaption;
    colCaption << tr("Station/Username");
    QMap<QString, int> dbAddr;
    QMap<int, int> dbColAddr;
    for (int i = 0; i < drdb.rowCount(); i++) {
        colCaption << drdb.value(i, "f_name").toString();
        dbAddr[drdb.value(i, "f_id").toString()] = i + 1;
        dbColAddr[i + 1] = drdb.value(i, "f_id").toInt();
    }
    ui->tblAccess->setHorizontalHeaderLabels(colCaption);
    DatabaseResult draccess;
    draccess.select(db, "select * from f_users", fDbBind);
    ui->tblAccess->setRowCount(draccess.rowCount());
    QMap<QString, int> userAddr;
    for (int i = 0; i < draccess.rowCount(); i++) {
        userAddr[draccess.value(i, "f_name").toString()] = i;
        ui->tblAccess->setItemWithValue(i, 0, draccess.value(i, "f_name"));
        for (int j = 1; j < ui->tblAccess->columnCount(); j++) {
            ui->tblAccess->addCheckBox(i, j);
            ui->tblAccess->setItemWithValue(i, j, "", dbColAddr[j]);
        }
    }

    draccess.select(db, "select * from f_access", fDbBind);
    for (int i = 0; i < draccess.rowCount(); i++) {
        int row = userAddr[draccess.value(i, "f_user").toString()];
        int col = dbAddr[draccess.value(i, "f_db").toString()];
        ui->tblAccess->checkBox(row, col)->setChecked(draccess.value(i, "f_right").toString() == "1");
    }
    ui->tblAccess->horizontalHeader()->resizeContentsPrecision();
}

void WGlobalDbConfig::getDatabases()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    DatabaseResult dr;
    dr.select(db, "select * from f_db", fDbBind);
    Utils::tableSetColumnWidths(ui->tblDatabases, ui->tblDatabases->columnCount(),
                                0, 120, 120, 200, 120, 120);
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = ui->tblDatabases->addRow();
        ui->tblDatabases->addLineEdit(row, 0, false)->setText(dr.value(i, "f_id").toString());
        ui->tblDatabases->addLineEdit(row, 1, false)->setText(dr.value(i, "f_name").toString());
        ui->tblDatabases->addLineEdit(row, 2, false)->setText(dr.value(i, "f_host").toString());
        ui->tblDatabases->addLineEdit(row, 3, false)->setText(dr.value(i, "f_database").toString());
        ui->tblDatabases->addLineEdit(row, 4, false)->setText(dr.value(i, "f_username").toString());
        ui->tblDatabases->addLineEdit(row, 5, false)->setText(dr.value(i, "f_password").toString());
    }
}


WGlobalDbConfig::WGlobalDbConfig(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WGlobalDbConfig)
{
    ui->setupUi(this);



    getCompSettings();
    getAccess();
    getDatabases();

    fDockHall = new DWSelectorHall(this);
    fDockHall->configure();
    fDockHall->setSelector(ui->leHallCode);
    connect(fDockHall, SIGNAL(hall(CI_RestHall*)), this, SLOT(hall(CI_RestHall*)));
}

WGlobalDbConfig::~WGlobalDbConfig()
{
    delete ui;
}

void WGlobalDbConfig::setupTab()
{
    setupTabTextAndIcon(tr("Global config"), ":/images/settings.png");
}

void WGlobalDbConfig::hall(CI_RestHall *c)
{
    dockResponse<CI_RestHall, CacheRestHall>(ui->leHallCode, ui->leHallName, c);
}

void WGlobalDbConfig::on_lwHost_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->leHost->setText(index.data(Qt::DisplayRole).toString());
    ui->leHallCode->setText(index.data(Qt::UserRole).toString());
    CI_RestHall *ch = CacheRestHall::instance()->get(ui->leHallCode->text());
    if (ch) {
        ui->leHallName->setText(ch->fName);
    }

    DatabaseResult dr;
    fDbBind[":f_comp"] = index.data(Qt::DisplayRole).toString();
    dr.select(fDb, "select f_key, f_value from r_config where upper(f_comp)=upper(:f_comp)", fDbBind);
    QMap<QString, QString> v;
    for (int i = 0; i < dr.rowCount(); i++) {
        v[dr.value(i, "f_key").toString()] = dr.value(i, "f_value").toString();
    }
    ui->chOpenTableAfterRun->setChecked(v[dr_open_table_after_run].toInt());
    ui->leFirstReceiptPrinter->setText(v[dr_first_receipt_printer]);
    ui->leSecondReceiptPrinter->setText(v[dr_second_receipt_printer]);
    ui->leDisc20->setText(v[dr_discount_20]);
    ui->leDisc50->setText(v[dr_discount_50]);
    ui->leS5Server->setText(v[dr_s5_ip]);
    ui->leS5Port->setText(v[dr_s5_port]);
    ui->leS5User->setText(v[dr_s5_user]);
    ui->leS5Pass->setText(v[dr_s5_pass]);
    ui->leBranch->setText(v[dr_branch]);
}

void WGlobalDbConfig::on_btnSaveRestaurant_clicked()
{
    if (ui->leHost->text().isEmpty()) {
        message_error_tr("Host not selected");
        return;
    }
    fDbBind[":f_hall"] = ui->leHallCode->text();
    fDb.update("s_tax_print", fDbBind, where_field("f_comp", "'" + ui->leHost->text() + "'"));
    fDbBind[":f_comp"] = ui->leHost->text();
    fDb.select("delete from r_config where f_comp=:f_comp", fDbBind, fDbRows);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_open_table_after_run;
    fDbBind[":f_value"] = QString("%1").arg((int)ui->chOpenTableAfterRun->isChecked());
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_first_receipt_printer;
    fDbBind[":f_value"] = ui->leFirstReceiptPrinter->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_second_receipt_printer;
    fDbBind[":f_value"] = ui->leSecondReceiptPrinter->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_discount_20;
    fDbBind[":f_value"] = ui->leDisc20->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_discount_50;
    fDbBind[":f_value"] = ui->leDisc50->text();
    fDb.insert("r_config", fDbBind);

    //S5
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_s5_ip;
    fDbBind[":f_value"] = ui->leS5Server->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_s5_port;
    fDbBind[":f_value"] = ui->leS5Port->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_s5_user;
    fDbBind[":f_value"] = ui->leS5User->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_s5_pass;
    fDbBind[":f_value"] = ui->leS5Pass->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_branch;
    fDbBind[":f_value"] = ui->leBranch->text();
    fDb.insert("r_config", fDbBind);


    getCompSettings();
    message_info_tr("Saved");
}

void WGlobalDbConfig::on_btnRefresh_clicked()
{
    getAccess();
}

void WGlobalDbConfig::on_btnSaveAccess_clicked()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    db.select("delete from f_access");
    for (int i = 0; i < ui->tblAccess->rowCount(); i++) {
        for (int j = 1; j < ui->tblAccess->columnCount(); j++) {
            fDbBind[":f_user"] = ui->tblAccess->toString(i, 0);
            fDbBind[":f_db"] = ui->tblAccess->item(i, j)->data(Qt::UserRole);
            fDbBind[":f_right"] = (ui->tblAccess->checkBox(i, j)->isChecked() ? 1 : 0);
            db.insert("f_access", fDbBind);
        }
    }
   // BroadcastThread::cmdCommand(cmd_global_settings, QMap<QString, QString>());
    message_info_tr("Saved");
}

void WGlobalDbConfig::on_btnSaveDatabases_clicked()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    for (int i = 0; i < ui->tblDatabases->rowCount(); i++) {
        fDbBind[":f_name"] = ui->tblDatabases->lineEdit(i, 1)->text();
        fDbBind[":f_host"] = ui->tblDatabases->lineEdit(i, 2)->text();
        fDbBind[":f_database"] = ui->tblDatabases->lineEdit(i, 3)->text();
        fDbBind[":f_username"] = ui->tblDatabases->lineEdit(i, 4)->text();
        fDbBind[":f_password"] = ui->tblDatabases->lineEdit(i, 5)->text();
        if (ui->tblDatabases->lineEdit(i, 0)->asInt() == 0) {
            ui->tblDatabases->lineEdit(i, 0)->setInt(db.insert("f_db", fDbBind));
        } else {
            db.update("f_db", fDbBind, where_id(ap(ui->tblDatabases->lineEdit(i, 0)->text())));
        }
    }
    message_info_tr("Saved");
}

void WGlobalDbConfig::on_btnRemoveStation_clicked()
{
    QModelIndexList sel = ui->tblAccess->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    if (message_confirm(tr("Confirm to remove station")) != QDialog::Accepted) {
        return;
    }
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    fDbBind[":f_name"] = ui->tblAccess->toString(sel.at(0).row(), 0);
    db.select("delete from f_users where lower(f_name)=lower(:f_name)", fDbBind, fDbRows);
    fDbBind[":f_user"] = ui->tblAccess->toString(sel.at(0).row(), 0);
    db.select("delete from f_access where lower(f_user)=lower(:f_user)", fDbBind, fDbRows);
    db.close();
    getAccess();
}

void WGlobalDbConfig::on_btnRefreshReset_clicked()
{
    getCompSettings();
}

void WGlobalDbConfig::on_btnRemoveRest_clicked()
{
    QModelIndex index = ui->lwHost->currentIndex();
    if (!index.isValid()) {
        message_error_tr("Nothing was selected");
    }
    if (message_confirm_tr("Confirm to delete") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_comp"] = index.data(Qt::DisplayRole).toString();
    fDb.select("delete from s_tax_print where f_comp=:f_comp", fDbBind, fDbRows);
    getCompSettings();
}

void WGlobalDbConfig::on_chShowLogs_clicked(bool checked)
{
    fPreferences.setDb(def_show_logs, checked);
}
