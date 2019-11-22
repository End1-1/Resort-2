#include "dlgpostingcharges.h"
#include "ui_dlgpostingcharges.h"
#include "pprintscene.h"
#include "pprintpreview.h"
#include "ptextrect.h"
#include "pimage.h"
#include "vauchers.h"
#include "pprintvaucher.h"
#include "cacheroomarrangment.h"
#include "dwselectorpaymentmode.h"
#include "paymentmode.h"
#include "dwselectorcreditcard.h"
#include <QDir>
#include <QProcess>

DlgPostingCharges::DlgPostingCharges(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPostingCharges)
{
    ui->setupUi(this);
    ui->deDate->setDate(WORKING_DATE);
    on_rbRoom_clicked(true);
    ui->leAmount->setValidator(new QDoubleValidator(0, 999999, 2));
    ui->leTaxCash->setValidator(new QDoubleValidator(0, 999999999, 2));
    ui->leTaxCard->setValidator(new QDoubleValidator(0, 99999999, 2));
    ui->leTaxPre->setValidator(new QDoubleValidator(0, 99999999, 2));
    fDockInvoiceItem = new DWSelectorInvoiceItem(this);
    fDockInvoiceItem->fGroup = IG_POSTING_CHARGES;
    fDockInvoiceItem->configure();
    fDockInvoiceItem->setSelector(ui->leItemCode);
    connect(fDockInvoiceItem, SIGNAL(invoiceItem(CI_InvoiceItem*)), this, SLOT(invoiceItem(CI_InvoiceItem*)));
    fDockActiveRoom = new DWSelectorActiveRoom(this);
    fDockActiveRoom->configure();
    fDockActiveRoom->setSelector(ui->leRoomCode);
    connect(fDockActiveRoom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomChanged(CI_ActiveRoom*)));
    fDockCity = new DWSelectorCityLedger(this);
    fDockCity->configure();
    fDockCity->setSelector(ui->leCityCode);
    connect(fDockCity, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));
    ui->cbVAT->setIndexForData(VAT_INCLUDED);
    fTrackControl = new TrackControl(TRACK_RESERVATION);
    DWSelectorPaymentMode *dockPay = new DWSelectorPaymentMode(this);
    dockPay->configure();
    dockPay->setSelector(ui->lePayCode);
    connect(dockPay, &DWSelectorPaymentMode::paymentMode, [this](CI_PaymentMode *ci) {
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, ci);
        if (ci) {
            ui->leCardCode->setEnabled(ci->fCode.toInt() == PAYMENT_CARD);
        }
    });
    QStringList pay;
    pay << QString::number(PAYMENT_CASH)
        << QString::number(PAYMENT_CARD)
        << QString::number(PAYMENT_BANK)
        << QString::number(PAYMENT_CREDIT);
    dockPay->setFilterList(pay);
    DWSelectorCreditCard *dockCard = new DWSelectorCreditCard(this);
    dockCard->configure();
    dockCard->setSelector(ui->leCardCode);
    connect(dockCard, &DWSelectorCreditCard::creditCard, [this](CI_CreditCard *ci) {
        dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, ci);
    });
    ui->wPayment->setVisible(false);
}

DlgPostingCharges::~DlgPostingCharges()
{
    delete ui;
}

