#include "dlgconfigattendance.h"
#include "ui_dlgconfigattendance.h"
#include "database2.h"
#include "doubledatabase.h"
#include "message.h"

DlgConfigAttendance::DlgConfigAttendance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfigAttendance)
{
    ui->setupUi(this);
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.exec("select * from salary2_config");
    db.next();
    ui->ch1->setChecked(db.integer("f1"));
    ui->ch2->setChecked(db.integer("f2"));
    ui->ch3->setChecked(db.integer("f3"));
    ui->ch4->setChecked(db.integer("f4"));
    ui->ch5->setChecked(db.integer("f5"));
    ui->ch6->setChecked(db.integer("f6"));
    ui->ch7->setChecked(db.integer("f7"));
}

DlgConfigAttendance::~DlgConfigAttendance()
{
    delete ui;
}

void DlgConfigAttendance::on_btnCancel_clicked()
{
    reject();
}

void DlgConfigAttendance::on_btnOk_clicked()
{
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f1"] = (int) ui->ch1->isChecked();
    db[":f2"] = (int) ui->ch2->isChecked();
    db[":f3"] = (int) ui->ch3->isChecked();
    db[":f4"] = (int) ui->ch4->isChecked();
    db[":f5"] = (int) ui->ch5->isChecked();
    db[":f6"] = (int) ui->ch6->isChecked();
    db[":f7"] = (int) ui->ch7->isChecked();
    db.update("salary2_config", "f_id", 1);
    accept();
}
