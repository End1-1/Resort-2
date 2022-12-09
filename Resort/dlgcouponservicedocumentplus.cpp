#include "dlgcouponservicedocumentplus.h"
#include "ui_dlgcouponservicedocumentplus.h"
#include "message.h"
#include "reportquery.h"
#include "dlgcouponservicefirstnumber.h"
#include "database2.h"

DlgCouponServiceDocumentPlus::DlgCouponServiceDocumentPlus(ReportQuery *rq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServiceDocumentPlus)
{
    ui->setupUi(this);
    ui->leQty->setValidator(new QIntValidator());
    ui->leDiscount->setValidator(new QDoubleValidator(0, 999999999, 2));
    rq->costumizeCombo(ui->cbType, "talon_type");
}

DlgCouponServiceDocumentPlus::~DlgCouponServiceDocumentPlus()
{
    delete ui;
}

void DlgCouponServiceDocumentPlus::getResult(QString &couponType, QString &first, QString &last, double &qty, double &price, double &discount, double &total)
{
    couponType = ui->cbType->currentText();
    first = ui->leStartNumber->text();
    last = ui->leEndNumber->text();
    qty = ui->leQty->asDouble();
    price = ui->lePrice->asDouble();
    discount = ui->leDiscount->asDouble();
    total = ui->leTotal->asDouble();
}

void DlgCouponServiceDocumentPlus::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(e);
}

void DlgCouponServiceDocumentPlus::on_btnCancel_clicked()
{
    reject();
}

void DlgCouponServiceDocumentPlus::on_btnOk_clicked()
{
    if (ui->cbType->currentIndex() < 0) {
        message_error(tr("Partner is not selected"));
        return;
    }
    if (ui->leStartNumber->text().isEmpty()) {
        message_error(tr("No valid coupons selected"));
        return;
    }
    if (ui->leEndNumber->text().isEmpty()) {
        message_error(tr("No valid coupons selected"));
        return;
    }
    if (ui->lePrice->asDouble() < 0.1) {
        message_error(tr("Price is not defined"));
        return;
    }
    accept();
}

void DlgCouponServiceDocumentPlus::on_leQty_textChanged(const QString &arg1)
{
    ui->leTotal->setDouble(ui->leQty->asDouble() * (ui->lePrice->asDouble() - ui->leDiscount->asDouble()));
    if (ui->cbType->currentIndex() > -1) {
        if (!ui->leStartNumber->text().isEmpty()) {
            if (ui->lbMax->text().toDouble() < arg1.toDouble()) {
                ui->leQty->clear();
                return;
            }
            int lastnum = ui->leStartNumber->text().rightRef(5).toInt() + arg1.toInt() - 1;
            ui->leEndNumber->setText(ui->leStartNumber->text().leftRef(9) + QString("%1").arg(lastnum, 5, 10, QChar('0')));
        }
    }
}

void DlgCouponServiceDocumentPlus::on_btnFirstNumber_clicked()
{
    if (ui->cbType->currentData().toInt() == 0) {
        message_error(tr("Coupon type is not selected"));
        return;
    }
    QString first, last;
    double qty;
    if (DlgCouponServiceFirstNumber::getFirstNumber(ui->cbType->currentData().toInt(), first, last, qty, this)) {
        ui->leStartNumber->setText(first);
        fLastNumber = last;
        ui->lbMax->setText(QString::number(qty));
        ui->leQty->setFocus();
    }
}

void DlgCouponServiceDocumentPlus::on_cbType_currentIndexChanged(int index)
{
    ui->leStartNumber->clear();
    ui->leEndNumber->clear();
    ui->lbMax->setText("");
    ui->lePrice->clear();
    ui->leTotal->clear();
    ui->leDiscount->clear();
    if (index > -1) {
        ui->lePrice->setText(ui->cbType->itemData(index, Qt::UserRole + 1).toString());
    }
}

void DlgCouponServiceDocumentPlus::on_leDiscount_textChanged(const QString &arg1)
{
    ui->leTotal->setDouble(ui->leQty->asDouble() * (ui->lePrice->asDouble() - arg1.toDouble()));
}

void DlgCouponServiceDocumentPlus::on_leStartNumber_returnPressed()
{
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db[":f_code"] = ui->leStartNumber->text();
    db.exec("select f_id, f_group, f_trsale from talon_service where f_code=:f_code");
    if (db.next() == false) {
        message_error(tr("Invalid code"));
        return;
    }
    if (db.integer("f_trsale") > 0) {
        message_error(tr("This coupon already saled"));
        return;
    }
    ui->cbType->setCurrentIndex(ui->cbType->findData(db.value("f_group")));
    db[":f_id"] = db.integer("f_id");
    db[":f_group"] = db.integer("f_group");
    db.exec("select f_id from talon_service where f_id>=:f_id and f_group=:f_group and f_trsale is null");
    int id = 0, count = 0;
    while (db.next()) {
        if (id == 0) {
            count++;
            id = db.integer("f_id");
            continue;
        }
        if (id + 1 < db.integer("f_id")) {
            break;
        }
        id = db.integer("f_id");
        count++;
    }
    ui->lbMax->setText(QString::number(count));
}
