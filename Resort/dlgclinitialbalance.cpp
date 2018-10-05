#include "dlgclinitialbalance.h"
#include "ui_dlgclinitialbalance.h"
#include "vauchers.h"
#include "pprintvaucher.h"
#include "paymentmode.h"

DlgCLInitialBalance::DlgCLInitialBalance(int clCode, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgCLInitialBalance)
{
    ui->setupUi(this);
    ui->leText->setText(tr("C/L INITIAL BALANCE"));
    fCityLedger = CacheCityLedger::instance()->get(clCode);
    fTrackControl = new TrackControl(TRACK_VAUCHER);
    fTrackControl->addWidget(ui->deDate, "Date")
            .addWidget(ui->leAmount, "Amount")
            .addWidget(ui->leText, "Text")
            .addWidget(ui->rbCredit, "Credit/Debit");
}

DlgCLInitialBalance::~DlgCLInitialBalance()
{
    delete ui;
}

void DlgCLInitialBalance::openVaucher(const QString &id)
{
    DlgCLInitialBalance *d = new DlgCLInitialBalance(0, fPreferences.getDefaultParentForMessage());
    DatabaseResult dr;
    d->fDbBind[":f_id"] = id;
    dr.select(d->fDb, "select * from m_register where f_id=:f_id", d->fDbBind);
    if (dr.rowCount() == 0) {
        delete d;
        message_error_tr("Cannot open voucher");
        return;
    }
    d->fCityLedger = CacheCityLedger::instance()->get(dr.value("f_cityLedger").toString());
    d->ui->leVaucher->setText(id);
    d->ui->leAmount->setDouble(dr.value("f_amountAmd").toDouble());
    d->ui->leText->setText(dr.value("f_finalName").toString());
    d->ui->deDate->setDate(dr.value("f_wdate").toDate());
    d->ui->rbCredit->setChecked(dr.value("f_sign").toInt() == -1);
    d->fTrackControl->resetChanges();
    d->exec();
    delete d;
}

void DlgCLInitialBalance::on_btnCancel_clicked()
{
    reject();
}

void DlgCLInitialBalance::on_btnOk_clicked()
{
    QString source = VAUCHER_CL_INIT_N;
    if (ui->rbTC->isChecked()) {
        source = VAUCHER_TRANSFER_CL_N;
    }
    QString dc = "DEBIT";
    int sign = 1;
    if (ui->rbCredit->isChecked()) {
        dc = "CREDIT";
        sign = -1;
    }
    int item = 0;
    if (ui->rbTC->isChecked()) {
        item = 0;
    } else if (ui->rbCredit->isChecked()) {
        item = fPreferences.getDb(def_invoice_default_positive_transfer_id).toInt();
    } else {
        item = fPreferences.getDb(def_invoice_default_negative_transfer_id).toInt();
    }
    fDb.fDb.transaction();
    if (ui->leVaucher->isEmpty()) {
        QString rid = uuid("CR", fAirDb);
        fDb.insertId("m_register", rid);
        ui->leVaucher->setText(rid);
        fDbBind[":f_source"] = source;
        fDbBind[":f_wdate"] = ui->deDate->date();
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_room"] = fCityLedger->fCode;
        fDbBind[":f_guest"] = fCityLedger->fName;
        fDbBind[":f_itemCode"] = item;
        fDbBind[":f_finalName"] = ui->leText->text();
        fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
        fDbBind[":f_amountVat"] = 0;
        fDbBind[":f_amountUsd"] = def_usd;
        fDbBind[":f_fiscal"] = 0;
        fDbBind[":f_paymentMode"] = PAYMENT_CREDIT;
        fDbBind[":f_creditCard"] = 0;
        fDbBind[":f_cityLedger"] = fCityLedger->fCode;
        fDbBind[":f_paymentComment"] = fCityLedger->fName;
        fDbBind[":f_dc"] = dc;
        fDbBind[":f_sign"] = sign;
        fDbBind[":f_doc"] = "";
        fDbBind[":f_rec"] = "";
        fDbBind[":f_inv"] = "";
        fDbBind[":f_finance"] = 1;
        fDbBind[":f_remarks"] = "";
        fDbBind[":f_canceled"] = 0;
        fDbBind[":f_cancelReason"] = "";
        fDbBind[":f_side"] = 0;
        fDbBind[":f_rb"] = 1;
    }
    fDbBind[":f_itemCode"] = ui->rbCredit->isChecked() ? fPreferences.getDb(def_invoice_default_positive_transfer_id) : fPreferences.getDb(def_invoice_default_negative_transfer_id);
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_wdate"] = ui->deDate->date();
    fDbBind[":f_finalName"] = ui->leText->text();
    fDbBind[":f_dc"] = ui->rbCredit->isChecked() ? "CREDIT" : "DEBIT";
    fDbBind[":f_sign"] = ui->rbCredit->isChecked() ? -1 : 1;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    fTrackControl->saveChanges();
    fDb.fDb.commit();

    PPrintVaucher::printVaucher(ui->leVaucher->text());
    accept();
}
