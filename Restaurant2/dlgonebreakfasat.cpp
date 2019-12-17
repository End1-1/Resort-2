#include "dlgonebreakfasat.h"
#include "ui_dlgonebreakfasat.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "paymentmode.h"
#include "pimage.h"
#include "rnumbers.h"
#include "rmessage.h"
#include "dlgreservation.h"
#include "cacheinvoiceitem.h"
#include "rpaymenttype.h"
#include "dlglist.h"
#include "vauchers.h"
#include "cacheinvoiceitem.h"
#include "cachecreditcard.h"
#include "cachecityledger.h"
#include "printtax.h"
#include "cachepaymentmode.h"
#include <QPrinter>
#include <QPainter>

DlgOneBreakfasat::DlgOneBreakfasat(User *u, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgOneBreakfasat)
{
    ui->setupUi(this);
    fUser = u;;
    hideAllMode();
}

DlgOneBreakfasat::~DlgOneBreakfasat()
{
    delete ui;
}

void DlgOneBreakfasat::on_btnPrint_clicked()
{
    if (ui->lePax->asDouble() < 1) {
        message_error(tr("Incorrect guest count"));
        return;
    }

    if (ui->lePayment->fHiddenText.toInt() == 0) {
        message_error(tr("Payment mode is not selected"));
        return;
    }

    switch (message_yesnocancel(tr("Save now?"))) {
    case RESULT_YES:
        break;
    case RESULT_NO:
        return;
    case RESULT_CANCEL:
        return;
    }

    CI_InvoiceItem *cb = CacheInvoiceItem::instance()->get(fPreferences.getDb(def_auto_breakfast_id).toString());
    if (!cb) {
        message_error(tr("Breakfast code is not defined. Contact to administrator"));
        return;
    }

    fDb.fDb.transaction();
    QString paymentComment = "CASH";
    switch (ui->lePayment->fHiddenText.toInt()) {
    case PAYMENT_BANK:
        paymentComment = tr("BANK");
        break;
    case PAYMENT_CARD:
        paymentComment = ui->leCardType->text();
        break;
    case PAYMENT_CL:
        paymentComment = ui->leCityLedger->text();
        break;
    }

    fDb.insertId("m_register", fBreakfastId);

    fDbBind[":f_source"] = cb->fVaucher;
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDbBind[":f_rdate"] = QDate::currentDate();
    fDbBind[":f_time"] = QTime::currentTime();
    fDbBind[":f_user"] = fUser->fId;
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_itemCode"] = cb->fCode;
    fDbBind[":f_finalName"] = QString("%1 S/N %2").arg(tr("BREAKFAST")).arg(fBreakfastId);
    fDbBind[":f_amountAmd"] = ui->leTotal->asDouble();
    fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->leTotal->asDouble(), VAT_INCLUDED);
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = ui->lePayment->fHiddenText;
    fDbBind[":f_creditCard"] = ui->leCardType->fHiddenText;
    fDbBind[":f_cityLedger"] = ui->leCityLedger->fHiddenText;
    fDbBind[":f_paymentComment"] = paymentComment;
    fDbBind[":f_dc"] = ui->lePayment->fHiddenText.toInt() == PAYMENT_CL ? "CREDIT" : "DEBIT";
    fDbBind[":f_sign"] = ui->lePayment->fHiddenText.toInt() == PAYMENT_CL ? -1 : 1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_remarks"] = "";
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(fBreakfastId)));

    fDbBind[":f_state"] = 2;
    fDb.update("o_breakfast", fDbBind, where_id(ap(fBreakfastId)));

    /*
    fDbBind[":f_show"] = ui->tblData->toInt(row, 5);
    fDbBind[":f_doc"] = vid;
    fDb.update("f_reservation_meal", fDbBind, where_id(ui->tblData->toInt(row, 0)));
    */
    fDb.fDb.commit();

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
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("RECEIPT #"))
                                     .arg(fBreakfastId),
                                     &th, f))->textHeight();
    top += ps->addTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Date")).arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th)->textHeight();
    ps->addLine(10, top, 680, top, boldPen);
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("NAME OF SERVICE:"), &th)->textHeight() + 10;
    f.setPointSize(40);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight + 20, tr("BREAKFAST"), &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("PAX:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, ui->lePax->text(), &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("PRICE PER PERSON:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(ui->lePrice->asDouble(), 2) + " AMD", &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("TOTAL AMOUNT:"), &th)->textHeight() + 10;
    f.setPointSize(40);
    f.setBold(true);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(ui->leTotal->asDouble(), 2) + " AMD", &th)->textHeight() + 10;
    f.setPointSize(24);
    f.setBold(false);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("EQUIVALENT OF USD:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(ui->leTotal->asDouble() / def_usd, 2) + " USD", &th)->textHeight() + 10;

    top += rowHeight;
    top += ps->addTextRect(10, top, 680, rowHeight, ui->leRoom->text() + ", " + ui->leGuest->text(), &th)->textHeight();
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

    ui->btnPrice->setEnabled(false);
    ui->btnPax->setEnabled(false);
    ui->btnPayment->setEnabled(false);
    ui->btnPrint->setEnabled(false);
    ui->btnPrintTax->setEnabled(true);
    ui->btnOpenDraft->setEnabled(false);
    ui->btnPrePrint->setEnabled(false);
}

