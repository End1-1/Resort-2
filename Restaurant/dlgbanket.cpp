#include "dlgbanket.h"
#include "ui_dlgbanket.h"
#include "rnumbers.h"
#include "rpaymenttype.h"
#include "dlgreservation.h"
#include "cachecityledger.h"
#include "cacheinvoiceitem.h"
#include "printtax.h"
#include "dlgbanketitems.h"
#include "rmessage.h"
#include "cacheusers.h"
#include "vauchers.h"
#include "rchangehall.h"
#include "hall.h"
#include "user.h"
#include "pprintscene.h"
#include "dlggettext.h"
#include "ptextrect.h"
#include "cachepaymentmode.h"
#include "printtaxn.h"
#include "pimage.h"
#include "paymentmode.h"
#include "cachetaxmap.h"
#include "dlglist.h"
#include <QPrinter>
#include <QPainter>

DlgBanket::DlgBanket(User *user, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgBanket)
{
    ui->setupUi(this);
    fFiscal = 0;
    fUser = user;
    fTrackControl = new TrackControl(TRACK_REST_ORDER);
    ui->deDate->setDate(WORKING_DATE);
    showFullScreen();
}

DlgBanket::~DlgBanket()
{
    delete ui;
    delete fUser;
}

void DlgBanket::printReceipt(const QString &num)
{
    if (num.isEmpty()) {
        if (message_yesnocancel(tr("Document not saved"))) {
            return;
        }
    }

    fDbBind[":f_id"] = num;
    DatabaseResult dr;
    dr.select(fDb, "select e.f_state, e.f_comment, e.f_pax, e.f_price, e.f_prepayment, e.f_total, e.f_paymentDetails, "
              "h.f_name as hallname, pm.f_en as paymentname, e.f_wdate "
              "from o_event e "
              "left join r_hall h on h.f_id=e.f_hall "
              "left join f_payment_type pm on pm.f_id=e.f_paymentMode "
              "where e.f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error(tr("Invalid number"));
        return;
    }

    QString state = dr.value("f_state").toInt() != 1 ? tr("DRAFT") : tr("CLOSED");

    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    QPen boldPen(Qt::SolidLine);
    boldPen.setWidth(4);
    PTextRect th;
    QFont f("Arial", 30);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(150, top, 400, 250));
    top += 250;
    f.setPointSize(40);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight + 10, dr.value("hallname").toString(), &th, f))->textHeight();
    top += 20;
    f.setPointSize(28);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("RECEIPT S/N "))
                                     .arg(num),
                                     &th, f))->textHeight();
    if (state.contains(tr("DRAFT"))) {
        top += ps->addTextRect(10, top, 680, rowHeight, tr("(DRAFT)"), &th)->textHeight();
    }
    top += ps->addTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Date")).arg(dr.value("f_wdate").toDate().toString(def_date_time_format)), &th)->textHeight();
    ps->addLine(10, top, 680, top, boldPen);
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("NAME OF SERVICE:"), &th)->textHeight() + 10;
    f.setPointSize(40);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight + 20, dr.value("f_comment").toString(), &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("PAX:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, dr.value("f_pax").toString(), &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("PRICE PER PERSON:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(dr.value("f_price").toDouble(), 2) + " AMD", &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    if (dr.value("f_prepayment").toDouble() > 0.1) {
        top += ps->addTextRect(10, top, 680, rowHeight, tr("PREPAYMENT: ") + dr.value("f_prepayment").toString(), &th)->textHeight();
    }
    top += ps->addTextRect(10, top, 680, rowHeight, tr("TOTAL AMOUNT:"), &th)->textHeight() + 10;
    f.setPointSize(40);
    f.setBold(true);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(dr.value("f_total").toDouble(), 2) + " AMD", &th)->textHeight() + 10;
    f.setPointSize(24);
    f.setBold(false);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("EQUIVALENT OF USD:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(dr.value("f_price").toDouble() / def_usd, 2) + " USD", &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("MODE OF PAYMENT:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, dr.value("paymentname").toString() + "/" + dr.value("f_paymentDetails").toString(), &th)->textHeight() + 10;
    top += rowHeight;
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Guest signature"), &th)->textHeight();
    top += (rowHeight * 2);
    ps->addLine(10, top, 680, top, boldPen);
    top += rowHeight;
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Manager signature"), &th)->textHeight();
    top += (rowHeight * 2);
    ps->addLine(10, top, 680, top, boldPen);
    top += rowHeight;


    //Finish
    top += rowHeight;
    ps->addTextRect(new PTextRect(10, top, 680, rowHeight, "_", &th, f));

    QPrinter printer;
    //printer.setPrinterName(fHall->fReceiptPrinter);
    QMatrix m;
    m.scale(3, 3);
    QPainter painter(&printer);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }

    fTrackControl->insert("Print event receipt", "", "");
    fTrackControl->saveChanges();
}

