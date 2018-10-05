#include "wvauchereditor.h"
#include "ui_wvauchereditor.h"
#include "message.h"
#include "dwselectorusers.h"
#include "dwselectorguest.h"
#include "dwselectorinvoiceitem.h"
#include "dwselectorpaymentmode.h"
#include "dwselectorcreditcard.h"
#include "dwselectorcityledger.h"
#include "dlgtracking.h"
#include "pprintvaucher.h"
#include "dlgnewvaucheroption.h"
#include "dwselectorcheckoutinvoice.h"
#include "dwselectorvatmode.h"
#include "paymentmode.h"
#include "vauchers.h"

#define DW_VAUCHER 1
#define DW_INVOICE 2

WVaucherEditor::WVaucherEditor(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::WVaucherEditor)
{
    ui->setupUi(this);
    fTc = new TrackControl(TRACK_VAUCHER);
    fTc->addWidget(ui->leSourceCode, "Source")
            .addWidget(ui->deWorking, "Working date")
            .addWidget(ui->deReal, "Real date")
            .addWidget(ui->teTime, "Time")
            .addWidget(ui->leUserCode, "User")
            .addWidget(ui->leRoom, "Room")
            .addWidget(ui->leGuestName, "Guest")
            .addWidget(ui->leItemCode, "Item")
            .addWidget(ui->leFinalName, "Final name")
            .addWidget(ui->leAmount, "Amount")
            .addWidget(ui->lePrepaid, "Prepaid")
            .addWidget(ui->leVAT, "VAT")
            .addWidget(ui->leUSD, "USD rate")
            .addWidget(ui->lePayCode, "Payment mode")
            .addWidget(ui->leFiscal, "Fiscal")
            .addWidget(ui->leCardCode, "Card code")
            .addWidget(ui->leCLCode, "City ledger code")
            .addWidget(ui->lePayComment, "Payment comment")
            .addWidget(ui->leDC, "Debit/Credit")
            .addWidget(ui->leSign, "Sign")
            .addWidget(ui->leInvoice, "Invoice")
            .addWidget(ui->leFinFlag, "Finance flag")
            .addWidget(ui->leCancelFlag, "Cancel flag")
            .addWidget(ui->leCancelReason, "Cancel reason")
            .addWidget(ui->leCancelUserCode, "Cancel user")
            .addWidget(ui->deCancelDate, "Cancel date")
            .addWidget(ui->leGuestCompany, "Guest/Company side")
            .addWidget(ui->teRemarks, "Remarks")
            .addWidget(ui->leVATCode, "Vat mode")
            ;
    DWSelectorUsers *dUsers = new DWSelectorUsers(this);
    dUsers->configure();
    dUsers->setSelector(ui->leUserCode);
    connect(dUsers, &DWSelectorUsers::user, [this](CI_User *ci) {
       dockResponse<CI_User, CacheUsers>(ui->leUserCode, ui->leUserName, ci);
    });

    DWSelectorUsers *dCUsers = new DWSelectorUsers(this);
    dCUsers->configure();
    dCUsers->setSelector(ui->leUserCode);
    connect(dCUsers, &DWSelectorUsers::user, [this](CI_User *ci) {
       dockResponse<CI_User, CacheUsers>(ui->leUserCode, ui->leUserName, ci);
    });

    DWSelectorInvoiceItem *dItem = new DWSelectorInvoiceItem(this);
    dItem->configure();
    dItem->setSelector(ui->leItemCode);
    connect(dItem, &DWSelectorInvoiceItem::invoiceItem, [this](CI_InvoiceItem *ci) {
       dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leItemCode, ui->leItemName, ci);
       if (fFlagNew && ci) {
           ui->leSourceCode->setText(ci->fVaucher);
           dockResponse<CI_Vaucher, CacheVaucher>(ui->leSourceCode, ui->leSourceName, 0);
           if (ui->leSourceCode->text() == VAUCHER_ROOMING_N) {
               ui->leFinalName->setText(ui->leItemName->text() + " " + ui->deWorking->text());
           } else{
            ui->leFinalName->setText(ui->leItemName->text());
           }
       }
    });

    DWSelectorPaymentMode *dPay = new DWSelectorPaymentMode(this);
    dPay->configure();
    dPay->setSelector(ui->lePayCode);
    connect(dPay, &DWSelectorPaymentMode::paymentMode, [this](CI_PaymentMode *ci) {
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, ci);
        if (ci && fFlagNew) {
            switch (ci->fCode.toInt()) {
            case PAYMENT_CASH:
                ui->lePayComment->setText(tr("CASH"));
                ui->leFinalName->setText(tr("PAYMENT CASH"));
                break;
            case PAYMENT_CARD:
                ui->lePayComment->setText("");
                ui->leFinalName->setText(tr("PAYMENT CARD"));
                break;
            case PAYMENT_CL:
                ui->lePayComment->setText("");
                ui->leFinalName->setText(tr("PAYMENT CL"));
                break;
            }
        }
    });

    DWSelectorVATMode *dVat = new DWSelectorVATMode(this);
    dVat->configure();
    dVat->setSelector(ui->leVATCode);
    connect(dVat, &DWSelectorVATMode::vatMode, [this](CI_VATMode *ci) {
       dockResponse<CI_VATMode, CacheVatMode>(ui->leVATCode, ui->leVATName, ci);
       if (ci) {
           if (ui->leSourceCode->text() == VAUCHER_POSTCHARGE_N
                   || ui->leSourceCode->text() == VAUCHER_ROOMING_N
                   || ui->leSourceCode->text() == VAUCHER_RECEIPT_N) {
               ui->leVAT->setDouble(Utils::countVATAmount(ui->leAmount->asDouble(), ui->leVATCode->asInt()));
           }
       }
    });
    ui->leVATCode->setInt(VAT_INCLUDED);
    dockResponse<CI_VATMode, CacheVatMode>(ui->leVATCode, ui->leVATName, 0);

    DWSelectorCreditCard *dCard = new DWSelectorCreditCard(this);
    dCard->configure();
    dCard->setSelector(ui->leCardCode);
    connect(dCard, &DWSelectorCreditCard::creditCard, [this](CI_CreditCard *ci) {
       dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, ci);
       if (ci && fFlagNew) {
           if (ui->lePayCode->asInt() == PAYMENT_CARD) {
               ui->leFinalName->setText("PAYMENT " + ci->fName);
               ui->lePayComment->setText(ci->fName);
           }
       }
    });

    DWSelectorCityLedger *dCL = new DWSelectorCityLedger(this);
    dCL->configure();
    dCL->setSelector(ui->leCLCode);
    connect(dCL, &DWSelectorCityLedger::cityLedger, [this](CI_CityLedger *ci) {
        dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, ci);
        if (ci && fFlagNew) {
            if (ui->lePayCode->asInt() == PAYMENT_CL) {
                ui->leFinalName->setText("PAYMENT " + ci->fName);
                ui->lePayComment->setText(ci->fName);
            }
        }
    });

    DWSelectorCheckoutInvoice *dInv = new DWSelectorCheckoutInvoice(this);
    dInv->configure();
    dInv->setSelector(ui->leInvoice);
    dInv->setDialog(this, DW_INVOICE);

    fDockVaucher = new DWSelectorVaucher(this);
    fDockVaucher->configure();
    fDockVaucher->setSelector(ui->leSourceCode);
    fDockVaucher->setDialog(this, DW_VAUCHER);

    fFlagNew = false;
}

