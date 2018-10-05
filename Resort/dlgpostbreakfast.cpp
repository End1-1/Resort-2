#include "dlgpostbreakfast.h"
#include "ui_dlgpostbreakfast.h"
#include "vauchers.h"
#include "dwselectorpaymentmode.h"
#include "dlgprinttaxsm.h"
#include "dwselectorinvoiceitem.h"
#include "dwselectorhall.h"
#include "cachereststore.h"
#include "dwselectorcreditcard.h"
#include "paymentmode.h"

#define SEL_TABLE 1
#define SEL_DISH 2


DlgPostBreakfast::DlgPostBreakfast(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPostBreakfast)
{
    ui->setupUi(this);
    ui->lbCard->setVisible(false);
    ui->leCardCode->setVisible(false);
    ui->leCardName->setVisible(false);
    DWSelectorPaymentMode *pm = new DWSelectorPaymentMode(this);
    pm->configure();
    pm->setSelector(ui->lePMCode);
    QStringList pmm;
    pmm << QString::number(PAYMENT_CASH)
        << QString::number(PAYMENT_CARD)
        << QString::number(PAYMENT_CREDIT);
    pm->setFilterList(pmm);
    connect(pm, &DWSelectorPaymentMode::paymentMode, [this](CI_PaymentMode *ci) {
        dockResponse<CI_PaymentMode, CachePaymentMode>(ui->lePMCode, ui->lePMComment, ci);
        ui->lbCard->setVisible(false);
        ui->leCardCode->setVisible(false);
        ui->leCardName->setVisible(false);
        if (ci) {
            if (ci->fCode.toInt() == PAYMENT_CARD) {
                ui->lbCard->setVisible(true);
                ui->leCardCode->setVisible(true);
                ui->leCardName->setVisible(true);
            }
        }
    });

    DWSelectorHall *dHall = new DWSelectorHall(this);
    dHall->configure();
    dHall->setSelector(ui->leHall);
    QStringList hl = fPreferences.getDb(def_rest_hall_for_reception).toString().split(",", QString::SkipEmptyParts);
    dHall->setFilterList(hl);
    connect(dHall, &DWSelectorHall::hall, [this](CI_RestHall *ci) {
        dockResponse<CI_RestHall, CacheRestHall>(ui->leHall, ui->leHallName, ci);
        if (ui->leHall->asInt() > 0) {
            fDockTable->fHall = ui->leHall->asInt();
        } else {
            fDockTable->fHall = -1;
        }
        if (ui->leHall->asInt() > 0) {
            ci = CacheRestHall::instance()->get(ui->leHall->text());
            fDockDish->fMenu = ci->fDefaultMenu.toInt();
        } else {
            fDockDish->fMenu = -1;
        }
    });

    DWSelectorCreditCard *dockCard = new DWSelectorCreditCard(this);
    dockCard->configure();
    dockCard->setSelector(ui->leCardCode);
    connect(dockCard, &DWSelectorCreditCard::creditCard, [this](CI_CreditCard *ci) {
        dockResponse<CI_CreditCard, CacheCreditCard>(ui->leCardCode, ui->leCardName, ci);
    });

    fDockTable = new DWSelectorRestTable(this);
    fDockTable->configure();
    fDockTable->setSelector(ui->leTable);
    fDockTable->setDialog(this, SEL_TABLE);

    fDockDish = new DWSelectorRestFullMenu(this);
    fDockDish->fMenu = -1;
    fDockDish->configure();
    fDockDish->setSelector(ui->leDish);
    fDockDish->setDialog(this, SEL_DISH);
}

DlgPostBreakfast::~DlgPostBreakfast()
{
    delete ui;
}