void DlgBanket::on_btnExit_clicked()
{
    if (fDoc > 0) {
        if (message_question(tr("Confirm to exit")) != QDialog::Accepted) {
            return;
        }
    }
    accept();
}

void DlgBanket::on_btnGuestQty_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        ui->leGuests->setInt(num);
        ui->leTotal->setDouble(ui->lePrice->asDouble() * ui->leGuests->asInt());
        ui->leNeedToPay->setDouble(ui->leTotal->asDouble() - ui->lePrepayment->asDouble());
    }
}

void DlgBanket::on_btnPrice_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        ui->lePrice->setInt(num);
        ui->leTotal->setDouble(ui->lePrice->asDouble() * ui->leGuests->asInt());
        ui->leNeedToPay->setDouble(ui->leTotal->asDouble() - ui->lePrepayment->asDouble());
    }
}

void DlgBanket::on_btnPayment_clicked()
{
    int cardCode = 0;
    QString cardName;
    int cityLedger = 0;
    QString cityLedgerName;
    int paymentMode = 0;
    switch (RPaymentType::getPaymentMode(this, cardCode)) {
    case -1:
        return;
    case 0:
        paymentMode = PAYMENT_CASH;
        cardName = "CASH";
        break;
    case 1:
        paymentMode = PAYMENT_CARD;
        cardName = "VISA";
        break;
    case 2:
        paymentMode = PAYMENT_CARD;
        cardName = "MAESTRO";
        break;
    case 3:
        paymentMode = PAYMENT_CARD;
        cardName = "MASTERCARD";
        break;
    case 4:
        paymentMode = PAYMENT_CARD;
        cardName = "ARCA";
        break;
    case 5:
        paymentMode = PAYMENT_CARD;
        cardName = "AMEX";
        break;
/*
    case 6: {
        DlgReservation *d = new DlgReservation(this);
        d->loadRoom();
        if (d->exec() == QDialog::Accepted) {
            fDbBind[":f_reservation"] = d->fReservationId;
            fDb.update("o_header", fDbBind, where_id(fTable->fOrder));
            fDbBind[":f_id"] = d->fReservationId;
            fDb.select("select f_invoice, f_room, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName) "
                       "from f_reservation r "
                       "inner join f_guests g on g.f_id=r.f_guest "
                       "where r.f_id=:f_id", fDbBind, fDbRows);
            invoice = fDbRows.at(0).at(0).toInt();
            room = fDbRows.at(0).at(1).toString();
            guest = fDbRows.at(0).at(2).toString();
            cardName = QString("%1, %2").arg(room).arg(guest);
        } else {
            delete d;
            return;
        }
        cardCode = 0;
        paymentMode = PAYMENT_ROOM;
        cardName = paymentModeComment;
        break;
    } */
    case 7: {
        DlgReservation *d = new DlgReservation(this);
        d->loadCL();
        if (d->exec() == QDialog::Accepted) {
            cityLedger = d->fReservationId.toInt();
            cityLedgerName = CacheCityLedger::instance()->get(d->fReservationId)->fName;
            paymentMode = PAYMENT_CL;
            cardName = cityLedgerName;
            cardCode = cityLedger;
        } else {
            delete d;
            return;
        }
        break;
    }
    default:
        message_error(tr("Payment mode not supported"));
        paymentMode = 0;
        cardName = "";
        break;
    }
    CI_PaymentMode *i = CachePaymentMode::instance()->get(QString::number(paymentMode));
    if (i) {
        ui->leModeOfPayment->fHiddenText = QString::number(paymentMode);
        ui->leModeOfPayment->setText(i->fName);
        ui->lePaymentComment->setText(cardName);
        ui->lePaymentComment->fHiddenText = QString::number(cardCode);
    } else {
        ui->leModeOfPayment->clear();
        ui->leModeOfPayment->fHiddenText = "";
        ui->lePaymentComment->clear();
    }
}