void DlgPostingCharges::loadVaucher(const QString &id)
{
    ui->leVaucherId->setText(id);
    ui->grType->setVisible(false);
    adjustSize();
    fDbBind[":f_id"] = id;
    fDb.select("select f_source, f_room, f_guest, f_cityLedger, f_amountAmd, f_amountVat, f_inv, f_itemCode, "
               "f_finalName, f_remarks, f_doc, f_side, f_wdate, f_rb, f_time, f_fiscal "
               "from m_register m "
               "where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error_tr("Invalid vaucher id. Please, contact to application developer.");
        accept();
        return;
    } else {
        int c = 0;
        ui->leVaucher->setText(fDbRows.at(0).at(c++).toString());
        ui->leRoomCode->setText(fDbRows.at(0).at(c++).toString());
        ui->leGuest->setText(fDbRows.at(0).at(c++).toString());
        ui->leCityCode->setText(fDbRows.at(0).at(c++).toString());
        ui->leAmount->setText(fDbRows.at(0).at(c++).toString());
        ui->leAmountVat->setText(fDbRows.at(0).at(c++).toString());
        ui->leInvoice->setText(fDbRows.at(0).at(c++).toString());
        ui->leItemCode->setText(fDbRows.at(0).at(c++).toString());
        ui->leItemName->setText(fDbRows.at(0).at(c++).toString());
        ui->teRemark->setPlainText(fDbRows.at(0).at(c++).toString());
        //fDoc = fDbRows.at(0).at(c++).toInt();
        c++;
        ui->chPayByComp->setChecked(fDbRows.at(0).at(c).toBool());
        ui->chPayGuest->setChecked(!fDbRows.at(0).at(c++).toBool());
        ui->deDate->setDate(fDbRows.at(0).at(c++).toDate());
        switch (fDbRows.at(0).at(c++).toInt()) {
        case 0:
            ui->rbRoom->setChecked(true);
            break;
        case 1:
            ui->rbDirect->setChecked(true);
            break;
        case 2:
            ui->rbCity->setChecked(true);
            break;
        }
        ui->teTime->setTime(fDbRows.at(0).at(c++).toTime());
        ui->leTaxCode->setText(fDbRows.at(0).at(c++).toString());

        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDb.select("select rm.f_short, concat(g.f_firstName, ' ', g.f_lastName) "
                   "from f_reservation r "
                   "left join f_room rm on rm.f_id=r.f_room "
                   "left join f_guests g on g.f_id=r.f_guest "
                   "where r.f_invoice=:f_invoice", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            ui->leRoomName->setText(fDbRows.at(0).at(0).toString());
        }
    }
    fDockActiveRoom->setSelector(0);
    fDockInvoiceItem->setSelector(0);
    ui->leItemCode->setReadOnly(!r__(cr__super_correction));
    ui->leRoomCode->setReadOnly(true);
    ui->leItemName->setReadOnly(false);
    CI_CityLedger *cl = CacheCityLedger::instance()->get(ui->leCityCode->text());
    if (cl) {
        ui->leCityName->setText(cl->fName);
    }
    ui->btnOk->setVisible(RIGHT(WORKING_USERGROUP, cr__super_correction));
    if (r__(cr__super_correction)) {
        ui->deDate->setReadOnly(false);
    }
    ui->btnPrint->setEnabled(true);
}

void DlgPostingCharges::setRoom(const QString &room)
{
    CacheActiveRoom *c = CacheActiveRoom::instance();
    CI_ActiveRoom *ci = c->get(room);
    if (ci) {
        ui->leRoomCode->setText(room);
        ui->leRoomName->setText(ci->fRoomName);
        ui->leInvoice->setText(ci->fInvoice);
        ui->leGuest->setText(ci->fGuestName);
        ui->leItemCode->setFocus();
        fTrackControl->resetChanges();
        fTrackControl->fInvoice = ci->fInvoice;
        fTrackControl->fReservation = ci->fCode;
        fDbBind[":f_reservation"] = ci->fCode;
        fDb.select("select f_itemId from f_reservation_item_side where f_reservation=:f_reservation", fDbBind, fDbRows);
        foreach_rows {
            fCompanySide.append(it->at(0).toString());
        }
        ui->rbCity->setEnabled(false);
        ui->rbDirect->setEnabled(false);
    }
}

void DlgPostingCharges::setInvoice(const QString &id)
{
    fDbBind[":f_invoice"] = id;
    DatabaseResult dr;
    dr.select(fDb, "select r.f_room, g.guest from "
              "f_reservation r "
              "left join guests g on g.f_id=r.f_guest "
              "where f_invoice=:f_invoice ", fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Cannot set invoice");
        return;
    }
    ui->leRoomCode->setText(dr.value("f_room").toString());
    ui->leRoomName->setText(dr.value("f_room").toString());
    ui->leGuest->setText(dr.value("guest").toString());
    ui->leInvoice->setText(id);
    fTrackControl->fInvoice = ui->leInvoice->text();
}

void DlgPostingCharges::setSaleItem(const QString &code, bool disableChanges)
{
    CI_InvoiceItem *c = CacheInvoiceItem::instance()->get(code);
    if (c) {
        if (c->fNoManual) {
            message_error_tr("You cannot use this name for manual charge");
            ui->leItemCode->clear();
            ui->leItemName->clear();
            return;
        }
        ui->leItemCode->setText(c->fCode);
        ui->leItemName->setText(c->fName);
        ui->leItemCode->setEnabled(!disableChanges);
    } else {
        message_error(tr("Item with code %1 not exist").arg(code));
    }
}

void DlgPostingCharges::cityLedger(CI_CityLedger *c)
{
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCityCode, ui->leCityName, c);
}

void DlgPostingCharges::invoiceItem(CI_InvoiceItem *c)
{
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leItemCode, ui->leItemName, c);
    if (c) {
        if (c->fNoManual) {
            if (!RIGHT(WORKING_USERGROUP, cr__super_correction)) {
                message_error_tr("You cannot use this name for manual charge");
                ui->leItemCode->clear();
                ui->leItemName->clear();
                return;
            }
        }
        ui->leVaucher->setText(c->fVaucher);
    }
}

