#include "dlgreceiptvaucher.h"
#include "ui_dlgreceiptvaucher.h"
#include "pprintpreview.h"
#include "ptextrect.h"
#include "pprintheader.h"
#include "cachepaymentmode.h"
#include "vauchers.h"
#include "pimage.h"
#include "paymentmode.h"
#include "pprintvaucher.h"
#include "cacheusers.h"

DlgReceiptVaucher::DlgReceiptVaucher(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReceiptVaucher)
{
    ui->setupUi(this);
    ui->leOpcode->setInt(WORKING_USERID);
    ui->leOpName->setText(CacheUsers::instance()->get(WORKING_USERID)->fFull);
    ui->leRoom->setVisible(false);
    ui->leName->setReadOnly(!r__(cr__super_correction));
    ui->deDate->setDate(WORKING_DATE);
    fDockRoom = new DWSelectorActiveRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->lePartnerCode);
    connect(fDockRoom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomChanged(CI_ActiveRoom*)));
    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    connect(fDockCL, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));
    fDockPM = new DWSelectorPaymentMode(this);
    fDockPM->configure();
    fDockPM->setSelector(ui->lePaymentCode);
    connect(fDockPM, SIGNAL(paymentMode(CI_PaymentMode*)), this, SLOT(paymentMode(CI_PaymentMode*)));
    QStringList pm;
    pm << QString::number(PAYMENT_CASH)
       << QString::number(PAYMENT_CARD)
       << QString::number(PAYMENT_BANK)
       << QString::number(PAYMENT_BARTER);
    fDockPM->setFilterList(pm);
    fDockCard = new DWSelectorCreditCard(this);
    fDockCard->configure();
    fDockCard->setSelector(ui->leCardCode);
    connect(fDockCard, SIGNAL(creditCard(CI_CreditCard*)), this, SLOT(creditCard(CI_CreditCard*)));
    cardVisible(false);
    fTrackControl = new TrackControl(TRACK_RESERVATION);
}

DlgReceiptVaucher::~DlgReceiptVaucher()
{
    delete ui;
}

void DlgReceiptVaucher::setVaucher(const QString &id)
{
    ui->leVaucher->setText(id);
    fDbBind[":f_id"] = id;
    fDb.select("select m.f_room, m.f_inv, m.f_guest, m.f_amountAmd, m.f_creditCard, m.f_cityLedger, \
        m.f_fiscal, m.f_wdate, m.f_finalName, m.f_doc, m.f_remarks, m.f_paymentMode, m.f_rb, m.f_user \
        from m_register m \
        where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        ui->leInvoice->setText(fDbRows.at(0).at(1).toString());
        ui->leAmountAMD->setDouble(fDbRows.at(0).at(3).toDouble());
        ui->leAmountUSD->setDouble(ui->leAmountAMD->asDouble() / def_usd);
        ui->leCardCode->setText(fDbRows.at(0).at(4).toString());
        ui->deDate->setDate(fDbRows.at(0).at(7).toDate());
        ui->leName->setText(fDbRows.at(0).at(8).toString());
        ui->teRemarks->setPlainText(fDbRows.at(0).at(10).toString());
        // = fDbRows.at(0).at(9).toInt();
        ui->lePaymentCode->setText(fDbRows.at(0).at(11).toString());
        ui->leRoom->setText(fDbRows.at(0).at(0).toString());
        ui->rbGuest->setChecked(fDbRows.at(0).at(12).toInt());
        ui->leOpcode->setText(fDbRows.at(0).at(13).toString());
        ui->leOpName->setText(CacheUsers::instance()->get(ui->leOpcode->text())->fFull);
        if (fDbRows.at(0).at(5).toInt() > 0) {
            ui->rbCityLedger->setChecked(true);
            ui->rbGuest->setChecked(false);
            ui->lePartnerCode->setText(fDbRows.at(0).at(5).toString());
            CI_CityLedger *cl = CacheCityLedger::instance()->get(ui->lePartnerCode->text());
            if (cl) {
                ui->lePartnerName->setText(cl->fName);
            }
        } else {
            ui->rbCityLedger->setChecked(false);
            ui->rbGuest->setChecked(true);
            ui->lePartnerCode->setText(fDbRows.at(0).at(0).toString());
            ui->lePartnerName->setText(fDbRows.at(0).at(2).toString());
        }
        if (ui->rbGuest->isChecked()) {
            fDbBind[":f_id"] = ui->lePartnerCode->asInt();
            fDb.select("select concat(g.f_firstName, ' ' , g.f_lastName) from f_guests g where f_id=:f_id", fDbBind, fDbRows);
            if (fDbRows.count() > 0) {
                ui->lePartnerName->setText(fDbRows.at(0).at(0).toString());
            }
        }
    }
    fTrackControl->resetChanges();
    switch (ui->lePaymentCode->asInt()) {
        case PAYMENT_BANK:
            ui->deDate->setReadOnly(r__(cr__rv_change_date_bank));
            break;
        case PAYMENT_CASH:
            ui->deDate->setReadOnly(r__(cr__rv_change_date_cash));
            break;
        case PAYMENT_CL:
            ui->deDate->setReadOnly(r__(cr__rv_change_date_cl));
            break;
        case PAYMENT_CARD:
            ui->deDate->setReadOnly(r__(cr__rv_change_date_card));
            break;
        default:
            ui->deDate->setReadOnly(true);
            break;
    }
    CI_CreditCard *cc = CacheCreditCard::instance()->get(ui->leCardCode->text());
    if (cc) {
        ui->leCardName->setText(cc->fName);
    }
    CI_PaymentMode *cp = CachePaymentMode::instance()->get(ui->lePaymentCode->text());
    if (cp) {
        ui->lePaymentName->setText(cp->fName);
    }
    ui->btnSave->setVisible(RIGHT(WORKING_USERGROUP, cr__super_correction));
    if (r__(cr__super_correction)) {
        ui->deDate->setReadOnly(false);
    }
    ui->btnPrint->setEnabled(true);
}

