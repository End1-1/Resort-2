#include "dlgcouponservicefirstnumber.h"
#include "ui_dlgcouponservicefirstnumber.h"
#include "database2.h"
#include "doubledatabase.h"
#include "message.h"

DlgCouponServiceFirstNumber::DlgCouponServiceFirstNumber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServiceFirstNumber)
{
    ui->setupUi(this);
    ui->tbl->setColumnWidth(0, 200);
    ui->tbl->setColumnWidth(0, 200);
    ui->tbl->setColumnWidth(0, 100);
}

DlgCouponServiceFirstNumber::~DlgCouponServiceFirstNumber()
{
    delete ui;
}

bool DlgCouponServiceFirstNumber::getFirstNumber(int type, QString &first, QString &last, double &qty, QWidget *parent)
{
       DlgCouponServiceFirstNumber d(parent);
       d.setup(type);
       bool result = d.exec() == QDialog::Accepted;
       if (result) {
           first = d.ui->tbl->item(d.fRow, 0)->data(Qt::EditRole).toString();
           last = d.ui->tbl->item(d.fRow, 1)->data(Qt::EditRole).toString();
           qty = d.ui->tbl->item(d.fRow, 2)->data(Qt::EditRole).toDouble();
       }
       return result;
}

void DlgCouponServiceFirstNumber::setup(int type)
{
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_group"] = type;
    db.exec("select f_code from talon_service where f_group=:f_group and f_trsale is null order by f_id");

    QString firstnumber, lastnumber, prevnumber;
    int curr = 0;
    while (db.next()) {
        lastnumber = db.string("f_code");
        if (firstnumber.isEmpty()) {
            firstnumber = lastnumber;
            int r = ui->tbl->rowCount();
            ui->tbl->setRowCount(r + 1);
            ui->tbl->setItem(r, 0, new QTableWidgetItem(firstnumber));
            curr = lastnumber.rightRef(5).toInt();
            continue;
        }
        int next = lastnumber.rightRef(5).toInt();
        if (next != curr + 1) {
            int first = firstnumber.rightRef(5).toInt();
            int last = prevnumber.rightRef(5).toInt();
            int r = ui->tbl->rowCount() - 1;
            ui->tbl->setItem(r, 1, new QTableWidgetItem(prevnumber));
            ui->tbl->setItem(r, 2, new QTableWidgetItem(QString::number(last - first + 1)));
            firstnumber = lastnumber;
            prevnumber = lastnumber;
            r = ui->tbl->rowCount();
            ui->tbl->setRowCount(r + 1);
            ui->tbl->setItem(r, 0, new QTableWidgetItem(firstnumber));
            curr = next;
            continue;
        }
        curr = next;
        prevnumber = lastnumber;
    }
    if (curr > 0) {
        int first = firstnumber.rightRef(5).toInt();
        int r = ui->tbl->rowCount() - 1;
        ui->tbl->setItem(r, 1, new QTableWidgetItem(lastnumber));
        ui->tbl->setItem(r, 2, new QTableWidgetItem(QString::number(curr - first + 1)));
    }
}

void DlgCouponServiceFirstNumber::on_tbl_doubleClicked(const QModelIndex &index)
{
    if (index.isValid() == false) {
        return;
    }
    fRow = index.row();
    accept();
}