void DlgBanket::printTax()
{
    //Print tax
    if (fPreferences.getDb(def_tax_port).toInt() == 0) {
        message_error(tr("Setup tax printer first"));
        return;
    }

    HallStruct *h = 0;
    for (int i = 0; i < Hall::fBanketHall.count(); i++) {
        if (Hall::fBanketHall.at(i)->fId == ui->leHall->fHiddenText.toInt()) {
            h = Hall::fBanketHall.at(i);
            break;
        }
    }

    if (!h) {
        message_error(tr("No hall selected"));
        return;
    }

    CI_InvoiceItem *i = CacheInvoiceItem::instance()->get(ui->leService->fHiddenText);
    if (!i) {
        message_error(tr("No invoice item defined for event"));
        return;
    }
    CI_TaxMap *tm = CacheTaxMap::instance()->get(i->fCode);
    if (!tm) {
        message_error(tr("No tax department defined for ") + i->fName);
        return;
    }

   //b double total = ui->lePrice->asDouble();
//    pt->fRecList.append(i->fCode);
//    pt->fDept.append(h->fVatDept);
//    pt->fAdgCode.append(i->fAdgt);
//    pt->fCodeList.append(i->fCode);
//    pt->fNameList.append(i->fName);
//    pt->fQtyList.append("1");
//    pt->fPriceList.append(ui->lePrice->text());
//    pt->fTaxNameList.append(i->fTaxName);

    PrintTaxN ptn(fPreferences.getDb(def_tax_address).toString(),
                  fPreferences.getDb(def_tax_port).toInt(),
                  fPreferences.getDb(def_tax_password).toString());
    ptn.addGoods(h->fVatDept, i->fAdgt, i->fCode, i->fTaxName, ui->lePrice->asDouble() + ui->lePrepayment->asDouble(), 1);

    switch (ui->leModeOfPayment->fHiddenText.toInt()) {
    case PAYMENT_CASH:
//        pt->fAmountCash = float_str(total, 2);
//        pt->fAmountCard = "0";
        break;
    case PAYMENT_CARD:/*
        pt->fAmountCard = float_str(total, 2);
        pt->fAmountCash = "0";*/
        break;
    default:
        message_error(tr("Printing fiscal receipt is not available for selected payment mode"));
        return;
    }
    QString in, out, err;
    int result = ptn.makeJsonAndPrint(ui->leModeOfPayment->fHiddenText.toInt() == PAYMENT_CARD ? ui->lePrice->asDouble() : 0, 0, in, out, err);
    if (result != pt_err_ok) {
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_db"] = 1;
        fDbBind[":f_order"] = fDoc;
        fDbBind[":f_queryCode"] = opcode_PrintTaxN;
        fDbBind[":f_queryJson"] = in;
        fDbBind[":f_replyJson"] = out;
        fDbBind[":f_replyResult"] = result;
        fDbBind[":f_replyJson"] = out;
        fDbBind[":f_replyText"] = err;
        fDbBind[":f_replyTaxCode"] = "-";
        fAirDb.open();
        fAirDb.insert("tax_print", fDbBind);
        fAirDb.close();
        message_error(out + QString("<br>") + err);
        return;
    }
    fDbBind[":f_date"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_db"] = 1;
    fDbBind[":f_order"] = fDoc;
    fDbBind[":f_queryCode"] = opcode_PrintTaxN;
    fDbBind[":f_queryJson"] = in;
    fDbBind[":f_replyJson"] = out;
    fDbBind[":f_replyResult"] = result;
    fDbBind[":f_replyJson"] = out;
    fDbBind[":f_replyText"] = err;
    fDbBind[":f_replyTaxCode"] = 1;
    fAirDb.open();
    fAirDb.insert("tax_print", fDbBind);
    fAirDb.close();

    fDbBind[":f_tax"] = 1;
    fDb.update("o_event", fDbBind, where_id(ap(fDoc)));
    fDbBind[":f_fiscal"] = 1;
    fDb.update("m_register", fDbBind, where_id(ap(fDoc)));
}

bool DlgBanket::isCorrect()
{
    if (ui->leService->fHiddenText.toInt() == 0) {
        message_error(tr("Service was not selected"));
        return false;
    }
    if (ui->leGuests->asInt() < 1) {
        message_error(tr("Incorrect guest count"));
        return false;
    }
    if (ui->lePrice->asDouble() < 1) {
        message_error(tr("Incorrect price"));
        return false;
    }
    if (ui->leModeOfPayment->fHiddenText.toInt() == 0) {
        message_error(tr("Mode of payment was not selected"));
        return false;
    }
    if (ui->leHall->fHiddenText.toInt() == 0) {
        message_error(tr("Hall was not selected"));
        return false;
    }

    if (message_question(tr("Really save?")) != QDialog::Accepted) {
        return false;
    }
    return true;
}

void DlgBanket::on_btnSelectService_clicked()
{
    int code;
    QString name;
    if (DlgBanketItems::getItem(code, name)) {
        ui->leService->setText(name);
        ui->leService->fHiddenText = QString::number(code);
    }
}

void DlgBanket::on_btnPrintReceipt_clicked()
{
    printReceipt(fDoc);
}

void DlgBanket::on_btnSave_clicked()
{
    if (!isCorrect()) {
        return;
    }

    int creditCard = 0;
    int cityLedger = 0;
    switch (ui->leModeOfPayment->fHiddenText.toInt()) {
    case PAYMENT_CARD:
        creditCard = ui->lePaymentComment->fHiddenText.toInt();
        break;
    case PAYMENT_CL:
        cityLedger = ui->lePaymentComment->fHiddenText.toInt();
        break;
    }

    fDb.fDb.transaction();

    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_state"] = 1;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = fUser->fId;
    fDbBind[":f_itemCode"] = ui->leService->fHiddenText.toInt();
    fDbBind[":f_comment"] = ui->leComment->text();
    fDbBind[":f_hall"] = ui->leHall->fHiddenText.toInt();
    fDbBind[":f_pax"] = ui->leGuests->asInt();
    fDbBind[":f_price"] = ui->lePrice->asDouble();
    fDbBind[":f_total"] = ui->leTotal->asDouble();
    fDbBind[":f_tax"] = 0;
    fDbBind[":f_paymentMode"] = ui->leModeOfPayment->fHiddenText;
    fDbBind[":f_creditCard"] = creditCard;
    fDbBind[":f_cityLedger"] = cityLedger;
    fDbBind[":f_paymentDetails"] = ui->lePaymentComment->text();
    if (fDoc.isEmpty()) {
        fDoc = uuid(VAUCHER_EVENT_N, fAirDb);
        fDb.insertId("o_event", fDoc);
    }
    fDb.insertId("m_register", fDoc);
    fDb.update("o_event", fDbBind, where_id(ap(fDoc)));

    fDbBind[":f_source"] = VAUCHER_EVENT_N;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = fUser->fId;
    fDbBind[":f_room"] = "-";
    fDbBind[":f_guest"] = "-";
    fDbBind[":f_itemCode"] = ui->leService->fHiddenText.toInt();
    fDbBind[":f_finalName"] = QString("%1 %2 #%3").arg(ui->leHall->text()).arg(ui->leService->text()).arg(fDoc);
    fDbBind[":f_amountAmd"] = ui->lePrice->asDouble();
    fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->lePrice->asDouble(), 1);
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = fFiscal;
    fDbBind[":f_paymentMode"] = ui->leModeOfPayment->fHiddenText.toInt();
    fDbBind[":f_creditCard"] = creditCard;
    fDbBind[":f_cityLedger"] = cityLedger;
    fDbBind[":f_paymentComment"] = ui->lePaymentComment->text();
    fDbBind[":f_dc"] = (cityLedger == 0 ? "DEBIT" : "CREDIT");
    fDbBind[":f_sign"] = (cityLedger == 0 ? 1 : -1);
    fDbBind[":f_doc"] = fDoc;
    fDbBind[":f_rec"] = 0;
    fDbBind[":f_inv"] = 0;
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = "";
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(fDoc)));

    fTrackControl->insert("Save event", "", "");
    fTrackControl->saveChanges();

    fDb.fDb.commit();

    ui->btnSelectService->setEnabled(false);
    ui->btnGuestQty->setEnabled(false);
    ui->btnPrice->setEnabled(false);
    ui->btnPayment->setEnabled(false);
    ui->btnHall->setEnabled(false);
    ui->btnSave->setEnabled(false);
    ui->btnDraft->setEnabled(false);

    ui->lbState->setText(QString("%1 %2").arg("EVENT S/N ").arg(fDoc));
}