void DlgOneBreakfasat::on_lePax_textChanged(const QString &arg1)
{
    ui->leTotal->setDouble(arg1.toDouble() * ui->lePrice->asDouble());
}

void DlgOneBreakfasat::on_lePrice_textChanged(const QString &arg1)
{
    ui->leTotal->setDouble(arg1.toDouble() * ui->lePax->asDouble());
}

void DlgOneBreakfasat::on_btnExit_clicked()
{
    accept();
}

void DlgOneBreakfasat::on_btnPax_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        ui->lePax->setInt(num);
    }
}

void DlgOneBreakfasat::on_btnPrice_clicked()
{
    float num = 0;
    if (RNumbers::getNumber(num, 0, this)) {
        ui->lePrice->setInt(num);
    }
}

void DlgOneBreakfasat::on_btnPayment_clicked()
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
        hideAllMode();
        cardName = "CASH";
        break;
    case 1:
        paymentMode = PAYMENT_CARD;
        setModeCard();
        cardName = "VISA";
        break;
    case 2:
        paymentMode = PAYMENT_CARD;
        setModeCard();
        cardName = "MAESTRO";
        break;
    case 3:
        paymentMode = PAYMENT_CARD;
        setModeCard();
        cardName = "MASTERCARD";
        break;
    case 4:
        fDbBind[":f_paymentMode"] = PAYMENT_CARD;
        setModeCard();
        cardName = "ARCA";
        break;
    case 5:
        paymentMode = PAYMENT_CARD;
        setModeCard();
        cardName = "AMEX";
        break;

    case 6: {
        DlgReservation *d = new DlgReservation(this);
        d->loadRoom();
        if (d->exec() == QDialog::Accepted) {
            fDbBind[":f_id"] = d->fReservationId;
            fDb.select("select f_invoice, f_room, g.guest "
                       "from f_reservation r "
                       "inner join guests g on g.f_id=r.f_guest "
                       "where r.f_id=:f_id", fDbBind, fDbRows);
            ui->leInvoice->setText(fDbRows.at(0).at(0).toString());
            ui->leRoom->setText(fDbRows.at(0).at(1).toString());
            ui->leGuest->setText(fDbRows.at(0).at(2).toString());
            cardName = QString("%1, %2").arg(ui->leRoom->text()).arg(ui->leGuest->text());
        } else {
            delete d;
            return;
        }
        setModeRoom();
        ui->leRoom->setText(fDbRows.at(0).at(1).toString());
        ui->leInvoice->setText(fDbRows.at(0).at(0).toString());
        ui->leGuest->setText(fDbRows.at(0).at(2).toString());
        paymentMode = PAYMENT_ROOM;
        break;
    }
    case 7: {
        DlgReservation *d = new DlgReservation(this);
        d->loadCL();
        if (d->exec() == QDialog::Accepted) {
            cityLedger = d->fReservationId.toInt();
            cityLedgerName = CacheCityLedger::instance()->get(d->fReservationId)->fName;
            paymentMode = PAYMENT_CL;
        } else {
            delete d;
            return;
        }
        setModeCityLedger();
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
        ui->lePayment->fHiddenText = QString::number(paymentMode);
        ui->lePayment->setText(i->fName);
        ui->leCardType->setText(cardName);
        ui->leCardType->fHiddenText = QString::number(cardCode);
        ui->leCityLedger->setText(cityLedgerName);
        ui->leCityLedger->fHiddenText = QString::number(cityLedger);
    } else {
        ui->lePayment->clear();
        ui->lePayment->fHiddenText = "";
        ui->leCardType->clear();
        ui->leCardType->fHiddenText = "";
    }
}