void DlgPostingCharges::roomChanged(CI_ActiveRoom *c)
{
    if (c) {
        ui->leRoomCode->setText(c->fRoomCode);
        ui->leRoomName->setText(c->fRoomName);
        ui->leGuest->setText(c->fGuestName);
        ui->leInvoice->setText(c->fInvoice);
        fTrackControl->fInvoice = c->fInvoice;
        fTrackControl->fReservation = c->fCode;
    } else {
        ui->leRoomCode->clear();
        ui->leRoomName->clear();
        ui->leGuest->clear();
        ui->leInvoice->clear();
    }
}

void DlgPostingCharges::on_btnCancel_clicked()
{
    reject();
}

void DlgPostingCharges::on_btnOk_clicked()
{
    if (ui->rbDirect->isChecked()) {
        if (ui->lePayCode->asInt() == 0) {
            message_error(tr("The mode of payment not selected"));
            return;
        }
    } else {
        ui->lePayCode->setInt(PAYMENT_CREDIT);
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, 0);
    }
    if (ui->lePayCode->asInt() == 0) {
        message_error(tr("The mode of payment not selected. Contact to Administrator"));
        return;
    }
    if (ui->lePayCode->asInt() == PAYMENT_CARD) {
        if (ui->leCardCode->asInt() == 0) {
            message_error(tr("The name of credit card not selected"));
            return;
        }
    }
    QString dc = "DEBIT";
    int rb = 0;
    int sign = 1;
    if (ui->rbDirect->isChecked()) {
        rb = 1;
    }
    if (ui->rbCity->isChecked()) {
        dc = "CREDIT";
        sign = -1;
        rb = 2;
    }
    if (ui->leVaucher->text().isEmpty()) {
        message_error_tr("Voucher code is missing. Contact with administrator.");
        return;
    }
    bool isNew = false;
    QString appendix;
    fDb.fDb.transaction();
    if (ui->leVaucherId->isEmpty()) {
        isNew = true;
        QString rid = uuuid(ui->leVaucher->text(), fAirDb);
        fDb.insertId("m_register", rid);
        if (ui->leVaucher->text() == VAUCHER_ROOMING_N) {
            switch (message_yesnocancel(tr("The date will append to the name.<br>Click YES to use current working date<br>Click NO - to use previouse working date"))) {
            case RESULT_YES:
                appendix = " " + fPreferences.getDb(def_working_day).toString();
                break;
            case RESULT_NO:
                appendix = " " + WORKING_DATE.addDays(-1).toString(def_date_format);
                break;
            }
            fDbBind[":f_invoice"] = ui->leInvoice->text();
            fDb.select("select f_arrangement from f_reservation where f_invoice=:f_invoice", fDbBind, fDbRows);
            if (fDbRows.count() > 0) {
                CI_RoomArrangment *ra = CacheRoomArrangment::instance()->get(fDbRows.at(0).at(0).toString());
                if (ra) {
                    appendix += " - " + ra->fName;
                }
            }
        }
        ui->leVaucherId->setText(rid);
    }
    fDbBind[":f_source"] = ui->leVaucher->text();
    if (isNew) {
        fDbBind[":f_wdate"] = ui->deDate->date();
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
    }
    fDbBind[":f_room"] = (ui->rbRoom->isChecked() ? ui->leRoomCode->text() : "-");
    fDbBind[":f_guest"] = (ui->rbRoom->isChecked() ? ui->leGuest->text() : (ui->rbDirect->isChecked() ? "-" : ui->leCityName->text()));
    fDbBind[":f_itemCode"] = ui->leItemCode->text();
    fDbBind[":f_finalName"] = ui->leItemName->text() + appendix;
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_amountVat"] = ui->leAmountVat->asDouble();
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->lePayCode->asInt();
    fDbBind[":f_creditCard"] = ui->leCardCode->asInt();
    fDbBind[":f_cityLedger"] = ui->leCityCode->asInt();
    fDbBind[":f_paymentComment"] = vaucherPaymentName(ui->lePayCode->asInt(), ui->leCardCode->text(), ui->leCityName->text());
    fDbBind[":f_dc"] = dc;
    fDbBind[":f_sign"] = sign;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = ui->teRemark->toPlainText();
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = (int) ui->chPayByComp->isChecked();
    fDbBind[":f_rb"] = rb;
    fDbBind[":f_cash"] = ui->rbDirect->isChecked() ? 1 : 0;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucherId->text())));
    fDb.fDb.commit();

    QString msg = isNew ? "New postcharge" : "Postcharge modified";
    msg += " " + ui->leVaucher->text();
    QString value = ui->deDate->text() + "/" + ui->leRoomCode->text()
            + "/" + ui->leGuest->text() + "/" + ui->leAmount->text()
            + "/" + ui->leCityName->text() + "/" + ui->leItemName->text() + appendix
            + "/" + ui->leAmount->text();
    fTrackControl->fRecord = ui->leVaucherId->text();
    fTrackControl->insert(msg, value, "");

    if (ui->rbRoom->isChecked()) {
        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDbBind[":f_used"] = 0;
        fDbBind[":f_amount"] = ui->leAmount->asDouble();
        fDb.select("select f_id, f_amount, f_item, f_source from m_free_tax "
                   "where f_invoice=:f_invoice and f_amount=:f_amount and f_used=:f_used ", fDbBind, fDbRows);
        bool done = false;
        foreach_rows {
            if (it->at(2).toInt() == ui->leItemCode->asInt()) {
                fDbBind[":f_fiscal"] = 1;
                fDbBind[":f_usedTaxId"] = it->at(0).toInt();
                fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucherId->text())));
                fDbBind[":f_used"] = 1;
                fDb.update("m_free_tax", fDbBind, where_id(it->at(0).toInt()));
                done = true;
                break;
            }
        }
        if (!done) {
            foreach_rows {
                if (ui->leVaucher->text() != "RM") {
                    break;
                }
                if (it->at(3).toString() == ui->leVaucher->text()) {
                    fDbBind[":f_fiscal"] = 1;
                    fDbBind[":f_usedTaxId"] = it->at(0).toInt();
                    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucherId->text())));
                    fDbBind[":f_used"] = 1;
                    fDb.update("m_free_tax", fDbBind, where_id(it->at(0).toInt()));
                    done = true;
                    break;
                }
            }
        }
    }

    ui->btnOk->setEnabled(false);
    ui->btnPrint->setEnabled(true);
    ui->leItemCode->setReadOnly(true);
    ui->leAmount->setReadOnly(true);
    ui->cbVAT->setEnabled(false);
    ui->teRemark->setReadOnly(true);
}