void DlgBanket::on_btnHall_clicked()
{
    RChangeHall *h = new RChangeHall(this);
    h->setup(Hall::fBanketHall);
    if (h->exec() == QDialog::Accepted)  {
        HallStruct *hs = 0;
        for (int i = 0; i < Hall::fBanketHall.count(); i++) {
            if (h->hall() == Hall::fBanketHall.at(i)->fId) {
                hs = Hall::fBanketHall.at(i);
                break;
            }
        }
        if (hs) {
            ui->leHall->setText(hs->fName);
            ui->leHall->fHiddenText = QString::number(hs->fId);
            if (!hs->fId) {
                message_error(QString::number(hs->fId));
            }
        } else {
            message_error(tr("Cannot find hall") + "<br>" + QString::number(h->hall()));
        }
    }
    delete h;
}

void DlgBanket::on_btnTaxPrint_clicked()
{
    if (fDoc.isEmpty()) {
        message_error(tr("Document not saved"));
        return;
    }
    printTax();

    fTrackControl->insert("Print event fiscal", "", "");
    fTrackControl->saveChanges();
}

void DlgBanket::on_btnComment_clicked()
{
    QString comment;
    QString sql = "select f_id, f_name from r_banquet_comment";
    QVariant result;
    if (DlgList::getValue(tr("COMMENT"), comment, result, sql)) {
        ui->leComment->setText(comment);
    }
}

