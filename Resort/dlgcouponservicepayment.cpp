#include "dlgcouponservicepayment.h"
#include "ui_dlgcouponservicepayment.h"
#include "database2.h"
#include "message.h"
#include "reportquery.h"

DlgCouponServicePayment::DlgCouponServicePayment(ReportQuery *rq, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCouponServicePayment)
{
    ui->setupUi(this);
    rq->costumizeCombo(ui->cbPartner, "partners", true, -1);
}

DlgCouponServicePayment::~DlgCouponServicePayment()
{
    delete ui;
}

void DlgCouponServicePayment::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(e);
}

void DlgCouponServicePayment::on_btnCancel_clicked()
{
    reject();
}

void DlgCouponServicePayment::on_btnSave_clicked()
{
    if (ui->cbPartner->currentData().toInt() == 0){
        message_error(tr("Partner not selected"));
        return;
    }
    if (ui->leAmount->asDouble() < 0.1) {
        message_error(tr("Amount cannot be zero"));
        return;
    }
    Database2 db;
    if (!db.open(__dd1Host, __dd1Database, __dd1Username, __dd1Password)) {
        message_error(db.lastDbError());
        return;
    }
    db.startTransaction();
    db[":f_type"] = 4;
    db[":f_date"] = ui->leDate->date();
    db[":f_amount"] = ui->leAmount->asDouble();
    int docid = 0;
    db.insert("talon_documents_header", docid);

    db[":f_document"] = docid;
    db[":f_refdocument"] = 0;
    db[":f_debit"] = ui->leAmount->asDouble();
    db[":f_credit"] = 0;
    db.insert("talon_payment");
    db.commit();

    ui->btnSave->setEnabled(false);
    ui->leAmount->setEnabled(false);
    ui->leDate->setEnabled(false);
}
