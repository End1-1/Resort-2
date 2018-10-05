#include "dlgexportother.h"
#include "ui_dlgexportother.h"
#include "fvauchers.h"
#include <QTextBlock>

DlgExportOther::DlgExportOther(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgExportOther)
{
    ui->setupUi(this);
}

DlgExportOther::~DlgExportOther()
{
    delete ui;
}

void DlgExportOther::on_btnStart_clicked()
{
    Database db;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return;
    }
    db.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!db.open()) {
        message_error_tr("Cannot connect to second database");
        return;
    }
    if (ui->chCardex->isChecked()) {
        processTable("f_cardex", db);
    }
    if (ui->chCityLedger->isChecked()) {
        processTable("f_city_ledger", db);
    }
    if (ui->chCorrectCLNames) {
        FVauchers::correctCL(db);
    }
    if (ui->chUsers->isChecked()) {
        processTable("users", db);
    }
    if (ui->chMenu->isChecked()) {
        processTable("r_dish", db);
        processTable("r_dish_complex", db);
        processTable("r_dish_mod", db);
        processTable("r_dish_mod_required", db);
        processTable("r_dish_part", db);
        processTable("r_dish_type", db);
        processTable("r_complimentary_comment", db);
        processTable("r_banquet_comment", db);
        processTable("r_menu", db);
        processTable("r_menu_names", db);
    }
    if (ui->chReports->isChecked()) {
        processTable("serv_reports", db);
    }
    if (ui->chUserRights->isChecked()) {
        processTable2("users_rights", db);
    }
    message_info_tr("Synchronization complete");
}

void DlgExportOther::processTable(const QString &table, Database &db)
{
    DatabaseResult dr;
    QMap<QString, QVariant> sdbBind;
    dr.select(fDb, "select * from " + table, fDbBind);
    ui->teLog->appendPlainText(QString("Process %1. %2 rows.")
                               .arg(table)
                               .arg(dr.rowCount()));
    ui->teLog->appendPlainText(tr("Go!"));
    for (int i = 0, count = dr.rowCount(); i < count; i++) {
        dr.getBindValues(i, sdbBind);
        if (table == "f_city_ledger") {
            if (sdbBind[":f_extra1"].toString().length() > 0) {
                sdbBind[":f_name"] = sdbBind[":f_extra1"];
                sdbBind[":f_address"] = sdbBind[":f_extra2"];
                sdbBind[":f_phone"] = "";
                sdbBind[":f_email"] = "";
            }
        }
        if (table == "f_cardex") {
            if (sdbBind[":f_extra1"].toString().length() > 0) {
                sdbBind[":f_name"] = sdbBind[":f_extra1"];
                sdbBind[":f_addr1"] = sdbBind[":f_extra2"];
                sdbBind[":f_addr2"] = "";
                sdbBind[":f_contact"] = "";
                sdbBind[":f_contactPos"] = "";
                sdbBind[":f_email"] = "";
                sdbBind[":f_fax"] = "";
                sdbBind[":f_tel"] = "";
            }
        }

        db.deleteEntry(table, sdbBind[":f_id"]);
        db.insertWithoutId(table, sdbBind);
        if (i % 50 == 0) {
            logCount(dr.rowCount(), i);
        }
    }
    logCount(dr.rowCount(), dr.rowCount());
}

void DlgExportOther::processTable2(const QString &table, Database &db)
{
    DatabaseResult dr;
    QMap<QString, QVariant> sdbBind;
    dr.select(fDb, "select * from " + table, fDbBind);
    ui->teLog->appendPlainText(QString("Process %1. %2 rows.")
                               .arg(table)
                               .arg(dr.rowCount()));
    ui->teLog->appendPlainText(tr("Go!"));
    db.select("delete from " + table);
    for (int i = 0, count = dr.rowCount(); i < count; i++) {
        dr.getBindValues(i, sdbBind);
        if (table == "f_city_ledger") {
            if (sdbBind[":f_extra1"].toString().length() > 0) {
                sdbBind[":f_name"] = sdbBind[":f_extra1"];
                sdbBind[":f_address"] = sdbBind[":f_extra2"];
                sdbBind[":f_phone"] = "";
                sdbBind[":f_email"] = "";
            }
        }
        if (table == "f_cardex") {
            if (sdbBind[":f_extra1"].toString().length() > 0) {
                sdbBind[":f_name"] = sdbBind[":f_extra1"];
                sdbBind[":f_addr1"] = sdbBind[":f_extra2"];
                sdbBind[":f_addr2"] = "";
                sdbBind[":f_contact"] = "";
                sdbBind[":f_contactPos"] = "";
                sdbBind[":f_email"] = "";
                sdbBind[":f_fax"] = "";
                sdbBind[":f_tel"] = "";
            }
        }
        sdbBind.remove(":f_id");
        db.insert(table, sdbBind);
        if (i % 50 == 0) {
            logCount(dr.rowCount(), i);
        }
    }
    logCount(dr.rowCount(), dr.rowCount());
}

void DlgExportOther::logCount(int total, int current)
{
    QString text = QString("%1 of %2").arg(current).arg(total);
    QTextBlock tb = ui->teLog->document()->findBlockByLineNumber(ui->teLog->document()->lineCount() - 1);
    if (tb.isValid()) {
        QTextCursor cursor(tb);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    }
    ui->teLog->appendPlainText(text);
    qApp->processEvents();
}

void DlgExportOther::on_btnCancel_clicked()
{
    reject();
}