bool DlgPostBreakfast::postBreakfast(const QString &invoice, POST_TYPE pt)
{
    DlgPostBreakfast *d = new DlgPostBreakfast(fPreferences.getDefaultParentForMessage());
    d->ui->leInvoice->setText(invoice);
    DatabaseResult dr;
    d->fDbBind[":f_invoice"] = invoice;
    dr.select("select r.f_id, r.f_room, g.guest from f_reservation r left join guests g on r.f_guest=g.f_id where f_invoice=:f_invoice", d->fDbBind);
    if (dr.rowCount() > 0) {
        d->ui->leRes->setText(dr.value("f_id").toString());
        d->ui->leRoom->setText(dr.value("f_room").toString());
        d->ui->leGuest->setText(dr.value("guest").toString());
    }
    switch (pt) {
    case ptBreakfast: {
        d->ui->leHall->setInt(fPreferences.getDb(def_breakfast_default_hall).toInt());
        d->ui->leTable->setInt(fPreferences.getDb(def_breakfast_default_table).toInt());
        d->ui->leDish->setInt(fPreferences.getDb(def_breakfast_default_dish).toInt());
        d->dockResponse<CI_RestHall, CacheRestHall>(d->ui->leHall, d->ui->leHallName, 0);
        d->dockResponse<CI_RestTable, CacheRestTable>(d->ui->leTable, d->ui->leTableName, 0);
        d->dockResponse<CI_RestFullMenu, CacheRestFullMenu>(d->ui->leDish, d->ui->leDishName, 0);
        CI_RestHall *ch = CacheRestHall::instance()->get(d->ui->leHall->text());
        if (ch) {
            d->fDockDish->fMenu = ch->fDefaultMenu.toInt();
        }
        CI_RestFullMenu *ci = CacheRestFullMenu::instance()->get(d->ui->leDish->text());
        if (ci) {
            d->ui->leStore->fHiddenText = QString::number(ci->fStore);
            d->ui->leDishName->setText(ci->fDishName[def_lang]);
            d->ui->lePrice->setDouble(ci->fPrice);
            CI_RestStore *cs = CacheRestStore::instance()->get(ci->fStore);
            if (cs) {
                d->ui->leStore->setText(cs->fName);
            }
        }
        d->ui->leQty->setFocus();
        break;
    }
    case ptMinibar: {
        d->ui->leHall->setInt(fPreferences.getDb(def_minibar_default_hall).toInt());
        d->ui->leTable->setInt(fPreferences.getDb(def_minibar_default_table).toInt());
        d->ui->leDish->setInt(fPreferences.getDb(def_minibar_default_dish).toInt());
        d->dockResponse<CI_RestHall, CacheRestHall>(d->ui->leHall, d->ui->leHallName, 0);
        d->dockResponse<CI_RestTable, CacheRestTable>(d->ui->leTable, d->ui->leTableName, 0);
        d->dockResponse<CI_RestFullMenu, CacheRestFullMenu>(d->ui->leDish, d->ui->leDishName, 0);
        CI_RestHall *ch = CacheRestHall::instance()->get(d->ui->leHall->text());
        if (ch) {
            d->fDockDish->fMenu = ch->fDefaultMenu.toInt();
        }
        CI_RestFullMenu *ci = CacheRestFullMenu::instance()->get(d->ui->leDish->text());
        if (ci) {
            d->ui->leStore->fHiddenText = QString::number(ci->fStore);
            d->ui->leDishName->setText(ci->fDishName[def_lang]);
            d->ui->lePrice->setDouble(ci->fPrice);
            CI_RestStore *cs = CacheRestStore::instance()->get(ci->fStore);
            if (cs) {
                d->ui->leStore->setText(cs->fName);
            }
        }
        d->ui->leQty->setFocus();
        break;
    }
    default:
        break;
    }
    bool result = d->exec() == QDialog::Accepted;
    delete d;
    return result;
}

void DlgPostBreakfast::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_TABLE:
        dockResponse<CI_RestTable, CacheRestTable>(ui->leTable, ui->leTableName, value.value<CI_RestTable*>());
        break;
    case SEL_DISH: {
        CI_RestFullMenu *ci = value.value<CI_RestFullMenu*>();
        dockResponse<CI_RestFullMenu, CacheRestFullMenu>(ui->leDish, ui->leDishName, ci);
        if (ci) {
            ui->leDish->setText(ci->fCode);
            ui->leDishName->setText(ci->fDishName[def_lang]);
            ui->lePrice->setDouble(ci->fPrice);
            ui->leStore->fHiddenText = QString::number(ci->fStore);
            CI_RestStore *cs = CacheRestStore::instance()->get(ci->fStore);
            if (cs) {
                ui->leStore->setText(cs->fName);
            }
        }
        }
        break;
    default:
        break;
    }
}

void DlgPostBreakfast::on_btnCancel_clicked()
{
    reject();
}