WVaucherEditor::~WVaucherEditor()
{
    delete ui;
}

void WVaucherEditor::load(const QString &id)
{
    WVaucherEditor *ve = new WVaucherEditor(fPreferences.getDefaultParentForMessage());
    if (!id.isEmpty()) {
        ve->loadVaucher(id);
    }
    ve->exec();
    delete ve;
}

void WVaucherEditor::newVaucher(const QString &invoice)
{
    WVaucherEditor *ve = new WVaucherEditor(fPreferences.getDefaultParentForMessage());
    ve->ui->leInvoice->setText(invoice);
    ve->loadInvoice();
    ve->setNewFlag();
    ve->exec();
    delete ve;
}

void WVaucherEditor::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case DW_VAUCHER: {
        CI_Vaucher *ci = value.value<CI_Vaucher*>();
        dockResponse<CI_Vaucher, CacheVaucher>(ui->leSourceCode, ui->leSourceName, ci);
        if (!ci) {
            return;
        }
        break;
    }
    case DW_INVOICE: {
        CI_CheckoutInvoice *ci = value.value<CI_CheckoutInvoice*>();
        if (!ci) {
            return;
        }
        ui->leInvoice->setText(ci->fCode);
        loadInvoice();
    }
    }
}

void WVaucherEditor::loadVaucher(const QString &id)
{
    ui->leCode->setText(id);
    fDbBind[":f_id"] = id;
    DatabaseResult dr;
    dr.select(fDb, "select * from m_register where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Incorrect vaucher id");
        return;
    }

    ui->leSourceCode->setText(dr.value("f_source").toString());
    ui->deWorking->setDate(dr.value("f_wdate").toDate());
    ui->deReal->setDate(dr.value("f_rdate").toDate());
    ui->teTime->setTime(dr.value("f_time").toTime());
    ui->leUserCode->setText(dr.value("f_user").toString());
    ui->leRoom->setText(dr.value("f_room").toString());
    ui->leGuestName->setText(dr.value("f_guest").toString());
    ui->leItemCode->setText(dr.value("f_itemCode").toString());
    ui->leFinalName->setText(dr.value("f_finalName").toString());
    ui->leAmount->setText(dr.value("f_amountAmd").toString());
    ui->lePrepaid->setText(dr.value("f_usedPrepaid").toString());
    ui->leVAT->setText(dr.value("f_amountVat").toString());
    ui->leUSD->setText(dr.value("f_amountUsd").toString());
    ui->leFiscal->setText(dr.value("f_fiscal").toString());
    ui->lePayCode->setText(dr.value("f_paymentMode").toString());
    ui->leCardCode->setText(dr.value("f_creditCard").toString());
    ui->leCLCode->setText(dr.value("f_cityLedger").toString());
    ui->lePayComment->setText(dr.value("f_paymentComment").toString());
    ui->leDC->setText(dr.value("f_dc").toString());
    ui->leSign->setText(dr.value("f_sign").toString());
    ui->leInvoice->setText(dr.value("f_inv").toString());
    ui->leReservation->setText(dr.value("f_res").toString());
    ui->leFinFlag->setText(dr.value("f_finance").toString());
    ui->leCancelFlag->setText(dr.value("f_canceled").toString());
    ui->leCancelReason->setText(dr.value("f_cancelReason").toString());
    ui->deCancelDate->setDateTime(dr.value("f_cancelDate").toDateTime());
    ui->leCancelUserCode->setText(dr.value("f_cancelUser").toString());
    ui->leGuestCompany->setText(dr.value("f_sign").toString());
    ui->teRemarks->setPlainText(dr.value("f_remarks").toString());

    dockResponse<CI_User, CacheUsers>(ui->leUserCode, ui->leUserName, 0);
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leItemCode, ui->leItemName, 0);
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, 0);
    dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, 0);
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, 0);
    dockResponse<CI_User, CacheUsers>(ui->leCancelUserCode, ui->leCancelUserName, 0);
    dockResponse<CI_Vaucher, CacheVaucher>(ui->leSourceCode, ui->leSourceName, 0);

    fTc->resetChanges();
}