void DlgOneBreakfasat::on_btnPrintTax_clicked()
{

}

void DlgOneBreakfasat::hideAllMode()
{
    ui->lbCardType->setVisible(false);
    ui->leCardType->setVisible(false);
    ui->lbCityLedger->setVisible(false);
    ui->leCityLedger->setVisible(false);
    ui->leRoom->setVisible(false);
    ui->leInvoice->setVisible(false);
    ui->lbRoom->setVisible(false);
    ui->lbInvoice->setVisible(false);
    ui->btnRoom->setVisible(false);
    ui->leGuest->setVisible(false);
    ui->lbGuest->setVisible(false);
    ui->leCardType->clear();
    ui->leCardType->fHiddenText.clear();
    ui->leCityLedger->clear();
    ui->leCityLedger->fHiddenText.clear();
    ui->leRoom->clear();
    ui->leGuest->clear();
    ui->leInvoice->clear();
}

void DlgOneBreakfasat::setModeCard()
{
    hideAllMode();
    ui->lbCardType->setVisible(true);
    ui->leCardType->setVisible(true);
}

void DlgOneBreakfasat::setModeCityLedger()
{
    hideAllMode();
    ui->lbCityLedger->setVisible(true);
    ui->leCityLedger->setVisible(true);
}

void DlgOneBreakfasat::setModeRoom()
{
    hideAllMode();
    ui->lbGuest->setVisible(true);
    ui->lbInvoice->setVisible(true);
    ui->lbRoom->setVisible(true);
    ui->leGuest->setVisible(true);
    ui->leInvoice->setVisible(true);
    ui->leRoom->setVisible(true);
}

