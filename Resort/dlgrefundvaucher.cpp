#include "dlgrefundvaucher.h"
#include "ui_dlgrefundvaucher.h"
#include "cacheredreservation.h"
#include "message.h"
#include "pprintvaucher.h"
#include "paymentmode.h"
#include "vauchers.h"

DlgRefundVaucher::DlgRefundVaucher(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgRefundVaucher)
{
    ui->setupUi(this);
    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCLCode);
    connect(fDockCL, &DWSelectorCityLedger::cityLedger, [this](CI_CityLedger *cl) {
        dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, cl);
    });

    QStringList fp;
    fp << QString::number(PAYMENT_CASH)
       << QString::number(PAYMENT_BANK);
    fDockPay = new DWSelectorPaymentMode(this);
    fDockPay->configure();
    fDockPay->setSelector(ui->lePaymentMode);
    fDockPay->setFilterList(fp);
    connect(fDockPay, &DWSelectorPaymentMode::paymentMode, [this](CI_PaymentMode *cl) {
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentMode, ui->lePaymentName, cl);
        if (!cl) {
           // ui->leCLCode->setEnabled(false);
        } else {
           // ui->leCLCode->setEnabled(cl->fCode.toInt() == PAYMENT_CL);
        }
    });
}

DlgRefundVaucher::~DlgRefundVaucher()
{
    delete ui;
}

void DlgRefundVaucher::refundVaucher(const QString &vaucher, const QString &reserv)
{
    Q_UNUSED(vaucher);
    DlgRefundVaucher *d = new DlgRefundVaucher(fPreferences.getDefaultParentForMessage());
    if (!reserv.isEmpty()) {
        d->setReservation(reserv);
    }
    d->exec();
    delete d;
}

void DlgRefundVaucher::on_btnSave_clicked()
{
    if (ui->lePaymentMode->asInt() == 0) {
        message_error(tr("Payment mode was not selected"));
        return;
    }
    if (ui->leReserve->isEmpty() && ui->rgGuest->isChecked()) {
        message_error(tr("Reservation is not selected"));
        return;
    }
    if (ui->leVaucher->isEmpty()) {
        ui->leVaucher->setText(uuid("RF", fAirDb));
        fDbBind[":f_id"] = ui->leVaucher->text();
        fDbBind[":f_source"] = VAUCHER_REFUND_N;
        fDb.insertWithoutId("m_register", fDbBind);
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
    }

    fDbBind[":f_wdate"] = ui->deDate->date();
    fDbBind[":f_res"] = ui->leReserve->text();
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_itemCode"] = VAUCHER_REFUND;
    fDbBind[":f_finalName"] = ui->rgGuest->isChecked() ? tr("Refund") + " " + ui->leReserve->text() : tr("Refund") + " " + ui->leCLName->text();
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->lePaymentMode->asInt();
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
    fDbBind[":f_paymentComment"] = ui->rgGuest->isChecked() ? tr("Refund") + " " + ui->leReserve->text() : tr("Refund") + " " + ui->leCLName->text();;
    fDbBind[":f_dc"] = ui->rgGuest->isChecked() ? "CREDIT" : "CREDIT";
    fDbBind[":f_sign"] = ui->rgGuest->isChecked() ? 1 : -1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = "";
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDbBind[":f_rb"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    getBalance();
    message_info_tr("Saved");
}

void DlgRefundVaucher::setReservation(const QString &reserv)
{
    CI_RedReservation *ci = CacheRedReservation::instance()->get(reserv);
    if (ci) {
        ui->leReserve->setText(reserv);
        ui->leInvoice->setText(ci->fInvoice);
        ui->leGuest->setText(ci->fName);
        ui->leRoom->setText(ci->fRoom);
    }
    getBalance();
}

void DlgRefundVaucher::getBalance()
{
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDb.select("select sum(f_amountAmd*f_sign*-1) from m_register "
               "where f_inv=:f_invoice and f_finance=1 and f_canceled=0", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leBalance->setText(fDbRows.at(0).at(0).toString());
    }
}

void DlgRefundVaucher::on_btnPrint_clicked()
{
    if (ui->leVaucher->isEmpty()) {
        message_error_tr("Voucher is not saved");
        return;
    }
    PPrintVaucher::printVaucher(ui->leVaucher->text());
}

void DlgRefundVaucher::on_btnCancel_clicked()
{
    reject();
}

void DlgRefundVaucher::on_rgGuest_clicked(bool checked)
{
    ui->leCLCode->setEnabled(!checked);
    ui->leCLName->setEnabled(!checked);
}

void DlgRefundVaucher::on_rgCL_clicked(bool checked)
{
    ui->leCLCode->setEnabled(checked);
    ui->leCLName->setEnabled(checked);
}