void DlgReceiptVaucher::roomChanged(CI_ActiveRoom *c)
{
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->lePartnerCode, ui->lePartnerName, c);
    if (c) {
        ui->leInvoice->setText(c->fInvoice);
        ui->lePartnerCode->setText(c->fRoomCode);
        ui->lePartnerName->setText(c->fGuestName);
        ui->leRoom->setText(c->fRoomCode);
        fDbBind[":f_side"] = 0;
        fDbBind[":f_invoice"] = c->fInvoice;
        fDb.select("select sum(f_amountAmd*f_sign) from m_register where f_side=:f_side and f_inv=:f_invoice "
                   "and f_canceled=0 and f_finance=1 ", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            ui->leBalance->setText(fDbRows.at(0).at(0).toString());
        }
        fTrackControl->resetChanges();
    }
}

void DlgReceiptVaucher::cityLedger(CI_CityLedger *c)
{
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->lePartnerCode, ui->lePartnerName, c);
    ui->leRoom->clear();
    ui->leInvoice->clear();
    if (c) {
        fDbBind[":f_cityLedger"] = ui->lePartnerCode->asInt();
        fDb.select("select sum(f_amountAmd*f_sign*-1) from m_register "
                   "where f_cityLedger=:f_cityLedger and f_canceled=0 and f_finance=1 ", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            ui->leBalance->setText(fDbRows.at(0).at(0).toString());
        }
        ui->leName->setText(tr("PAYMENT ") + " " + c->fName);
    }
}

void DlgReceiptVaucher::paymentMode(CI_PaymentMode *c)
{
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentCode, ui->lePaymentName, c);
    if (c) {
        switch (c->fCode.toInt()) {
        case PAYMENT_CASH:
            cardVisible(false);
            ui->leName->setText(tr("PAYMENT CASH"));
            break;
        case PAYMENT_CARD:
            cardVisible(true);
            break;
        case PAYMENT_BANK:
            cardVisible(false);
            ui->leName->setText(tr("PAYMENT BANK"));
            break;
        default:
            cardVisible(false);
            ui->leName->setText("PAYMENT " + c->fName);
            break;
        }
    }
}

void DlgReceiptVaucher::creditCard(CI_CreditCard *c)
{
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, c);
    if (c) {
        ui->leName->setText(tr("PAYMENT") + " " + c->fName);
    }
}

void DlgReceiptVaucher::on_btnSave_clicked()
{
    QString errors;
    if (ui->leAmountAMD->asDouble() < 0.1) {
        errors += tr("Amount cannot be equal to zero.") + "<br>";
    }
    if (ui->rbGuest->isChecked()) {
        CI_ActiveRoom *c = CacheActiveRoom::instance()->get(ui->lePartnerCode->text());
        if (!c && ui->leVaucher->isEmpty()) {
            errors += tr("Incorrect room number") + "<br>";
        }
    }
    if (ui->rbCityLedger->isChecked()) {
        CI_CityLedger *c = CacheCityLedger::instance()->get(ui->lePartnerCode->text());
        if (!c) {
            errors += tr("Incorrect city ledger code") + "<br>";
        }
    }
    if (ui->lePaymentCode->asInt() == PAYMENT_CARD) {
        if (ui->leCardCode->asInt() == 0) {
            errors += tr("Card is not selected");
        }
    }
    if (!errors.isEmpty()) {
        message_error(errors);
        return;
    }
    saveRoom();
}

void DlgReceiptVaucher::on_rbGuest_clicked(bool checked)
{
    if (checked) {
        fDockCL->setSelector(0);
        fDockRoom->setSelector(ui->lePartnerCode);
        ui->leBalance->clear();
    }
}

