#include "dlgnoshow.h"
#include "ui_dlgnoshow.h"
#include "pprintvaucher.h"
#include "message.h"
#include "dwselectorvatmode.h"
#include "dwselectorcityledger.h"
#include "dwselectorcreditcard.h"
#include "vauchers.h"
#include "cacheinvoiceitem.h"
#include "dwselectorredreservation.h"
#include "paymentmode.h"
#include "cachetaxmap.h"
#include <QProcess>
#include <QDir>

DlgNoShow::DlgNoShow(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgNoShow)
{
    ui->setupUi(this);
    DWSelectorVATMode *selVat = new DWSelectorVATMode(this);
    selVat->configure();
    selVat->setSelector(ui->leVATCode);
    connect(selVat, &DWSelectorVATMode::vatMode, [this](CI_VATMode *ci){
        dockResponse<CI_VATMode, CacheVatMode>(ui->leVATCode, ui->leVATName, ci);
    });
    DWSelectorCityLedger *selCL = new DWSelectorCityLedger(this);
    selCL->configure();
    selCL->setSelector(ui->leCLCode);
    connect(selCL, &DWSelectorCityLedger::cityLedger, [this](CI_CityLedger *ci) {
       dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, ci);
    });
    DWSelectorCreditCard *selCard = new DWSelectorCreditCard(this);
    selCard->configure();
    selCard->setSelector(ui->leCardCode);
    connect(selCard, &DWSelectorCreditCard::creditCard, [this](CI_CreditCard *ci) {
       dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, ci);
    });
    fDockPay = new DWSelectorPaymentMode(this);
    fDockPay->configure();
    fDockPay->setSelector(ui->lePaymentMode);
    QStringList fv;
    fv << QString::number(PAYMENT_CASH)
       << QString::number(PAYMENT_CARD)
       << QString::number(PAYMENT_CL);
    /*
       << QString::number(PAYMENT_ROOM)*/
    fDockPay->setFilterList(fv);
    connect(fDockPay, &DWSelectorPaymentMode::paymentMode, [this](CI_PaymentMode *ci){
       dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentMode, ui->lePaymentName, ci);
       ui->leCLCode->setEnabled(false);
       ui->leCardCode->setEnabled(false);
       if (ci) {
           switch( ci->fCode.toInt()) {
           case PAYMENT_CL:
               ui->leCLCode->setEnabled(true);
               ui->btnPrintTax->setEnabled(true);
               break;
           case PAYMENT_CARD:
               ui->leCardCode->setEnabled(true);
               ui->btnPrintTax->setEnabled(true);
               break;
           }
           ui->btnPrintTax->setEnabled(ci->fCode.toInt() != PAYMENT_CL);
       }
    });
    DWSelectorRedReservation *selRes = new DWSelectorRedReservation(this);
    selRes->configure();
    selRes->setSelector(ui->leReserve);
    connect(selRes, &DWSelectorRedReservation::redReservation, [this](CI_RedReservation *ci) {
        if (ci) {
            ui->leReserve->setText(ci->fCode);
            ui->leInvoice->setText(ci->fInvoice);
            ui->leGuest->setText(ci->fName);
            ui->leRoom->setText(ci->fRoom);
        }
    });
    ui->leVATCode->setInt(VAT_NOVAT);
    dockResponse<CI_VATMode, CacheVatMode>(ui->leVATCode, ui->leVATName, 0);
}

DlgNoShow::~DlgNoShow()
{
    delete ui;
}

void DlgNoShow::setReservation(const QString &reserv)
{
    CI_RedReservation *ci = CacheRedReservation::instance()->get(reserv);
    if (ci) {
        ui->leReserve->setText(ci->fCode);
        ui->leInvoice->setText(ci->fInvoice);
        ui->leGuest->setText(ci->fName);
        ui->leRoom->setText(ci->fRoom);
    }
    getBalance();
}