bool WVaucherEditor::errorCheck()
{
    QString errors;
    if (ui->leSourceName->isEmpty()) {
        errors += tr("Vaucher type is not selected") + "<br>";
    }
    if (!errors.isEmpty()) {
        message_error(errors);
    }
    return errors.isEmpty();
}

void WVaucherEditor::loadInvoice()
{
    CI_CheckoutInvoice *ci = CacheCheckoutInvoice::instance()->get(ui->leInvoice->text());
    if (!ci) {
        return;
    }
    ui->leInvoice->setText(ci->fCode);
    ui->leReservation->setText(ci->fReservation);
    ui->leRoom->setText(ci->fRoom);
    ui->leGuestName->setText(ci->fGuest);
}

void WVaucherEditor::setNewFlag()
{
    int vo = DlgNewVaucherOption::vaucherOption();
    if (!vo) {
        return;
    }
    fFlagNew = true;
    switch (vo) {
    case VO_RV:
        ui->leSourceCode->setText(VAUCHER_RECEIPT_N);
        dockResponse<CI_Vaucher, CacheVaucher>(ui->leSourceCode, ui->leSourceName, 0);
        setRV();
        break;
    case VO_PS:
        ui->leSourceCode->setText(VAUCHER_POSTCHARGE_N);
        dockResponse<CI_Vaucher, CacheVaucher>(ui->leSourceCode, ui->leSourceName, 0);
        setPS();
        break;
    default:
        break;
    }
}

void WVaucherEditor::setRV()
{
    ui->leCancelFlag->setText("0");
    ui->leCancelReason->setText("");
    ui->leCancelUserCode->setText("0");
    ui->leCancelUserName->setText("");
    ui->leUSD->setDouble(def_usd);
    ui->deWorking->setDate(QDate::currentDate());
    ui->deReal->setDate(QDate::currentDate());
    ui->teTime->setTime(QTime::currentTime());
    ui->leItemCode->setInt(fPreferences.getDb(def_receip_vaucher_id).toInt());
    ui->leVATCode->setInt(VAT_NOVAT);
    dockResponse<CI_VATMode, CacheVatMode>(ui->leVATCode, ui->leVATName, 0);
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leItemCode, ui->leItemName, 0);
    ui->leUserCode->setInt(WORKING_USERID);
    dockResponse<CI_User, CacheUsers>(ui->leUserCode, ui->leUserName, 0);
    ui->lePayCode->setInt(PAYMENT_CASH);
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, 0);
    ui->leSign->setText("-1");
    ui->leFinFlag->setText("1");
    ui->lePayComment->setText(tr("CASH"));
    ui->leFinalName->setText(tr("PAYMENT CASH"));
    ui->leDC->setText("CREDIT");
}