void DlgReceiptVaucher::on_rbCityLedger_clicked(bool checked)
{
    if (checked) {
        fDockRoom->setSelector(0);
        fDockCL->setSelector(ui->lePartnerCode);
        ui->leBalance->clear();
    }
}

void DlgReceiptVaucher::on_btnCancel_clicked()
{
    reject();
}

void DlgReceiptVaucher::saveRoom()
{
    int rb = ui->rbCityLedger->isChecked();
    QString dc = "CREDIT";
    int sign = -1;
    if (ui->rbCityLedger->isChecked()) {
        dc = "DEBIT";
        sign = 1;
    }
    QString finalName;
    if (ui->rbGuest->isChecked()) {
        switch (ui->lePaymentCode->asInt()) {
        case PAYMENT_CASH:
            finalName = "CASH";
            break;
        case PAYMENT_BANK:
            finalName = "BANK";
            break;
        case PAYMENT_CARD:
            finalName = "CARD " + ui->leCardName->text();
            break;
        case PAYMENT_CL:
            finalName = "C/L " + ui->lePartnerName->text();
            break;
        case PAYMENT_BARTER:
            finalName = "BARTER " + ui->lePartnerName->text();
            break;
        default:
            message_error_tr("Selected payment mode is not allowed here");
            return;
        }
    } else {
        switch (ui->lePaymentCode->asInt()) {
        case PAYMENT_CASH:
            finalName = "CASH";
            break;
        case PAYMENT_BANK:
            finalName = "BANK";
            break;
        case PAYMENT_CARD:
            finalName = "CARD " + ui->leCardName->text();
            break;
        case PAYMENT_BARTER:
            finalName = "BARTER " + ui->lePartnerName->text();
            break;
        default:
            message_error_tr("Selected payment mode is not allowed here");
            return;
        }
    }
    bool isNew = false;
    fDb.fDb.transaction();
    if (ui->leVaucher->isEmpty()) {
        isNew = true;
        ui->leVaucher->setText(uuuid(VAUCHER_RECEIPT_N, fAirDb));
        fDb.insertId("m_register", ui->leVaucher->text());
    }
    fDbBind[":f_source"] = VAUCHER_RECEIPT_N;
    if (isNew) {
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = ui->leOpcode->asInt();
    }
    fDbBind[":f_wdate"] = ui->deDate->date();
    fDbBind[":f_room"] = (ui->rbCityLedger->isChecked() ? "-" : ui->leRoom->text());
    fDbBind[":f_guest"] = ui->lePartnerName->text();
    fDbBind[":f_itemCode"] = fPreferences.getDb(def_receip_vaucher_id);
    fDbBind[":f_finalName"] = ui->leName->text();
    fDbBind[":f_amountAmd"] = ui->leAmountAMD->asDouble();
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->lePaymentCode->asInt();
    fDbBind[":f_creditCard"] = ui->leCardCode->asInt();
    fDbBind[":f_cityLedger"] = (ui->rbCityLedger->isChecked() ? ui->lePartnerCode->asInt() : 0);
    fDbBind[":f_paymentComment"] = vaucherPaymentName(ui->lePaymentCode->asInt(),
                                                      ui->leCardCode->text(),
                                                      ui->lePartnerCode->text());
    fDbBind[":f_dc"] = dc;
    fDbBind[":f_sign"] = sign;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = ui->teRemarks->toPlainText();
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDbBind[":f_rb"] = rb;
    fDbBind[":f_cash"] = 1;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    fDb.fDb.commit();

    QString msg = isNew ? "New receipt" : "Receipt modified";
    msg += " " + ui->leVaucher->text();
    QString value = ui->deDate->text() + "/" + ui->leRoom->text()
            + "/" + ui->lePartnerName->text() + "/" + ui->lePaymentName->text()
            + "/" + ui->leCardName->text() + "/" + ui->leName->text() + "/" + ui->leAmountAMD->text();
    fTrackControl->insert(msg, value, "");

    ui->btnPrint->setEnabled(true);
    ui->btnSave->setEnabled(false);
}

void DlgReceiptVaucher::cardVisible(bool v)
{
    ui->lbCardType->setVisible(v);
    ui->leCardCode->setVisible(v);
    ui->leCardName->setVisible(v);
    if (!v) {
        ui->leCardCode->clear();
        ui->leCardName->clear();
    }
}

void DlgReceiptVaucher::on_btnPrint_clicked()
{
    PPrintVaucher::printVaucher(ui->leVaucher->text());
}

void DlgReceiptVaucher::on_leAmountAMD_textEdited(const QString &arg1)
{
    ui->leAmountUSD->setDouble(arg1.toDouble() / def_usd);
}

void DlgReceiptVaucher::on_btnNew_clicked()
{
    accept();
    DlgReceiptVaucher *d = new DlgReceiptVaucher(this);
    d->exec();
    delete d;
}