void DlgBanket::on_btnDraft_clicked()
{
    if (!isCorrect()) {
        return;
    }

    int creditCard = 0;
    int cityLedger = 0;
    switch (ui->leModeOfPayment->fHiddenText.toInt()) {
    case PAYMENT_CARD:
        creditCard = ui->lePaymentComment->fHiddenText.toInt();
        break;
    case PAYMENT_CL:
        cityLedger = ui->lePaymentComment->fHiddenText.toInt();
        break;
    }

    fDbBind[":f_wdate"] = ui->deDate->date();
    fDbBind[":f_state"] = 0;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = fUser->fId;
    fDbBind[":f_itemCode"] = ui->leService->fHiddenText.toInt();
    fDbBind[":f_comment"] = ui->leComment->text();
    fDbBind[":f_hall"] = ui->leHall->fHiddenText.toInt();
    fDbBind[":f_pax"] = ui->leGuests->asInt();
    fDbBind[":f_price"] = ui->lePrice->asDouble();
    fDbBind[":f_total"] = ui->leTotal->asDouble();
    fDbBind[":f_tax"] = 0;
    fDbBind[":f_paymentMode"] = ui->leModeOfPayment->fHiddenText;
    fDbBind[":f_creditCard"] = creditCard;
    fDbBind[":f_cityLedger"] = cityLedger;
    fDbBind[":f_prepayment"] = ui->lePrepayment->asDouble();
    fDbBind[":f_prepaymentMode"] = ui->leModePrepayment->fHiddenText.toInt();
    fDbBind[":f_prepaymentModeComment"] = ui->leModePrepaymentComment->text();
    fDbBind[":f_prepaymentTax"] = ui->lePrepaymentTax->text();
    if (fDoc.isEmpty()) {
        fDoc = uuid(VAUCHER_EVENT_N, fAirDb);
        fDb.insertId("o_event", fDoc);
    }
    fDb.update("o_event", fDbBind, where_id(ap(fDoc)));

    fTrackControl->insert("Save draft", "", "");
    fTrackControl->saveChanges();

    ui->lbState->setText(QString("%1 #%2").arg(tr("DRAFT")).arg(fDoc));

}

