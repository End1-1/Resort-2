#include "dlgadvanceentry.h"
#include "ui_dlgadvanceentry.h"
#include "cachereservation.h"
#include "pprintpreview.h"
#include "ptextrect.h"
#include "pimage.h"
#include "vauchers.h"
#include "pprintscene.h"
#include "paymentmode.h"
#include "pprintvaucher.h"
#include "dwselectorcladvance.h"
#include "voucher_tc.h"

DlgAdvanceEntry::DlgAdvanceEntry(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_RESERVATION, parent),
    ui(new Ui::DlgAdvanceEntry)
{
    ui->setupUi(this);
    ui->leUser->setInt(WORKING_USERID);
    ui->leUser->setVisible(false);
    ui->lbOperator->setVisible(false);
    ui->lineEdit->setVisible(false ); // canceled
    ui->btnPrint->setEnabled(false);
    ui->lePaymentCode->setValidator(new QIntValidator());
    ui->leCityLedgerCode->setValidator(new QIntValidator());
    ui->leAmount->setValidator(new QDoubleValidator(0, 999999999, 2));
    ui->grCard->setVisible(false);
    ui->grCityLedger->setVisible(false);
    ui->leTCVoucher->setVisible(false);
    fDockPayment = new DWSelectorPaymentMode(this);
    fDockPayment->configure();
    fDockPayment->setSelector(ui->lePaymentCode);
    QStringList pm;
    pm << QString::number(PAYMENT_CASH)
       << QString::number(PAYMENT_CARD)
       << QString::number(PAYMENT_CL)
       << QString::number(PAYMENT_BANK);
    fDockPayment->setFilterList(pm);
    fDockReserv = new DWSelectorRedReservation(this);
    fDockReserv->configure();
    fDockReserv->setSelector(ui->leReserveId);
    fDockCard = new DWSelectorCreditCard(this);
    fDockCard->configure();
    fDockCard->setSelector(ui->leCardCode);
    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCityLedgerCode);
    connect(fDockPayment, SIGNAL(paymentMode(CI_PaymentMode*)), this, SLOT(paymentMode(CI_PaymentMode*)));
    connect(fDockReserv, SIGNAL(redReservation(CI_RedReservation*)), this, SLOT(redReservation(CI_RedReservation*)));
    connect(fDockCard, SIGNAL(creditCard(CI_CreditCard*)), this, SLOT(creditCard(CI_CreditCard*)));
    connect(fDockCL, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));
    addWidget(ui->leState, "");
    addWidget(ui->leUser, "User");
    addWidget(ui->leReserveId, "");
    addWidget(ui->deDate, "Advance date");
    addWidget(ui->lePaymentCode, "");
    addWidget(ui->lePaymentName, "Payment mode");
    addWidget(ui->leCityLedgerCode, "");
    addWidget(ui->leCityLedgerName, "City ledger");
    addWidget(ui->leCardCode, "");
    addWidget(ui->lineEdit, "");
    addWidget(ui->leCardName, "Card");
    addWidget(ui->leAmount, "Amount of advance");
    addWidget(ui->leAmountUSD, "Amount of advance, USD");
    addWidget(ui->teRemarks, "Remarks");
    fCacheId = cid_red_reservation;
    ui->lbState->setVisible(false);
    ui->leState->setVisible(false);
    adjustSize();
}

DlgAdvanceEntry::~DlgAdvanceEntry()
{
    delete ui;
}

void DlgAdvanceEntry::redReservation(CI_RedReservation *c)
{
    dockResponse<CI_RedReservation, CacheRedReservation>(ui->leReserveId, ui->leGuest, c);
    fTrackControl->fInvoice = "";
    fTrackControl->fReservation = "";
    if (c) {
        ui->leBalance->setText(c->fBalance);
        ui->leRoom->setText(c->fRoom);
        ui->leInvoice->setText(c->fInvoice);
        fReserve = c;
        fTrackControl->fInvoice = c->fInvoice;
        fTrackControl->fReservation = c->fCode;
    }
}

bool DlgAdvanceEntry::isDataCorrect()
{
    if (ui->leReserveId->asInt() == 0) {
        fDataErrors.append(tr("Incorrect reservation number"));
    }
    if (ui->leAmount->asDouble() < 0.01) {
        fDataErrors.append(tr("Incorrect amount"));
    }
    switch (ui->lePaymentCode->asInt()) {
    case 0:
        fDataErrors.append(tr("Payment mode is not correct"));
        break;
    case PAYMENT_CL:
        if (ui->leCityLedgerCode->asInt() == 0) {
            fDataErrors.append(tr("Selected city ledger is not correct"));
        }
        break;
    case PAYMENT_CARD:
        if (ui->leCardCode->asInt() == 0) {
            fDataErrors.append(tr("Select card type"));
        }
    }
    return fDataErrors.count() == 0;
}

