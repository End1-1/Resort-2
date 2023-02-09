#include "dlgcouponserviceback.h"
#include "ui_dlgcouponserviceback.h"
#include "database2.h"
#include "message.h"

DlgCouponServiceBack::DlgCouponServiceBack(ReportQuery *rq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServiceBack)
{
    ui->setupUi(this);
    ui->tbl->setColumnWidth(0, 100);
    ui->tbl->setColumnWidth(1, 200);
    ui->tbl->setColumnWidth(2, 100);
}

DlgCouponServiceBack::~DlgCouponServiceBack()
{
    delete ui;
}

void DlgCouponServiceBack::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(e);
}

void DlgCouponServiceBack::on_leCode_returnPressed()
{
    QString code = ui->leCode->text();
    ui->leCode->clear();
    if (code.isEmpty()) {
        return;
    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_code"] = code;
    db.exec("select t.*, g.f_name as groupname from talon_service t, talon_service_items_group g where g.f_id=t.f_group and f_code=:f_code");
    if (db.next() == false) {
        message_error(tr("Invalid code"));
        return;
    }
    if (db.integer("f_trsale") == 0) {
        message_error(tr("This coupon wasnt saled"));
        return;
    }
    if (db.integer("f_trback") > 0) {
        message_error(tr("This coupon already returned"));
        return;
    }
    int r = ui->tbl->rowCount();
    ui->tbl->setRowCount(r + 1);
    ui->tbl->setItem(r, 0, new QTableWidgetItem(db.string("f_code")));
    ui->tbl->setItem(r, 2, new QTableWidgetItem(db.string("groupname")));
    ui->tbl->setItem(r, 3, new QTableWidgetItem(QString::number(db.doubleValue("f_price") - db.doubleValue("f_discount"))));
    db[":f_id"] = db.integer("f_id");
    db.exec("select p.f_name from talon_service t "
           "left join talon_documents_header d on t.f_trsale=d.f_id "
            "left join r_partners p on p.f_id=d.f_partner "
            "where t.f_id=:f_id");
    if (db.next() == false) {
        message_error(tr("Error, partner not found"));
        return;
    }
    ui->tbl->setItem(r, 1, new QTableWidgetItem(db.string("f_name")));
    countAmount();
}

void DlgCouponServiceBack::countAmount()
{
    double amount = 0;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        amount += ui->tbl->item(i, 2)->data(Qt::EditRole).toDouble();
    }
    ui->leTotal->setDouble(amount);
}

void DlgCouponServiceBack::on_btnSave_clicked()
{
    if (ui->tbl->rowCount() == 0) {
        message_error(tr("Empty document"));
        return;
    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.startTransaction();
    db[":f_type"] = 3;
    db[":f_date"] = ui->leDate->date();
    db[":f_amount"] = ui->leTotal->asDouble();
    int docid = 0;
    db.insert("talon_documents_header", docid);
    bool first = true;
    QString codes;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        if (first) {
            first = false;
        } else {
            codes += ",";
        }
        codes += QString("'%1'").arg(ui->tbl->item(i, 0)->text());
    }
    if (!db.exec(QString("select * from talon_service where f_code in (%1) for update").arg(codes))) {
        message_error(db.lastQuery());
        db.rollback();
        return;
    }
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        db[":f_code"] = ui->tbl->item(i, 0)->text();
        db[":f_trback"] = docid;
        db.exec("update talon_service set f_trback=:f_trback where f_code=:f_code");
    }
    db.commit();
    ui->leDocument->setInt(docid);
    ui->leDate->setReadOnly(true);
    ui->btnSave->setEnabled(false);
    message_info(tr("Saved"));
}