void DlgNoShow::load(const QString &id)
{
    DatabaseResult dr;
    fDbBind[":f_id"] = id;
    ui->leCode->setText(id);
    dr.select(fDb, "select * from m_register where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Invalid voucher id");
        return;
    }
    ui->deDate->setDate(dr.value("f_wdate").toDate());
    ui->leReserve->setText(dr.value("f_res").toString());
    ui->leInvoice->setText(dr.value("f_inv").toString());
    ui->leGuest->setText(dr.value("f_guest").toString());
    ui->leRoom->setText(dr.value("f_room").toString());
    ui->lePaymentMode->setText(dr.value("f_paymentMode").toString());
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentMode, ui->lePaymentName, 0);
    ui->leCLCode->setText(dr.value("f_cityLedger").toString());
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, 0);
    ui->leCardCode->setText(dr.value("f_creditCard").toString());
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, 0);
    ui->leAmount->setText(dr.value("f_amountAmd").toString());
    ui->leVATCode->setInt(VAT_NOVAT);
    dockResponse<CI_VATMode, CacheVatMode>(ui->leVATCode, ui->leVATName, 0);
    ui->btnPrintTax->setEnabled(dr.value("f_fiscal").toInt() == 0);
    fDbBind[":f_id"] = ui->leReserve->text();
    dr.select(fDb, "select * from f_reservation where f_id=:f_id", fDbBind);
    ui->rbCancelation->setChecked(fPreferences.getDb(def_cancelfee_code).toInt());
    if (dr.rowCount() == 0) {
        message_error_tr("Invalid reservation number");
        return;
    }
    ui->deEntry->setDate(dr.value("f_startDate").toDate());
    ui->deDeparture->setDate(dr.value("f_endDate").toDate());
    ui->leTaxCode->setInt(dr.value("f_fiscal").toInt());
    getBalance();
}

void DlgNoShow::timeout()
{
    exit(0);
}

void DlgNoShow::on_btnClose_clicked()
{
    reject();
}

void DlgNoShow::on_btnPrint_clicked()
{
    if (ui->leCode->isEmpty()) {
        message_error_tr("The voucher was not saved");
    } else {
        PPrintVaucher::printVaucher(ui->leCode->text());
    }
}

void DlgNoShow::on_btnSave_clicked()
{
    if (ui->leBalance->asDouble() > 0.01) {
        if (ui->leAmount->asDouble() > ui->leBalance->asDouble()) {
            message_error(tr("Amount cannot be greater than balance"));
            return;
        }
    }
    if (ui->lePaymentMode->asInt() == 0) {
        message_error(tr("Payment mode was note selected"));
        return;
    }
    if (ui->leAmount->asDouble() < 0.01) {
        message_error(tr("Amount cannot be 0"));
        return;
    }
    if (ui->leCode->isEmpty()) {
        ui->leCode->setText(uuuid("CH", fAirDb));
        fDbBind[":f_id"] = ui->leCode->text();
        fDbBind[":f_source"] = VAUCHER_POSTCHARGE_N;
        fDb.insertWithoutId("m_register", fDbBind);
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
    }

    fDbBind[":f_wdate"] = ui->deDate->date();
    fDbBind[":f_res"] = ui->leReserve->text();
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_itemCode"] = (ui->rbCancelation->isChecked() ? fPreferences.getDb(def_cancelfee_code).toInt() : fPreferences.getDb(def_noshowfee_code).toInt());
    fDbBind[":f_finalName"] = (ui->rbCancelation->isChecked() ? tr("Cancelation fee") + " " + ui->leReserve->text()
                                                              : tr("No show fee") + " " + ui->leReserve->text() );
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->leAmount->asDouble(), ui->leVATCode->asInt());
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->lePaymentMode->asInt();
    fDbBind[":f_creditCard"] = ui->leCardCode->asInt();
    fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
    fDbBind[":f_paymentComment"] = ui->lePaymentMode->asInt() == PAYMENT_CL ? ui->leCLName->text() : "";
    fDbBind[":f_dc"] = "DEBIT";
    fDbBind[":f_sign"] = 1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = "";
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDbBind[":f_rb"] = 0;
    fDbBind[":f_cash"] = ui->lePaymentMode->asInt() == PAYMENT_CL ? 0 : 1;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leCode->text())));
    message_info_tr("Saved");
    getBalance();
}

void DlgNoShow::on_btnPrintTax_clicked()
{

}

void DlgNoShow::getBalance()
{
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDb.select("select sum(f_amountAmd*f_sign*-1) from m_register "
               "where f_inv=:f_invoice and f_finance=1 and f_canceled=0", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leBalance->setText(fDbRows.at(0).at(0).toString());
    }
    QStringList fa, fp;
    fa << QString::number(PAYMENT_ADVANCE);
    fp << QString::number(PAYMENT_BANK)
       << QString::number(PAYMENT_CARD)
       << QString::number(PAYMENT_CASH)
       << QString::number(PAYMENT_CL);
    if (ui->leBalance->asDouble() > 0.01) {
        fDockPay->setFilterList(fa);
    } else {
        fDockPay->setFilterList(fp);
    }
}