void DlgBanket::on_btnOpen_clicked()
{
    QString name;
    QString sql = "select f_id, concat(date_format(f_wdate, '%d/%m/%Y'), ' ', f_comment, ' ', f_total) "
            "from o_event where f_state=0 and f_canceled=0";
    QVariant result;
    if (!DlgList::getValue(tr("DRAFTS"), name, result, sql)) {
        return;
    }
    fDbBind[":f_id"] = result;
    fDb.select("select f_itemCode, f_comment, f_hall, f_pax, f_total, f_paymentMode, f_creditCard, f_cityLedger, "
               "f_prepayment "
               "from o_event where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        message_error(tr("Cannot load event"));
        return;
    }
    fDbBind[":f_id"] = result;
    DatabaseResult dre;
    dre.select(fDb, "select * from o_event where f_id=:f_id", fDbBind);
    if (dre.rowCount() == 0) {
        message_error(tr("Cannot load event"));
        return;
    }
    const QList<QVariant> &row = fDbRows.at(0);
    CI_InvoiceItem *ci = CacheInvoiceItem::instance()->get(row.at(0).toString());
    if (ci) {
        ui->leService->setText(ci->fName);
        ui->leService->fHiddenText = ci->fCode;
    }
    ui->leComment->setText(row.at(1).toString());
    HallStruct *hall = Hall::fHallMap[row.at(2).toInt()];
    if (hall) {
        ui->leHall->setText(hall->fName);
        ui->leHall->fHiddenText = QString::number(hall->fId);
    }
    ui->leGuests->setText(row.at(3).toString());
    ui->leTotal->setDouble(row.at(4).toDouble());
    if (ui->leGuests->asInt()) {
        ui->lePrice->setDouble(ui->leTotal->asDouble() / ui->leGuests->asInt());
    }
    CI_PaymentMode *pc = CachePaymentMode::instance()->get(row.at(5).toString());
    if (pc) {
        ui->leModeOfPayment->setText(pc->fName);
        ui->leModeOfPayment->fHiddenText = pc->fCode;
    }
    CI_CreditCard *cc = CacheCreditCard::instance()->get(row.at(6).toString());
    if (cc) {
        ui->lePaymentComment->setText(cc->fName);
        ui->lePaymentComment->fHiddenText = cc->fCode;
    }
    CI_CityLedger *cl = CacheCityLedger::instance()->get(row.at(7).toString());
    if (cl) {
        ui->lePaymentComment->setText(cl->fName);
        ui->lePaymentComment->fHiddenText = cl->fCode;
    }
    ui->lePrepayment->setText(float_str(row.at(8).toDouble(), 0));
    ui->lbState->setText(QString("%1 #%2").arg(tr("DRAFT")).arg(result.toString()));
    fDoc = result.toString();

    CI_PaymentMode *pprep = CachePaymentMode::instance()->get(dre.value("f_prepaymentMode").toString());
    if (pprep) {
        ui->leModePrepayment->setText(pprep->fName);
        ui->leModePrepayment->fHiddenText = pprep->fCode;
    }
    ui->lePaymentComment->setText(dre.value("f_prepaymentModeComment").toString());
    ui->lePrepaymentTax->setText(dre.value("f_prepaymentTax").toString());
    ui->leTotal->setDouble(ui->lePrice->asDouble() * ui->leGuests->asInt());
    ui->leNeedToPay->setDouble(ui->leTotal->asDouble() - ui->lePrepayment->asDouble());
    ui->deDate->setDate(dre.value("f_wdate").toDate());

    ui->btnDraft->setEnabled(true);
    ui->btnSave->setEnabled(true);
    ui->pushButton_2->setEnabled(ui->lePrepaymentTax->asInt() == 0);
    ui->deDate->setEnabled(false);

    fTrackControl->insert("Open draft", "", "");
    fTrackControl->saveChanges();
}

void DlgBanket::on_btnNew_clicked()
{
    ui->leService->fHiddenText.clear();
    ui->leService->clear();
    ui->leComment->clear();
    ui->leGuests->clear();
    ui->lePrepayment->clear();
    ui->lePrice->clear();
    ui->leModeOfPayment->clear();
    ui->leModeOfPayment->fHiddenText.clear();
    ui->lePaymentComment->clear();
    ui->lePaymentComment->fHiddenText.clear();
    ui->leHall->clear();
    ui->leHall->fHiddenText.clear();
    fDoc = "";
    ui->btnDraft->setEnabled(true);
}

void DlgBanket::on_btnPrepayment_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        ui->lePrepayment->setInt(num);
        ui->leNeedToPay->setDouble(ui->leTotal->asDouble() - ui->lePrepayment->asDouble());
    }
}

void DlgBanket::on_btnPrintDuplicate_clicked()
{
    QString ordNum;
    if (!DlgGetText::getText(ordNum, "PE-")) {
        return;
    }
    if (ordNum.trimmed().isEmpty()) {
        return;
    }
    printReceipt(ordNum);
}