void DlgAdvanceEntry::paymentMode(CI_PaymentMode *c)
{
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePaymentCode, ui->lePaymentName, c);
    ui->leCLbalance->setText(0);
    ui->leCityLedgerCode->clear();
    if (c) {
        ui->grCard->setVisible(false);
        ui->grCityLedger->setVisible(false);
        switch (c->fCode.toInt()) {
        case PAYMENT_CARD:
            ui->grCard->setVisible(true);
            //ui->leCardCode->customButtonClicked(true);
            break;
        case PAYMENT_CL:
            ui->grCityLedger->setVisible(true);
            //ui->leCityLedgerCode->customButtonClicked(true);
            break;
        }
        adjustSize();
    }
}

void DlgAdvanceEntry::advance(CI_RedReservation *c)
{
    QList<QVariant> values;
    DlgAdvanceEntry *d = new DlgAdvanceEntry(values, fPreferences.getDefaultParentForMessage());
    d->redReservation(c);
    d->exec();
    delete d;
}

void DlgAdvanceEntry::cityLedger(CI_CityLedger *c)
{
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCityLedgerCode, ui->leCityLedgerName, c);
    ui->chUserFromBalance->setEnabled(false);
    if (c) {
        CI_CLAdvance *ca = CacheCLAdvance::instance()->get(c->fCode);
        if (ca) {
            ui->leCLbalance->setText(ca->fAmount);
            ui->chUserFromBalance->setEnabled(ca->fAmount > 0.01);
        }
    } else {
        ui->leCLbalance->setText(0);
    }
}

void DlgAdvanceEntry::creditCard(CI_CreditCard *c)
{
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, c);
}

void DlgAdvanceEntry::on_btnCancel_clicked()
{
    reject();
}

