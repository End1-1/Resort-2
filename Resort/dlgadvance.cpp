#include "dlgadvance.h"
#include "ui_dlgadvance.h"
#include "vauchers.h"
#include "dlgprinttaxsm.h"
#include "pprintvaucher.h"
#include "paymentmode.h"

DlgAdvance::DlgAdvance(const QString &reserve, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgAdvance)
{
    ui->setupUi(this);
    ui->leCardCode->setEnabled(false);
    ui->leCardName->setEnabled(false);
    ui->leReserve->setText(reserve);
    fDbBind[":f_id"] = reserve;
    ui->deDate->setDate(WORKING_DATE);
    fDb.select("select f_room, f_invoice, concat(g.f_firstName, ' ', g.f_lastName) "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "where r.f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leRoom->setText(fDbRows.at(0).at(0).toString());
        ui->leGuest->setText(fDbRows.at(0).at(2).toString());
        ui->leInvoice->setText(fDbRows.at(0).at(1).toString());
    }
    fDockPay = new DWSelectorPaymentMode(this);
    fDockPay->configure();
    fDockPay->setSelector(ui->lePaymentCode);
    connect(fDockPay, SIGNAL(paymentMode(CI_PaymentMode*)), this, SLOT(paymentMode(CI_PaymentMode*)));
    fDockCard = new DWSelectorCreditCard(this);
    fDockCard->configure();
    fDockCard->setSelector(ui->leCardCode);
    connect(fDockCard, SIGNAL(creditCard(CI_CreditCard*)), this, SLOT(creditCard(CI_CreditCard*)));
    fTrackControl = new TrackControl(TRACK_RESERVATION);
}

DlgAdvance::~DlgAdvance()
{
    delete ui;
}

void DlgAdvance::setVaucherId(const QString &id)
{
    ui->leVaucher->setText(id);
    fDbBind[":f_id"] = id;
    fDb.select("select r.f_room, r.f_invoice, concat(g.f_firstName, ' ', g.f_lastName), "
               "m.f_amountAmd, m.f_creditCard, m.f_paymentMode, m.f_doc, m.f_remarks, "
               "r.f_id, m.f_wdate, f_fiscal "
               "from m_register m "
               "left join f_reservation r on r.f_invoice=m.f_inv "
               "left join f_guests g on g.f_id=r.f_guest "
               "where m.f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leRoom->setText(fDbRows.at(0).at(0).toString());
        ui->leGuest->setText(fDbRows.at(0).at(2).toString());
        ui->leInvoice->setText(fDbRows.at(0).at(1).toString());
        ui->lePaymentCode->setText(fDbRows.at(0).at(5).toString());
        ui->leCardCode->setText(fDbRows.at(0).at(4).toString());
        ui->leAmount->setText(float_str(fDbRows.at(0).at(3).toDouble(), 0));
        //fVaucherDoc = fDbRows.at(0).at(6).toInt();
        ui->teRemarks->setPlainText(fDbRows.at(0).at(7).toString());
        ui->leReserve->setText(fDbRows.at(0).at(8).toString());
        ui->deDate->setDate(fDbRows.at(0).at(9).toDate());
        ui->leTax->setInt(fDbRows.at(0).at(10).toInt());
        CI_PaymentMode *cp = CachePaymentMode::instance()->get(ui->lePaymentCode->text());
        CI_CreditCard *cc = CacheCreditCard::instance()->get(ui->leCardCode->text());
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentCode, ui->lePaymentName, cp);
        if (cp) {
            bool cardCodeVisibility = cp->fCode.toInt() == PAYMENT_CARD;
            ui->leCardCode->setEnabled(cardCodeVisibility);
            ui->leCardName->setEnabled(ui->leCardCode->isEnabled());
            ui->lbCard->setEnabled(ui->leCardCode->isEnabled());
            if (!ui->leCardCode->isEnabled()) {
                ui->leCardCode->clear();
                ui->leCardName->clear();
            }
        }
        creditCard(cc);
    }
    ui->btnSave->setVisible(r__(cr__super_correction));
    ui->teRemarks->setEnabled(r__(cr__super_correction));
    ui->leAmount->setEnabled(r__(cr__super_correction));
    ui->lePaymentCode->setEnabled(r__(cr__super_correction));
    ui->leCardCode->setEnabled(r__(cr__super_correction));
}

void DlgAdvance::openAdvance(const QString &vaucherId)
{
    DlgAdvance *d = new DlgAdvance(0, fPreferences.getDefaultParentForMessage());
    d->setVaucherId(vaucherId);
    d->exec();
    delete d;
}

void DlgAdvance::paymentMode(CI_PaymentMode *c)
{
    if (c) {
        switch (c->fCode.toInt()) {
        case PAYMENT_CASH:
        case PAYMENT_CARD:
            break;
        default:
            message_error_tr("This mode of payment is not allowed here");
            return;
        }
    }
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentCode, ui->lePaymentName, c);
    if (c) {
        ui->leCardCode->setEnabled(c->fCode.toInt() == PAYMENT_CARD);
        ui->leCardName->setEnabled(ui->leCardCode->isEnabled());
        ui->lbCard->setEnabled(ui->leCardCode->isEnabled());
        if (!ui->leCardCode->isEnabled()) {
            ui->leCardCode->clear();
            ui->leCardName->clear();
        }
    }
}