void DlgBanket::on_pushButton_clicked()
{
    int cardCode = 0;
    QString cardName;
    int cityLedger = 0;
    QString cityLedgerName;
    int paymentMode = 0;
    switch (RPaymentType::getPaymentMode(this, cardCode)) {
    case -1:
        return;
    case 0:
        paymentMode = PAYMENT_CASH;
        cardName = "CASH";
        break;
    case 1:
        paymentMode = PAYMENT_CARD;
        cardName = "VISA";
        break;
    case 2:
        paymentMode = PAYMENT_CARD;
        cardName = "MAESTRO";
        break;
    case 3:
        paymentMode = PAYMENT_CARD;
        cardName = "MASTERCARD";
        break;
    case 4:
        paymentMode = PAYMENT_CARD;
        cardName = "ARCA";
        break;
    case 5:
        paymentMode = PAYMENT_CARD;
        cardName = "AMEX";
        break;
/*
    case 6: {
        DlgReservation *d = new DlgReservation(this);
        d->loadRoom();
        if (d->exec() == QDialog::Accepted) {
            fDbBind[":f_reservation"] = d->fReservationId;
            fDb.update("o_header", fDbBind, where_id(fTable->fOrder));
            fDbBind[":f_id"] = d->fReservationId;
            fDb.select("select f_invoice, f_room, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName) "
                       "from f_reservation r "
                       "inner join f_guests g on g.f_id=r.f_guest "
                       "where r.f_id=:f_id", fDbBind, fDbRows);
            invoice = fDbRows.at(0).at(0).toInt();
            room = fDbRows.at(0).at(1).toString();
            guest = fDbRows.at(0).at(2).toString();
            cardName = QString("%1, %2").arg(room).arg(guest);
        } else {
            delete d;
            return;
        }
        cardCode = 0;
        paymentMode = PAYMENT_ROOM;
        cardName = paymentModeComment;
        break;
    } */
    case 7: {
        DlgReservation *d = new DlgReservation(this);
        d->loadCL();
        if (d->exec() == QDialog::Accepted) {
            cityLedger = d->fReservationId.toInt();
            cityLedgerName = CacheCityLedger::instance()->get(d->fReservationId)->fName;
            paymentMode = PAYMENT_CL;
            cardName = cityLedgerName;
            cardCode = cityLedger;
        } else {
            delete d;
            return;
        }
        break;
    }
    default:
        message_error(tr("Payment mode not supported"));
        paymentMode = 0;
        cardName = "";
        break;
    }
    CI_PaymentMode *i = CachePaymentMode::instance()->get(QString::number(paymentMode));
    if (i) {
        ui->leModePrepayment->fHiddenText = QString::number(paymentMode);
        ui->leModePrepayment->setText(i->fName);
        ui->leModePrepaymentComment->setText(cardName);
        ui->leModePrepaymentComment->fHiddenText = QString::number(cardCode);
    } else {
        ui->leModePrepayment->clear();
        ui->leModePrepayment->fHiddenText = "";
        ui->leModePrepaymentComment->clear();
    }
}

void DlgBanket::on_pushButton_2_clicked()
{
    if (fDoc.length() == 0) {
        message_error(tr("Save first!"));
        return;
    }
    if (ui->lePrepayment->asDouble() < 0.01) {
        message_error(tr("Amount of prepayment cannot be empty"));
        return;
    }
    PrintTaxN ptn(fPreferences.getDb(def_tax_address).toString(),
                  fPreferences.getDb(def_tax_port).toInt(),
                  fPreferences.getDb(def_tax_password).toString());
    QString in, out, err;
    int res = ptn.printAdvanceJson(ui->leModePrepayment->fHiddenText.toInt() == PAYMENT_CASH ? ui->lePrepayment->asDouble() : 0,
                         ui->leModePrepayment->fHiddenText.toInt() == PAYMENT_CASH ? 0 : ui->lePrepayment->asDouble(), in, out, err);
    if (res == pt_err_ok) {
        ui->pushButton_2->setEnabled(false);
        fDbBind[":f_prepaymentTax"] = 1;
        fDb.update("o_event", fDbBind, where_id(ap(fDoc)));

        fDbBind[":f_order"] = fDoc;
        fDbBind[":f_queryJson"] = in;
        fDbBind[":f_replyJson"] = out;
        fDbBind[":f_replyText"] = err;
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fAirDb.insert("tax_print", fDbBind);
    }
}
