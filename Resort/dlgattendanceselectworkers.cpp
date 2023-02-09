#include "dlgattendanceselectworkers.h"
#include "ui_dlgattendanceselectworkers.h"
#include "database2.h"
#include "message.h"

DlgAttendanceSelectWorkers::DlgAttendanceSelectWorkers(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgAttendanceSelectWorkers)
{
    ui->setupUi(this);
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

DlgAttendanceSelectWorkers::~DlgAttendanceSelectWorkers()
{
    delete ui;
}

void DlgAttendanceSelectWorkers::setListOfChecked(const QList<int> &idlist)
{
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        ui->tbl->item(i, 3)->setCheckState(idlist.contains(ui->tbl->item(i, 0)->text().toInt()) ? Qt::Checked : Qt::Unchecked);
    }
}

void DlgAttendanceSelectWorkers::on_btnSave_clicked()
{
    if (ui->cbBranch->currentIndex() < 0) {
        message_error("Նշեք մասնաճյուղը");
        return;

    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_date"] = ui->leDate->date();
    db[":f_branch"] = ui->cbBranch->currentData();
    db.exec("select f_employee from salary2 where f_date=:f_date and :f_branch=:f_branch");
    QStringList currentId;
    while (db.next()) {
        currentId.append(db.string("f_employee"));
    }
    QStringList newId;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        if (ui->tbl->item(i, 3)->checkState() == Qt::Checked) {
            newId.append(ui->tbl->item(i, 0)->text());
        }
    }
    for (const QString &id: currentId) {
        if (!newId.contains(id)) {
            db[":f_branch"] = ui->cbBranch->currentData();
            db[":f_date"] = ui->leDate->date();
            db[":f_employee"] = id;
            db.exec("delete from salary where f_date=:f_date and f_branch=:f_branch and f_employee=:f_employee");
            db[":f_branch"] = ui->cbBranch->currentData();
            db[":f_date"] = ui->leDate->date();
            db[":f_employee"] = id;
            db.exec("delete from salary2 where f_date=:f_date and f_branch=:f_branch and f_employee=:f_employee");
        }
    }
    for (const QString &id: newId) {
        if (!currentId.contains(id)) {
            db[":f_date"] = ui->leDate->date();
            db[":f_branch"] = ui->cbBranch->currentData();
            db[":f_employee"] = id;
            db[":f_amount"]= 0;
            db.insert("salary");
            db[":f_date"] = ui->leDate->date();
            db[":f_branch"] = ui->cbBranch->currentData();
            db[":f_employee"] = id;
            db[":f_amount"]= 0;
            db.insert("salary2");
        }
    }
    accept();
}

void DlgAttendanceSelectWorkers::getList()
{
    if (ui->cbBranch->currentIndex() < 0) {
        message_error("Նշեք մասնաճյուղը");
        return;

    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.exec("select u.f_id as `Կոդ`, p.f_en `Հաստիկ`, concat_ws(' ', u.f_lastname, u.f_firstname) as `Անուն`, 'X' "
            "from users u "
            "left join users_groups p on p.f_id=u.f_group "
            "where u.f_state=1 "
            "order by 3 ");
    QStringList labels;
    for (int i = 0; i < db.columnCount(); i++) {
        labels.append(db.columnName(i));
    }
    ui->tbl->setHorizontalHeaderLabels(labels);
    while (db.next()) {
        int r = ui->tbl->addRow();
        for (int c = 0; c < ui->tbl->columnCount() - 1; c++) {
            ui->tbl->setItem(r, c, new QTableWidgetItem(db.value(c).toString()));
        }
        ui->tbl->setItem(r, 3, new QTableWidgetItem());
        ui->tbl->item(r, 3)->setCheckState(Qt::Unchecked);
    }
    ui->tbl->resizeColumnsToContents();
    db[":f_date"] = ui->leDate->date();
    db[":f_branch"] = ui->cbBranch->currentData();
    db.exec("select f_employee from salary2 where f_date=:f_date and f_branch=:f_branch");
    while (db.next()) {
        for (int i = 0; i < ui->tbl->rowCount(); i++) {
            if (ui->tbl->item(i, 0)->text().toInt() == db.integer("f_employee")) {
                ui->tbl->item(i, 3)->setCheckState(Qt::Checked);
                goto gotomark;
            }
        }
        gotomark: {}
    }
}

void DlgAttendanceSelectWorkers::on_chHideUnchecked_clicked(bool checked)
{
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        ui->tbl->setRowHidden(i, ui->tbl->item(i, 3)->checkState() == Qt::Unchecked && checked);
    }
}

void DlgAttendanceSelectWorkers::on_btnRefresh_clicked()
{
    getList();
}
