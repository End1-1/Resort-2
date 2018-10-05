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
#include "dlgprinttaxsm.h"
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
        ui->leCode->setText(uuid("CH", fAirDb));
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
    if (ui->leCode->isEmpty()) {
        message_error(tr("Save first"));
        return;
    }
//    if (ui->lePaymentMode->asInt() != PAYMENT_CASH || ui->lePaymentMode->asInt() != PAYMENT_CARD) {

//    }
    QString itemCode = ui->rbCancelation->isChecked() ? fPreferences.getDb(def_cancelfee_code).toString() : fPreferences.getDb(def_noshowfee_code).toString();
    CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(itemCode);
    if (!ii) {
        return;
    }
    CI_TaxMap *tm = CacheTaxMap::instance()->get(itemCode);
    if (!tm) {
        message_error(tr("No tax department defined for ") + ii->fName);
        return;
    }
    int pm = ui->lePaymentMode->asInt();
//    double cash = pm == PAYMENT_CASH ? ui->leAmount->asDouble() : 0;
    double card = pm == PAYMENT_CARD ? ui->leAmount->asDouble() : 0;
    double prepaid = pm == PAYMENT_ADVANCE ? ui->leAmount->asDouble() : 0;
    DlgPrintTaxSM dpt;
    dpt.addGoods(ii->fVatDept,
                 ii->fAdgt,
                 ii->fCode,
                 ii->fTaxName,
                 ui->leAmount->asDouble(),
                 1);
    dpt.fOrder = ui->leCode->text();
    dpt.fCardAmount = card;
    dpt.fPrepaid = prepaid;
    int result = dpt.exec();
    if (result == TAX_OK) {
        fDbBind[":f_fiscal"] = dpt.fTaxCode;
        fDb.update("m_register", fDbBind, where_id(ap(ui->leCode->text())));
        ui->leTaxCode->setInt(dpt.fTaxCode);

        fDbBind[":f_vaucher"] = ui->leCode->text();
        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_name"] = (ui->rbCancelation->isChecked() ? tr("Cancelation fee") + " " + ui->leReserve->text()
                                                             : tr("No show fee") + " " + ui->leReserve->text() );
        fDbBind[":f_amountCash"] = pm == PAYMENT_CASH ? ui->leAmount->asDouble() : 0;
        fDbBind[":f_amountCard"] = pm == PAYMENT_CARD ? ui->leAmount->asDouble() : 0;
        fDbBind[":f_amountPrepaid"] = pm == PAYMENT_ADVANCE ? ui->leAmount->asDouble() : 0;
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comp"] = HOSTNAME;
        fDb.insert("m_tax_history", fDbBind);

        TrackControl::insert(TRACK_RESERVATION, ui->rbCancelation->isChecked() ? tr("Cancelation fee tax") : tr("No show fee tax"),
                      ui->leTaxCode->text(), ui->leCode->text(), ui->leCode->text(), ui->leInvoice->text(), ui->leReserve->text());
        message_info_tr("Tax printed");
    }
    //dpt.fPrepaid = ui->leTaxPre->asDouble();
    /*
    QDir d;
    QString fileName = QString("tax_%1.json").arg(ui->leInvoice->text());
    QFile f(d.currentPath() + "/" + fileName);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QString("{\"seq\":1,\"paidAmount\":%1, \"paidAmountCard\":%2, \"partialAmount\":0, "
                        "\"prePaymentAmount\":%3, \"useExtPOS\":true, \"mode\":2,\"items\":[")
                 .arg(float_str(pm == PAYMENT_CASH ? ui->leAmount->asDouble() : 0, 2))
                 .arg(float_str(pm == PAYMENT_CASH ? 0 : ui->leAmount->asDouble(), 2))
                 .arg(float_str(0, 2)).toUtf8());

            f.write(QString("{\"dep\":%1,\"qty\":%2,\"price\":%3, \"totalPrice\":%4, "
                            "\"productCode\":\"%5\",\"productName\":\"%6\",\"adgCode\":\"%7\", \"unit\":\"%8\"}")
                    .arg(ui->leVATCode->asInt() == VAT_INCLUDED ? tm->fName : ii->fNoVatDept)
                    .arg("1")
                    .arg(ui->leAmount->text())
                    .arg(ui->leAmount->text())
                    .arg(ii->fCode)
                    .arg(ii->fTaxName)
                    .arg(ii->fAdgt)
                    .arg(QString::fromUtf8("հատ"))
                    .toUtf8());

        f.write("]}");
        f.close();
    }
    QStringList args;
    args << fPreferences.getDb(def_tax_address).toString()
         << fPreferences.getDb(def_tax_port).toString()
         << fPreferences.getDb(def_tax_password).toString()
         << fileName;
    QProcess *p = new QProcess();
    p->start(d.currentPath() + "/printtax.exe", args);
    */
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