void DlgPostBreakfast::on_btnSave_clicked()
{
    if (ui->leHall->asInt() == 0) {
        message_error(tr("Hall must be selected"));
        return;
    }
    if (ui->leTable->asInt() == 0) {
        message_error(tr("Table must be selected"));
        return;
    }
    if (ui->leDish->asInt() == 0) {
        message_error(tr("Dish must be selected"));
        return;
    }
    if (ui->leQty->asDouble() < 0.01) {
        message_error(tr("Invalid quantity of the guests"));
        return;
    }
    if (ui->leAmount->asDouble() < 0.01) {
        message_error(tr("Invalid amount"));
        return;
    }
    if (ui->lePMCode->asInt() == 0) {
        message_error(tr("Payment mode is not selected"));
        return;
    }
    if (ui->lePMCode->asInt() == PAYMENT_CARD && ui->leCardCode->asInt() == 0) {
        message_error(tr("Card is not selected"));
        return;
    }
    bool isNew = false;
    fDb.fDb.transaction();
    if (ui->leVoucher->isEmpty()) {
        isNew = true;
        QString rid = uuid(VAUCHER_POINT_SALE_N, fAirDb);
        fDb.insertId("m_register", rid);
        ui->leVoucher->setText(rid);
    }
    fDbBind[":f_source"] = VAUCHER_POINT_SALE_N;
    if (isNew) {
        fDbBind[":f_wdate"] = ui->deDate->date();
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
    }
    fDbBind[":f_room"] = ui->leRoom->text();
    fDbBind[":f_guest"] = ui->leGuest->text();
    fDbBind[":f_itemCode"] = fPreferences.getDb(def_auto_breakfast_id).toInt();
    fDbBind[":f_finalName"] = ui->leHallName->text() + " S/N " + ui->leVoucher->text();
    fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
    fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->leAmount->asDouble(), VAT_INCLUDED);
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = PAYMENT_ROOM;
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = 0;
    fDbBind[":f_paymentComment"] = ui->leRoom->text() + "," + ui->leGuest->text();
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
    fDbBind[":f_cash"] = ui->lePMCode->asInt() == PAYMENT_CREDIT ? 0 : 1;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVoucher->text())));

    QString headerId = ui->leVoucher->text();
    fDbBind[":f_id"] = headerId;
    fDb.insertWithoutId("o_header", fDbBind);
    fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    fDbBind[":f_hall"] = ui->leHall->asInt();
    fDbBind[":f_table"] = ui->leTable->asInt();
    fDbBind[":f_staff"] = WORKING_USERID;
    fDbBind[":f_dateOpen"] = QDateTime::currentDateTime();
    fDbBind[":f_dateClose"] = QDateTime::currentDateTime();
    fDbBind[":f_dateCash"] = ui->deDate->date();
    fDbBind[":f_paymentMode"] = PAYMENT_ROOM;
    fDbBind[":f_paymentModeComment"] = ui->leRoom->text() + "," + ui->leGuest->text();
    fDbBind[":f_comment"] = "";
    fDbBind[":f_cityLedger"] = 0;
    fDbBind[":f_reservation"] = "";
    fDbBind[":f_complex"] = 0;
    fDbBind[":f_print"] = 0;
    fDbBind[":f_tax"] = 0;
    fDbBind[":f_roomComment"] = ui->leRoom->text() + ", " + ui->leGuest->text();
    fDbBind[":f"] = 0;
    fDbBind[":f_total"] = ui->leAmount->asDouble();
    fDb.update("o_header", fDbBind, where_id(ap(headerId)));

    CI_RestFullMenu *rf = CacheRestFullMenu::instance()->get(ui->leDish->text());
    QString dishId = uuid("DR", fAirDb);
    fDbBind[":f_id"] = dishId;
    fDb.insertWithoutId("o_dish", fDbBind);
    fDbBind[":f_header"] = headerId;
    fDbBind[":f_state"] = DISH_STATE_READY;
    fDbBind[":f_dish"] = rf->fDishCode;
    fDbBind[":f_qty"] = ui->leQty->asDouble();
    fDbBind[":f_qtyPrint"] = ui->leQty->asDouble();
    fDbBind[":f_price"] = ui->lePrice->asDouble();
    fDbBind[":f_svcValue"] = 0;
    fDbBind[":f_svcAmount"] = 0;
    fDbBind[":f_dctValue"] = 0;
    fDbBind[":f_dctAmount"] = 0;
    fDbBind[":f_total"] = ui->leAmount->asDouble();
    fDbBind[":f_totalUSD"] = def_usd;
    fDbBind[":f_print1"] = "";
    fDbBind[":f_print2"] = "";
    fDbBind[":f_store"] = ui->leStore->fHiddenText.toInt();
    fDbBind[":f_comment"] = "";
    fDbBind[":f_staff"] = WORKING_USERID;
    fDbBind[":f_complex"] = 0;
    fDbBind[":f_complexId"] = 0;
    fDbBind[":f_adgt"] = ui->leDish->fHiddenText;
    fDbBind[":f_complexRec"] = "";
    fDbBind[":f_cancelUser"] = 0;
    fDb.update("o_dish", fDbBind, where_id(ap(dishId)));

    if (ui->lePMCode->asInt() == PAYMENT_CASH || ui->lePMCode->asInt() == PAYMENT_CARD) {
        QString voucherPay = uuid(VAUCHER_RECEIPT_N, fAirDb);
        fDb.insertId("m_register", voucherPay);
        if (isNew) {
            fDbBind[":f_wdate"] = ui->deDate->date();
            fDbBind[":f_rdate"] = QDate::currentDate();
            fDbBind[":f_time"] = QTime::currentTime();
            fDbBind[":f_user"] = WORKING_USERID;
        }
        fDbBind[":f_source"] = VAUCHER_RECEIPT_N;
        fDbBind[":f_room"] = ui->leRoom->text();
        fDbBind[":f_guest"] = ui->leGuest->text();
        fDbBind[":f_itemCode"] = fPreferences.getDb(def_auto_breakfast_id).toInt();
        fDbBind[":f_finalName"] = vaucherPaymentName(ui->lePMCode->asInt(),
                                                     ui->leCardCode->text(),
                                                     "");
        fDbBind[":f_amountAmd"] = ui->leAmount->asDouble();
        fDbBind[":f_amountVat"] = Utils::countVATAmount(ui->leAmount->asDouble(), VAT_INCLUDED);
        fDbBind[":f_amountUsd"] = def_usd;
        fDbBind[":f_fiscal"] = 0;
        fDbBind[":f_paymentMode"] = ui->lePMCode->asInt();
        fDbBind[":f_creditCard"] = 0;
        fDbBind[":f_cityLedger"] = 0;
        fDbBind[":f_paymentComment"] = vaucherPaymentName(ui->lePMCode->asInt(),
                                                          ui->leCardCode->text(),
                                                          "");
        fDbBind[":f_dc"] = "CREDIT";
        fDbBind[":f_sign"] = -1;
        fDbBind[":f_doc"] = "";
        fDbBind[":f_rec"] = "";
        fDbBind[":f_inv"] = ui->leInvoice->text();
        fDbBind[":f_finance"] = 1;
        fDbBind[":f_remarks"] = "";
        fDbBind[":f_canceled"] = 0;
        fDbBind[":f_cancelReason"] = "";
        fDbBind[":f_side"] = 0;
        fDbBind[":f_rb"] = 0;
        fDbBind[":f_cash"] = ui->lePMCode->asInt() == PAYMENT_CREDIT ? 0 : 1;
        fDb.update("m_register", fDbBind, where_id(ap(voucherPay)));
    }

    fDb.fDb.commit();
    ui->btnSave->setEnabled(false);
    message_info(tr("Saved"));
}