void DlgPostingCharges::on_leRoomCode_returnPressed()
{
    CacheActiveRoom *c = CacheActiveRoom::instance();
    CI_ActiveRoom *ci = c->get(ui->leRoomCode->text());
    if (ci) {
        ui->leRoomName->setText(ci->fRoomName);
        ui->leInvoice->setText(ci->fInvoice);
        ui->leGuest->setText(ci->fGuestName);
    }
}

void DlgPostingCharges::on_leItemCode_returnPressed()
{
    CI_InvoiceItem *ci = CacheInvoiceItem::instance()->get(ui->leItemCode->text());
    if (ci) {
        ui->leItemName->setText(ci->fName);
        if (fCompanySide.contains(ci->fCode)) {
            ui->chPayByComp->setChecked(true);
            ui->chPayGuest->setChecked(false);
        } else {
            ui->chPayByComp->setChecked(false);
            ui->chPayGuest->setChecked(true);
        }
    }
}

void DlgPostingCharges::on_leAmount_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    switch (ui->cbVAT->asInt()) {
    case VAT_NOVAT:
        ui->leAmountVat->clear();
        break;
    case VAT_INCLUDED:
        ui->leAmountVat->setDouble(ui->leAmount->asDouble() - (ui->leAmount->asDouble() / ((20.0 / 100.0) + 1)));
        break;
    case VAT_WITHOUT:
        ui->leAmountVat->setDouble(ui->leAmount->asDouble() * (20.0 / 100.0));
        break;
    }
}

void DlgPostingCharges::on_cbVAT_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    on_leAmount_textEdited("");
}

void DlgPostingCharges::on_chPayGuest_clicked(bool checked)
{
    ui->chPayByComp->setChecked(!checked);
}

void DlgPostingCharges::on_chPayByComp_clicked(bool checked)
{
    ui->chPayGuest->setChecked(!checked);
}

void DlgPostingCharges::on_rbRoom_clicked(bool checked)
{
    ui->grRoom->setVisible(checked);
    ui->grCity->setVisible(!checked);
    ui->chPayGuest->setEnabled(checked);
    ui->chPayByComp->setEnabled(checked);
    disablePayment();
    adjustSize();
}

void DlgPostingCharges::on_rbDirect_clicked(bool checked)
{
    ui->grRoom->setVisible(!checked);
    ui->grCity->setVisible(!checked);
    ui->chPayGuest->setEnabled(checked);
    ui->chPayGuest->setChecked(true);
    ui->chPayByComp->setEnabled(!checked);
    enablePayment();
    adjustSize();
}