void DlgAdvanceEntry::on_btnSave_clicked()
{
    QString finalName;
    switch (ui->lePaymentCode->asInt()) {
    case PAYMENT_CARD:
        finalName = "CC/" + ui->leCardName->text();
        break;
    case PAYMENT_CASH:
        finalName = "CASH";
        break;
    case PAYMENT_CL:
        finalName = "CL/" + ui->leCityLedgerName->text();
        if (ui->chUserFromBalance->isChecked()) {
            if (ui->leAmount->asDouble() > ui->leCLbalance->asDouble()) {
                message_error(tr("Not enough money on the C/L balance"));
                return;
            }
        }
        break;
    case PAYMENT_BANK:
        finalName = "BANK";
        break;
    default:
        message_error_tr("Selected mode of payment is not allowed here");
        return;
    }
    //finalName += " " + WORKING_DATE.toString(def_date_format);

    bool isNew = false;
    fDb.fDb.transaction();
    if (ui->leVaucher->isEmpty()) {
        isNew = true;
        ui->leVaucher->setText(uuuid(VAUCHER_ADVANCE_N, fAirDb));
        fDb.insertId("m_register", ui->leVaucher->text());
    }
    fDbBind[":f_source"] = VAUCHER_ADVANCE_N;
    if (isNew) {
        fDbBind[":f_wdate"] = WORKING_DATE;
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
    fDbBind[":f_cityLedger"] = ui->leCityLedgerCode->asInt();
    fDbBind[":f_paymentComment"] = vaucherPaymentName(ui->lePaymentCode->asInt(), ui->leCardCode->text(), ui->leCityLedgerCode->text());
    fDbBind[":f_dc"] = "CREDIT";
    fDbBind[":f_sign"] = -1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_res"] = ui->leReserveId->text();
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = ui->teRemarks->toPlainText();
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDbBind[":f_cash"] = ui->chUserFromBalance->isChecked() ? 0 : 1;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    fDb.fDb.commit();
    ui->btnSave->setEnabled(false);
    ui->btnPrint->setEnabled(true);
    ui->leReserveId->setEnabled(false);
    BroadcastThread::cmdRefreshCache(cid_red_reservation, ui->leReserveId->text());

    if (isNew) {
        if (ui->chUserFromBalance->isChecked()) {
            voucher_tc tc;
            QString tcId = tc.genId("TC");
            tc.set("f_id", tcId);
            tc.set("f_res", ui->leReserveId->text())
                .set("f_wdate", WORKING_DATE)
                .set("f_rdate", QDate::currentDate())
                .set("f_time", QTime::currentTime())
                .set("f_user", WORKING_USERID)
                .set("f_room", ui->leRoom->text())
                .set("f_guest", ui->leGuest->text())
                .set("f_itemCode", VAUCHER_TRANSFER_CL)
                .set("f_finalName", finalName)
                .set("f_amountAmd", ui->leAmount->asDouble())
                .set("f_usedPrepaid", 0)
                .set("f_amountVat", 0)
                .set("f_amountUsd", def_usd)
                .set("f_fiscal", 0)
                .set("f_paymentMode", 0)
                .set("f_creditCard", 0)
                .set("f_cityLedger", ui->leCityLedgerCode->asInt())
                .set("f_paymentComment", "")
                .set("f_dc", "CREDIT")
                .set("f_sign", -1)
                .set("f_doc", ui->leVaucher->text())
                .set("f_inv", ui->leVaucher->text())
                .set("f_finance", 1)
                .set("f_remarks", "")
                .set("f_side", 1)
                .set("f_canceled", 0)
                .set("f_cancelReason", "")
                .set("f_cancelDate", QDate::currentDate())
                .set("f_cancelUser", 0)
                .set("p", 0)
                .set("f_usedTaxId", 0)
                .set("f_rb", 0)
                .set("f_cash", 0);
            tc.saveVoucher();

            fDbBind[":f_doc"] = tcId;
            fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
        }
    }


    QString msg = isNew ? "New advance" : "Advance modified";
    msg += " " + ui->leVaucher->text();
    QString v2;
    if (ui->chUserFromBalance->isChecked()) {
        v2 += " [Transfer from C/L " + ui->leCityLedgerCode->text() + "]";
    }
    QString value = ui->deDate->text() + "/" + ui->leRoom->text()
            + "/" + ui->leGuest->text() + "/" + ui->lePaymentName->text()
            + "/" + ui->leCardName->text() + "/" + finalName
            + "/" + float_printout(ui->leAmount->asDouble());
    fTrackControl->insert(msg, value, v2);
}

void DlgAdvanceEntry::on_leAmount_textChanged(const QString &arg1)
{
    ui->leAmountUSD->setDouble(arg1.toDouble() / def_usd);
}

void DlgAdvanceEntry::on_btnPrint_clicked()
{
   PPrintVaucher::printVaucher(ui->leVaucher->text());
}

void DlgAdvanceEntry::on_btnNew_clicked()
{
    accept();
    QList<QVariant> v;
    DlgAdvanceEntry *d = new DlgAdvanceEntry(v, fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}

void DlgAdvanceEntry::on_btnPrintTax_clicked()
{
    if (ui->leVaucher->text().isEmpty()) {
        message_error(tr("Save first"));
        return;
    }
    if (ui->leTax->asInt() > 0) {
        message_error(tr("Already printed"));
        return;
    }

    double cash = ui->lePaymentCode->asInt() == PAYMENT_CASH ? ui->leAmount->asDouble() : 0;
    double card = ui->lePaymentCode->asInt() == PAYMENT_CARD ? ui->leAmount->asDouble() : 0;
    int taxCode;
//    if (!DlgPrintTaxSM::printAdvance(cash, card, ui->leVaucher->text(), taxCode)) {
//        return;
//    }
    //PrintTax::printAdvance(ui->leAmount->asDouble(), ui->lePaymentCode->asInt(), QString("AV%1").arg(ui->leVaucher->text()));
    ui->leTax->setInt(taxCode);
    fDbBind[":f_vaucher"] = ui->leVaucher->text();
    fDbBind[":f_invoice"] = "";
    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_name"] = tr("ADVANCE") + " " + ui->lePaymentName->text() + " " + ui->leCardName->text() + ui->leCityLedgerName->text();
    fDbBind[":f_amountCash"] = ui->lePaymentCode->asInt() == PAYMENT_CASH ? ui->leAmount->asDouble() : 0;
    fDbBind[":f_amountCard"] = ui->lePaymentCode->asInt() == PAYMENT_CASH ? 0 : ui->leAmount->asDouble();
    fDbBind[":f_amountPrepaid"] = 0;
    fDbBind[":f_user"] = WORKING_USERID;
    fDbBind[":f_comp"] = HOSTNAME;
    fDb.insert("m_tax_history", fDbBind);

    fTrackControl->insert("Tax printed", ui->leVaucher->text(), ui->leAmount->text());
}