void DlgAdvance::creditCard(CI_CreditCard *c)
{
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, c);
}

void DlgAdvance::on_btnClose_clicked()
{
    reject();
}

void DlgAdvance::on_btnPrint_clicked()
{
    if (ui->leVaucher->isEmpty()) {
        message_error_tr("Document not saved");
        return;
    }
    PPrintVaucher::printVaucher(ui->leVaucher->text());
}

void DlgAdvance::on_btnSave_clicked()
{
    if (ui->lePaymentCode->asInt() == 0) {
        message_error_tr("Mode of payment not selected");
        return;
    }
    if (ui->lePaymentCode->asInt() == PAYMENT_CARD) {
        if (ui->leCardCode->asInt() == 0) {
            message_error_tr("Card type not selected");
            return;
        }
    }
    if (ui->leAmount->asDouble() < 0.1) {
        message_error_tr("Amount must be greater then zero");
        return;
    }
    QString finalName = "CASH";
    switch (ui->lePaymentCode->asInt()) {
    case PAYMENT_CARD:
        finalName = ui->leCardName->text();
        break;
    }

    bool isNew = false;
    if (ui->leVaucher->isEmpty()) {
        isNew = true;
        ui->leVaucher->setText(uuid(VAUCHER_ADVANCE_N, fAirDb));
        fDb.insertId("m_register", ui->leVaucher->text());
    }
    fDbBind[":f_source"] = VAUCHER_ADVANCE_N;
    if (isNew) {
        fDbBind[":f_wdate"] = ui->deDate->date();
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
    } else {
        if (r__(cr__super_correction)) {
            fDbBind[":f_wdate"] = ui->deDate->date();
        }
    }
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_itemCode"] = fPreferences.getDb(def_receip_vaucher_id);
    fDbBind[":f_finalName"] = tr("ADVANCE") + " " + finalName;
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->lePaymentCode->asInt();
    fDbBind[":f_creditCard"] = ui->leCardCode->asInt();
    fDbBind[":f_cityLedger"] = 0;
    fDbBind[":f_paymentComment"] = finalName;
    fDbBind[":f_dc"] = "CREDIT";
    fDbBind[":f_sign"] = -1;
    fDbBind[":f_res"] = ui->leReserve->text();
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = ui->teRemarks->toPlainText();
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDbBind[":f_cash"] = 1;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    fDb.fDb.commit();
    ui->btnSave->setEnabled(false);

    QString msg = isNew ? "New advance" : "Advance modified";
    msg += " " + ui->leVaucher->text();
    QString value = ui->deDate->text() + "/" + ui->leRoom->text()
            + "/" + ui->leGuest->text() + "/" + ui->lePaymentName->text()
            + "/" + ui->leCardName->text() + "/" + finalName + "/" + ui->leAmount->text();
    fTrackControl->insert(msg, value, "");
}

void DlgAdvance::on_btnPrintTax_clicked()
{
    if (ui->leVaucher->text().isEmpty()) {
        message_error(tr("Save first"));
        return;
    }
    if (ui->leTax->asInt() > 0) {
        message_error(tr("Already printed"));
        return;
    }

    if (!(ui->lePaymentCode->asInt() == PAYMENT_CASH || ui->lePaymentCode->asInt() == PAYMENT_CARD)) {
        message_error_tr("Tax print is not available for this payment mode");
    }

    ///
    double cash = ui->lePaymentCode->asInt() == PAYMENT_CASH ? ui->leAmount->asDouble() : 0;
    double card = ui->lePaymentCode->asInt() == PAYMENT_CARD ? ui->leAmount->asDouble() : 0;
    int taxCode;
    if (!DlgPrintTaxSM::printAdvance(cash, card, ui->leVaucher->text(), taxCode)) {
        return;
    }
    //PrintTax::printAdvance(ui->leAmount->asDouble(), ui->lePaymentCode->asInt(), QString("AV%1").arg(ui->leVaucher->text()));
    ui->leTax->setInt(taxCode);


    ///

    fDbBind[":f_fiscal"] = taxCode;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));

    fDbBind[":f_vaucher"] = ui->leVaucher->text();
    fDbBind[":f_invoice"] = ui->leInvoice->text();
    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_name"] = tr("ADVANCE") + " " + ui->lePaymentName->text() + " " + ui->leCardName->text();
    fDbBind[":f_amountCash"] = ui->lePaymentCode->asInt() ==  PAYMENT_CASH ? ui->leAmount->asDouble() : 0;
    fDbBind[":f_amountCard"] = ui->lePaymentCode->asInt() ==  PAYMENT_CASH ? 0 : ui->leAmount->asDouble();
    fDbBind[":f_amountPrepaid"] = 0;
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_comp"] = HOSTNAME;
    fDb.insert("m_tax_history", fDbBind);
    fTrackControl->insert("Tax printed", ui->leVaucher->text(), ui->leAmount->text());

    message_info_tr("Document was sent to tax printer.");
}