void DlgPostBreakfast::on_btnPrintTax_clicked()
{
    if (ui->leTax->asInt() > 0) {
        message_error(tr("Tax already printed"));
        return;
    }
    CI_InvoiceItem *ii = CacheInvoiceItem::instance()->get(fPreferences.getDb(def_auto_breakfast_id).toInt());
    if (!ii) {
        message_error(tr("Application error. Contact to developer. Message DlgPostBreakfast invoice item = 0"));
        return;
    }

    DlgPrintTaxSM dpt;
    dpt.addGoods(ii->fVatDept,
                 ii->fAdgt,
                 ii->fCode,
                 ii->fTaxName,
                 ui->lePrice->asDouble(),
                 ui->leQty->asDouble());
    dpt.fOrder = ui->leVoucher->text();
    dpt.fCardAmount = 0;
    dpt.fPrepaid = 0;

    int result = dpt.exec();
    if (result == TAX_OK) {
        fDbBind[":f_fiscal"] = dpt.fTaxCode;
        fDb.update("m_register", fDbBind, where_id(ap(ui->leVoucher->text())));
        ui->leTax->setInt(dpt.fTaxCode);

        fDbBind[":f_vaucher"] = ui->leVoucher->text();
        fDbBind[":f_invoice"] = ui->leInvoice->text();
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_name"] = ii->fName;
        fDbBind[":f_amountCash"] = ui->leAmount->asDouble();
        fDbBind[":f_amountCard"] = 0;
        fDbBind[":f_amountPrepaid"] = 0;
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comp"] = HOSTNAME;
        fDb.insert("m_tax_history", fDbBind);
        return;
    }
}

void DlgPostBreakfast::on_leQty_textChanged(const QString &arg1)
{
    ui->leAmount->setDouble(arg1.toDouble() * ui->lePrice->asDouble());
}

void DlgPostBreakfast::on_leAmount_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
}