void WVaucherEditor::setPS()
{
    ui->leCancelFlag->setText("0");
    ui->leCancelReason->setText("");
    ui->leCancelUserCode->setText("0");
    ui->leCancelUserName->setText("");
    ui->leUSD->setDouble(def_usd);
    ui->deWorking->setDate(QDate::currentDate());
    ui->deReal->setDate(QDate::currentDate());
    ui->teTime->setTime(QTime::currentTime());
    ui->leItemCode->setInt(0);
    ui->leUserCode->setInt(WORKING_USERID);
    dockResponse<CI_User, CacheUsers>(ui->leUserCode, ui->leUserName, 0);
    ui->lePayCode->setInt(PAYMENT_CREDIT);
    dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePayCode, ui->lePayName, 0);
    ui->leSign->setText("1");
    ui->leFinFlag->setText("1");
    ui->lePayComment->setText(tr("CREDIT"));
    ui->leFinalName->setText(ui->leItemName->text());
    ui->leDC->setText("DEBIT");
}

void WVaucherEditor::on_btnSave_clicked()
{
    if (!errorCheck()) {
        return;
    }
    if (fFlagNew) {
        ui->leCode->setText(uuid(ui->leSourceCode->text(), fAirDb));
        fDbBind[":f_id"] = ui->leCode->text();
        fDb.insertWithoutId("m_register", fDbBind);
    }
    fDbBind[":f_source"] = ui->leSourceCode->text();
    fDbBind[":f_wdate"] = ui->deWorking->date();
    fDbBind[":f_rdate"] = ui->deWorking->date();
    fDbBind[":f_time"] = ui->teTime->time();
    fDbBind[":f_user"] = ui->leUserCode->asInt();
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuestName->text();
    fDbBind[":f_itemCode"] = ui->leItemCode->asInt();
    fDbBind[":f_finalName"] = ui->leFinalName->text();
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_usedPrepaid"] = ui->lePrepaid->asDouble();
    fDbBind[":f_amountVat"] = ui->leVAT->asDouble();
    fDbBind[":f_amountUSD"] = ui->leUSD->asDouble();
    fDbBind[":f_fiscal"] = ui->leFiscal->asInt();
    fDbBind[":f_paymentMode"] = ui->lePayCode->asInt();
    fDbBind[":f_creditCard"] = ui->leCardCode->asInt();
    fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
    fDbBind[":f_paymentComment"] = ui->lePayComment->text();
    fDbBind[":f_dc"] = ui->leDC->text();
    fDbBind[":f_sign"] = ui->leSign->asInt();
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_res"] = ui->leReservation->text();
    fDbBind[":f_finance"] = ui->leFinFlag->asInt();
    fDbBind[":f_canceled"] = ui->leCancelFlag->asInt();
    fDbBind[":f_cancelUser"] = ui->leCancelUserCode->asInt();
    fDbBind[":f_cancelDate"] = ui->deCancelDate->dateTime();
    fDbBind[":f_cancelReason"] = ui->leCancelReason->text();
    fDbBind[":f_side"] = ui->leGuestCompany->asInt();
    fDbBind[":f_remarks"] = ui->teRemarks->toPlainText();
    fDb.update("m_register", fDbBind, where_id(ap(ui->leCode->text())));
    fTc->saveChanges();
    fFlagNew = false;
    message_info_tr("Saved");
}

void WVaucherEditor::on_btnTrack_clicked()
{
    DlgTracking::showTracking(TRACK_VAUCHER, ui->leCode->text());
}

void WVaucherEditor::on_btnPrint_clicked()
{
    if (ui->leCode->isEmpty()) {
        message_error_tr("Vaucher is not saved");
        return;
    }
    PPrintVaucher::printVaucher(ui->leCode->text());
}

void WVaucherEditor::on_btnNewVaucher_clicked()
{

}

void WVaucherEditor::on_deWorking_textChanged(const QString &arg1)
{
    if (fFlagNew) {
        if (ui->leSourceCode->text() == VAUCHER_ROOMING_N) {
            ui->leFinalName->setText(ui->leItemName->text() + " " + arg1);
        }
    }
}

void WVaucherEditor::on_leAmount_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if (ui->leSourceCode->text() == VAUCHER_POSTCHARGE_N
            || ui->leSourceCode->text() == VAUCHER_ROOMING_N
            || ui->leSourceCode->text() == VAUCHER_RECEIPT_N) {
        ui->leVAT->setDouble(Utils::countVATAmount(ui->leAmount->asDouble(), ui->leVATCode->asInt()));
    }
}