void DlgOneBreakfasat::on_btnPrePrint_clicked()
{
    CI_InvoiceItem *cb = CacheInvoiceItem::instance()->get(fPreferences.getDb(def_auto_breakfast_id).toString());
    if (!cb) {
        message_error("Breakfast code is not defined. Contact to administrator");
        return;
    }

    fDbBind[":f_user"] = fUser->fId;
    fDbBind[":f_pax"] = ui->lePax->asInt();
    fDbBind[":f_price"] = ui->lePrice->asDouble();
    fDbBind[":f_paymentMode"] = ui->lePayment->fHiddenText.toInt();
    fDbBind[":f_card"] = ui->leCardType->fHiddenText.toInt();
    fDbBind[":f_cityLedger"] = ui->leCityLedger->fHiddenText.toInt();
    fDbBind[":f_room"] = ui->leRoom->text().toInt();

    fBreakfastId = uuuid(cb->fVaucher, fDb);
    fDb.insertId("o_breakfast", fBreakfastId);
    fDb.update("o_breakfast", fDbBind, where_id(ap(fBreakfastId)));

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
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(tr("RECEIPT #"))
                                     .arg(fBreakfastId),
                                     &th, f))->textHeight();
    top += ps->addTextRect(10, top, 680, rowHeight, QString("%1: %2").arg(tr("Date")).arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th)->textHeight();
    ps->addLine(10, top, 680, top, boldPen);
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("NAME OF SERVICE:"), &th)->textHeight() + 10;
    f.setPointSize(40);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight + 20, tr("BREAKFAST"), &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("PAX:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, ui->lePax->text(), &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("PRICE PER PERSON:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(ui->lePrice->asDouble(), 2) + " AMD", &th)->textHeight() + 10;
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("TOTAL AMOUNT:"), &th)->textHeight() + 10;
    f.setPointSize(40);
    f.setBold(true);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(ui->leTotal->asDouble(), 2) + " AMD", &th)->textHeight() + 10;
    f.setPointSize(24);
    f.setBold(false);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("EQUIVALENT OF USD:"), &th)->textHeight() + 10;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, float_str(ui->leTotal->asDouble() / def_usd, 2) + " USD", &th)->textHeight() + 10;

    top += rowHeight;
    top += ps->addTextRect(10, top, 680, rowHeight, ui->leRoom->text() + ", " + ui->leGuest->text(), &th)->textHeight();
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

    ui->btnPrint->setEnabled(true);
    ui->btnPrePrint->setEnabled(false);
}

void DlgOneBreakfasat::on_btnOpenDraft_clicked()
{
    QString name;
    QString sql = "select f_id, concat('Pax: ', f_pax, ' Price: ', f_price, ' Room: ', f_room) from o_breakfast"
            " where f_state=1";
    QVariant draftId;
    if (!DlgList::getValue(tr("Breakfast"), name, draftId, sql)) {
        return;
    }
    fDbBind[":f_id"] = draftId;
    fDb.select("select f_pax, f_price, f_paymentMode, f_card, f_cityLedger, f_room "
               "from o_breakfast where f_id=:f_id", fDbBind, fDbRows);
    if (fDbRows.count() == 0) {
        return;
    }
    fBreakfastId = draftId.toString();
    ui->lePax->setText(fDbRows.at(0).at(0).toString());
    ui->lePrice->setText(fDbRows.at(0).at(1).toString());
    ui->lePayment->fHiddenText = fDbRows.at(0).at(2).toString();
    ui->leCardType->fHiddenText = fDbRows.at(0).at(3).toString();
    ui->leCityLedger->fHiddenText = fDbRows.at(0).at(4).toString();
    ui->leRoom->setText(fDbRows.at(0).at(5).toString());
    CI_PaymentMode *pm = CachePaymentMode::instance()->get(ui->lePayment->fHiddenText);
    if (pm) {
        ui->lePayment->setText(pm->fName);
    }
    CI_CreditCard *cc = CacheCreditCard::instance()->get(ui->leCardType->fHiddenText);
    if (cc) {
        setModeCard();
        ui->leCardType->setText(cc->fName);
    }
    CI_CityLedger *cl = CacheCityLedger::instance()->get(ui->leCityLedger->fHiddenText);
    if (cl) {
        setModeCityLedger();
        ui->leCityLedger->setText(cl->fName);
    }
    if (ui->leRoom->text().toInt() > 0) {
        fDbBind[":f_room"] = ui->leRoom->text().toInt();
        fDbBind[":f_state"] = RESERVE_CHECKIN;
        fDb.select("select r.f_invoice, g.guest, f_room from f_reservation r "
                   "left join guests g on g.f_id=r.f_guest "
                   "where r.f_room=:f_room and r.f_state=:f_state", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            setModeRoom();
            ui->leRoom->setText(fDbRows.at(0).at(2).toString());
            ui->leInvoice->setText(fDbRows.at(0).at(0).toString());
            ui->leGuest->setText(fDbRows.at(0).at(1).toString());
        }
    }
    ui->btnPrePrint->setEnabled(false);
    ui->btnPrint->setEnabled(true);
}

void DlgOneBreakfasat::on_btnRoom_clicked()
{

}
