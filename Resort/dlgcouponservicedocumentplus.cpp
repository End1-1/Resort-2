#include "dlgcouponservicedocumentplus.h"
#include "ui_dlgcouponservicedocumentplus.h"
#include "message.h"
#include "reportquery.h"
#include "dlgcouponservicefirstnumber.h"
#include "database2.h"

static  QSettings _s("a", "b");

DlgCouponServiceDocumentPlus::DlgCouponServiceDocumentPlus(ReportQuery *rq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServiceDocumentPlus)
{
    ui->setupUi(this);
    ui->lePrice->setText(_s.value("lastcouponprice").toString());
    ui->leDiscount->setValidator(new QDoubleValidator(0, 999999999, 2));
    ui->leDiscount->setText(_s.value("lastcoupondiscount").toString());
    rq->costumizeCombo(ui->cbType, "talon_type");
    ui->cbType->setCurrentIndex(_s.value("lastcoupontype").toInt());
    ui->leStartNumber->setFocus();
    connect(ui->leStartNumber, &EQLineEdit::focusOut, this, &DlgCouponServiceDocumentPlus::startNumFocusOut);
}

DlgCouponServiceDocumentPlus::~DlgCouponServiceDocumentPlus()
{
    delete ui;
}

void DlgCouponServiceDocumentPlus::getResult(QString &couponType, QString &first, double &price, double &discount,
        double &total, int &group)
{
    couponType = ui->cbType->currentText();
    first = ui->leStartNumber->text();
    price = ui->lePrice->asDouble();
    discount = ui->leDiscount->asDouble();
    total = ui->leTotal->asDouble();
    group = ui->cbType->currentData().toInt();
}

void DlgCouponServiceDocumentPlus::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(e);
}

void DlgCouponServiceDocumentPlus::startNumFocusOut()
{
    if (ui->leStartNumber->text().isEmpty() == false) {
        on_leStartNumber_returnPressed();
    }
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
    if (ui->lePrice->asDouble() < 0.1) {
        message_error(tr("Price is not defined"));
        return;
    }
    accept();
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
    }
}

void DlgCouponServiceDocumentPlus::on_cbType_currentIndexChanged(int index)
{
    ui->leStartNumber->clear();
    ui->lePrice->clear();
    ui->leTotal->clear();
    ui->leDiscount->clear();
    if (index > -1) {
        ui->lePrice->setText(ui->cbType->itemData(index, Qt::UserRole + 1).toString());
    }
    if (index > 0) {
        _s.setValue("lastcoupontype", index);
    }
}

void DlgCouponServiceDocumentPlus::on_leDiscount_textChanged(const QString &arg1)
{
    ui->leTotal->setDouble(1 * (ui->lePrice->asDouble() - arg1.toDouble()));
    _s.setValue("lastcoupondiscount", arg1);
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
        if (ui->cbType->currentData().toInt()  == 0) {
            message_error(tr("Invalid code"));
            return;
        }
        db[":f_code"] = ui->leStartNumber->text();
        db[":f_group"] = ui->cbType->currentData().toInt();
        db[":f_price"] = ui->lePrice->asDouble();
        db[":f_trregister"] = -1;
        db.insert("talon_service");
        on_leStartNumber_returnPressed();
        return;
    }
    if (db.integer("f_trsale") != 0) {
        ui->leStartNumber->clear();
        message_error(tr("This coupon already saled"));
        return;
    }
    ui->cbType->setCurrentIndex(ui->cbType->findData(db.integer("f_group")));
    ui->lePrice->setEnabled(true);
    ui->lePrice->setFocus();
    ui->leTotal->setText(ui->lePrice->text());
}

void DlgCouponServiceDocumentPlus::on_lePrice_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        return;
    }
    _s.setValue("lastcouponprice", arg1);
}