void DlgPostingCharges::on_rbCity_clicked(bool checked)
{
    ui->grRoom->setVisible(!checked);
    ui->grCity->setVisible(checked);
    ui->chPayGuest->setEnabled(!checked);
    ui->chPayByComp->setEnabled(checked);
    ui->chPayByComp->setChecked(true);
    disablePayment();
    adjustSize();
}

void DlgPostingCharges::on_btnPrint_clicked()
{
    PPrintVaucher::printVaucher(ui->leVaucherId->text());
}

void DlgPostingCharges::on_btnPrintTax_clicked()
{
//    if (ui->leVaucherId->text().isEmpty()) {
//        message_error(tr("Save first"));
//        return;
//    }
//    if (ui->leTaxCode->asInt() > 0) {
//        message_error(tr("Already printed"));
//        return;
//    }
//    CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(ui->leItemCode->text());
//    if (!ii) {
//        message_error(tr("Application error. Contact to developer. Message DlgPostCharge invoice item = 0"));
//        return;
//    }
//    DlgPrintTaxSM dpt;
//    dpt.addGoods(ii->fVatDept,
//                 ii->fAdgt,
//                 ii->fCode,
//                 ii->fTaxName,
//                 ui->leAmount->asDouble(),
//                 1);
//    dpt.fOrder = ui->leVaucherId->text();
//    dpt.fCardAmount = ui->leTaxCard->asDouble();
//    dpt.fPrepaid = ui->leTaxPre->asDouble();

//    int result = dpt.exec();
//    if (result == TAX_OK) {
//        fDbBind[":f_fiscal"] = dpt.fTaxCode;
//        fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucherId->text())));
//        ui->leTaxCode->setInt(dpt.fTaxCode);

//        fDbBind[":f_vaucher"] = ui->leVaucherId->text();
//        fDbBind[":f_invoice"] = ui->leInvoice->text();
//        fDbBind[":f_date"] = QDate::currentDate();
//        fDbBind[":f_time"] = QTime::currentTime();
//        fDbBind[":f_name"] = ui->leItemName->text();
//        fDbBind[":f_amountCash"] = ui->leTaxCash->asDouble();
//        fDbBind[":f_amountCard"] = ui->leTaxCard->asDouble();
//        fDbBind[":f_amountPrepaid"] = ui->leTaxPre->asDouble();
//        fDbBind[":f_user"] = WORKING_USERID;
//        fDbBind[":f_comp"] = HOSTNAME;
//        fDb.insert("m_tax_history", fDbBind);

//        fTrackControl->insert(TRACK_RESERVATION, "Tax printed", ui->leVaucher->text(), ui->leAmount->text());
//        return;
//    }
}

void DlgPostingCharges::on_leTaxCash_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    correctAmounts(ui->leTaxCash, ui->leTaxCard, ui->leTaxPre);
}

void DlgPostingCharges::on_leTaxCard_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    correctAmounts(ui->leTaxCard, ui->leTaxCash, ui->leTaxPre);
}

void DlgPostingCharges::on_leTaxPre_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    correctAmounts(ui->leTaxPre, ui->leTaxCash, ui->leTaxCard);
}

void DlgPostingCharges::correctAmounts(EQLineEdit *l1, EQLineEdit *l2, EQLineEdit *l3)
{
    double total = ui->leAmount->asDouble();
    if (l1->asDouble() > total) {
        l1->setDouble(total);
        l2->setDouble(0);
        l3->setDouble(0);
    }
    l2->setDouble(total - l3->asDouble() - l1->asDouble());
    if (l2->asDouble() < 0) {
        l3->setDouble(l3->asDouble() + l2->asDouble());
        l2->setDouble(0);
    }
}

void DlgPostingCharges::disablePayment()
{
    ui->wPayment->setVisible(false);
    ui->lePayCode->setInt(PAYMENT_CREDIT);
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, 0);
    ui->leCardCode->clear();
    ui->leCardName->clear();
    ui->leCardCode->setEnabled(false);
}

void DlgPostingCharges::enablePayment()
{
    ui->wPayment->setVisible(true);
    ui->lePayCode->setInt(PAYMENT_CASH);
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, 0);
    ui->leCardCode->clear();
    ui->leCardName->clear();
    ui->leCardCode->setEnabled(false);
}

void DlgPostingCharges::on_btnNew_clicked()
{
    accept();
    DlgPostingCharges *p = new DlgPostingCharges(this);
    p->exec();
    delete p;
}
