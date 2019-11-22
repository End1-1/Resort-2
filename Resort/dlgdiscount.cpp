#include "dlgdiscount.h"
#include "paymentmode.h"
#include "ui_dlgdiscount.h"
#include "cacheactiveroom.h"
#include "cacheinvoiceitem.h"
#include "vauchers.h"
#include "pprintvaucher.h"

DlgDiscount::DlgDiscount(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgDiscount)
{
    ui->setupUi(this);
    ui->leInvoice->setVisible(false);
    ui->leValue->setValidator(new QDoubleValidator(0, 999999999, 2));
    CI_InvoiceItem *i = CacheInvoiceItem::instance()->get(fPreferences.getDb(def_invoice_default_discount_id).toString());
    if (i) {
        ui->leType->setText(i->fName);
        ui->leType->fHiddenText = i->fCode;
    }
    fDockRoom = new DWSelectorActiveRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->leRoomCode);
    connect(fDockRoom, SIGNAL(roomChanged(CI_ActiveRoom*)), this, SLOT(roomChanged(CI_ActiveRoom*)));

    fDockDisc = new DWSelectorInvoiceItem(this);
    fDockDisc->configure();
    fDockDisc->setSelector(ui->leTypeCode);
    fDockDisc->setSourceFilter(VAUCHER_DISCOUNT_N);
    connect(fDockDisc, SIGNAL(invoiceItem(CI_InvoiceItem*)), this, SLOT(discount(CI_InvoiceItem*)));

    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCLCode);
    connect(fDockCL, SIGNAL(cityLedger(CI_CityLedger*)), this, SLOT(cityLedger(CI_CityLedger*)));

    sideChanged(true);
    fTrackControl = new TrackControl(TRACK_RESERVATION);
    fTrackControl->addWidget(ui->deDate, "Date")
            .addWidget(ui->leCLCode, "City ledger")
            .addWidget(ui->leDiscountAmount, "Discount amount")
            .addWidget(ui->leFinalAmount, "Final amount")
            .addWidget(ui->leGuest, "Guest")
            .addWidget(ui->leRoomCode, "Room")
            .addWidget(ui->rbGuest, "Guest/Company");
}

DlgDiscount::~DlgDiscount()
{
    delete ui;
}

void DlgDiscount::setParams(const QString &room, double guestAmount, double companyAmount)
{
    ui->leRoomCode->setText(room);
    fGuestAmount = guestAmount;
    fCompanyAmount = companyAmount;
    CI_ActiveRoom *ar = CacheActiveRoom::instance()->get(room);
    if (!ar) {
        message_error("Application error, contact to developer. Message: Discount dialog, set params, active room==0");
        return;
    }
    fInvoice = ar->fInvoice;
    ui->leInvoice->setText(fInvoice);
    ui->leRoomName->setText(ar->fRoomName);
    ui->leGuest->setText(ar->fGuestName);
    ui->leAmount->setText(float_str(guestAmount, 2));
    ui->leFinalAmount->setText(float_str(guestAmount, 2));
    fTrackControl->resetChanges();
}

void DlgDiscount::openVaucher(const QString &id)
{
    DlgDiscount *d = new DlgDiscount(fPreferences.getDefaultParentForMessage());
    DatabaseResult dr;
    d->fDbBind[":f_id"] = id;
    dr.select(d->fDb, "select * from m_register where f_id=:f_id", d->fDbBind);
    if (dr.rowCount() == 0) {
        message_error_tr("Cannot load voucher");
        delete d;
        return;
    }
    d->ui->deDate->setDate(dr.value("f_wdate").toDate());
    d->ui->leVaucher->setText(id);
    d->ui->leInvoice->setText(dr.value("f_inv").toString());
    d->ui->leRoomCode->setText(dr.value("f_room").toString());
    d->ui->leGuest->setText(dr.value("f_guest").toString());
    d->ui->leCLCode->setText(dr.value("f_cityledger").toString());
    d->ui->leAmount->setText(dr.value("f_amountAmd").toString());
    d->ui->rbGuest->setChecked(dr.value("f_rb").toInt() == 0);
    d->fTrackControl->resetChanges();
    d->exec();
    delete d;
}

void DlgDiscount::discount(CI_InvoiceItem *c)
{
    dockResponse<CI_InvoiceItem, CacheInvoiceItem>(ui->leTypeCode, ui->leType, c);
}

void DlgDiscount::roomChanged(CI_ActiveRoom *c)
{
    dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leRoomCode, ui->leRoomName, c);
    if (c) {
        fDbBind[":invoice"] = c->fInvoice;
        ui->leInvoice->setText(c->fInvoice);
        fDb.select("select l.f_amount, r.f_amount "
                   "from m_register i "
                   "left join (select f_inv, sum(f_amountAmd*f_sign)as f_amount from m_register where f_inv=:invoice and f_side=0 and f_finance=1 and f_canceled=0 group by 1) l on l.f_inv=i.f_inv "
                   "left join (select f_inv, sum(f_amountAmd*f_sign)as f_amount from m_register where f_inv=:invoice and f_side=1 and f_finance=1 and f_canceled=0 group by 1) r on r.f_inv=i.f_inv "
                   "where i.f_inv=:invoice ", fDbBind, fDbRows);
        if (fDbRows.count() > 0) {
            setParams(c->fRoomCode, fDbRows.at(0).at(0).toDouble(), fDbRows.at(0).at(1).toDouble());
        } else {
            message_error_tr("No active invoice for this room");
        }
    }
}

void DlgDiscount::cityLedger(CI_CityLedger *c)
{
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCLCode, ui->leCLName, c);
}

void DlgDiscount::on_btnCancel_clicked()
{
    reject();
}

void DlgDiscount::on_leValue_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    countDiscount();
}

void DlgDiscount::on_rbAmount_clicked()
{
    ui->lbType->setText("AMD");
    ui->leValue->setValidator(new QDoubleValidator(0, 999999999, 2));
    countDiscount();
}

void DlgDiscount::countDiscount()
{
    if (ui->rbAmount->isChecked()) {
        ui->leFinalAmount->setDouble(ui->leAmount->asDouble() - ui->leValue->asDouble());
        ui->leDiscountAmount->setText(ui->leValue->text());
    } else {
        ui->leDiscountAmount->setDouble(ui->leAmount->asDouble() * (ui->leValue->asDouble() / 100));
        ui->leFinalAmount->setDouble(ui->leAmount->asDouble() - ui->leDiscountAmount->asDouble());
    }
}

void DlgDiscount::sideChanged(bool guest)
{
    ui->lbCityLedger->setVisible(!guest);
    ui->leCLCode->setVisible(!guest);
    ui->leCLName->setVisible(!guest);
    ui->lbGuest->setVisible(guest);
    ui->lbRoom->setVisible(guest);
    ui->leGuest->setVisible(guest);
    ui->leRoomCode->setVisible(guest);
    ui->leRoomName->setVisible(guest);
    ui->rbPercent->setEnabled(guest);
    ui->gbGuest->setEnabled(guest);
    ui->rbCompany->setEnabled(guest);
    adjustSize();
}

void DlgDiscount::on_rbPercent_clicked()
{
    ui->lbType->setText("%");
    ui->leValue->setValidator(new QDoubleValidator(0, 100, 2));
    if (ui->leValue->asDouble() > 100.0001) {
        ui->leValue->setDouble(100);
    }
    countDiscount();
}

void DlgDiscount::on_gbGuest_clicked()
{
    ui->leAmount->setDouble(fGuestAmount);
    countDiscount();
}

void DlgDiscount::on_rbCompany_clicked()
{
    ui->leAmount->setDouble(fCompanyAmount);
    countDiscount();
}

void DlgDiscount::on_btnOk_clicked()
{
    if (ui->leValue->asDouble() < 0.01) {
        message_error_tr("Empty discount value");
        return;
    }
    if (ui->leType->fHiddenText.toInt() == 0) {
        message_error_tr("Discount type not defined");
        return;
    }
    if (ui->leFinalAmount->asDouble() < -0.001 && ui->rbGuest->isChecked()) {
        message_error_tr("You cannot to do this :)");
        return;
    }
    bool isNew = true;
    fDb.fDb.transaction();
    QString rid = uuuid(VAUCHER_DISCOUNT_N, fAirDb);
    fDb.insertId("m_register", rid);
    ui->leVaucher->setText(rid);
    fDbBind[":f_source"] = VAUCHER_DISCOUNT_N;
    if (isNew) {
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
    }
    fDbBind[":f_room"] = ui->rbGuest->isChecked() ?  ui->leRoomCode->text() : ui->leCLCode->text();
    fDbBind[":f_guest"] = ui->rbGuest->isChecked() ? ui->leGuest->text() : ui->leCLName->text();
    fDbBind[":f_itemCode"] = fPreferences.getDb(def_receip_vaucher_id);
    fDbBind[":f_finalName"] = tr("DISCOUNT S/N ") + ui->leVaucher->text();
    fDbBind[":f_amountAmd"] = ui->leDiscountAmount->asDouble();
    fDbBind[":f_amountVat"] = 0;
    fDbBind[":f_amountUsd"] = def_usd;
    fDbBind[":f_fiscal"] = 0;
    fDbBind[":f_paymentMode"] = PAYMENT_DISCOUNT;
    fDbBind[":f_creditCard"] = 0;
    fDbBind[":f_cityLedger"] = ui->leCLCode->asInt();
    fDbBind[":f_paymentComment"] = "";
    fDbBind[":f_dc"] = ui->rbGuest->isChecked() ? "CREDIT" : "DEBIT";
    fDbBind[":f_sign"] = ui->rbGuest->isChecked() ? -1 : 1;
    fDbBind[":f_doc"] = "";
    fDbBind[":f_rec"] = "";
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDbBind[":f_finance"] = 1;
    fDbBind[":f_canceled"] = 0;
    fDbBind[":f_cancelReason"] = "";
    if (ui->rbAmount->isChecked()) {
        fDbBind[":f_remarks"] = QString("Discount, %1AMD").arg(ui->leDiscountAmount->asDouble());
    } else {
        fDbBind[":f_remarks"] = QString("Discount, %1% (%2AMD)")
                .arg(ui->leValue->asDouble())
                .arg(ui->leDiscountAmount->asDouble());
    }
    fDbBind[":f_side"] = 0;
    fDb.update("m_register", fDbBind, where_id(ap(ui->leVaucher->text())));
    fDb.fDb.commit();

    QString msg = isNew ? "New Discount" : "Discount modified";
    msg += " " + ui->leVaucher->text();
    QString value = ui->deDate->text() + "/" + ui->leRoomCode->text()
            + "/" + ui->leGuest->text() + "/" + ui->leAmount->text()
            + "/" + ui->leCLName->text() + "/" + ui->leType->text();
    fTrackControl->insert(msg, value, "");

    if (ui->rbAmount->isChecked()) {
        fTrackControl->insert("Discount", float_str(ui->leAmount->asDouble(), 2),
                              QString("%1AMD, Final amount: %2AMD").arg(ui->leValue->asDouble()).arg(ui->leFinalAmount->text()));
    } else {
        fTrackControl->insert("Discount", float_str(ui->leAmount->asDouble(), 2),
                              QString("%1%, Final amount: %2AMD").arg(ui->leValue->asDouble()).arg(ui->leFinalAmount->text()));
    }
    ui->btnOk->setEnabled(false);
    ui->btnPrint->setEnabled(true);
}

void DlgDiscount::on_rbCityLedger_clicked()
{
    sideChanged(false);
}

void DlgDiscount::on_rbGuest_clicked()
{
    sideChanged(true);
}

void DlgDiscount::on_btnPrint_clicked()
{
    if (ui->leVaucher->isEmpty()) {
        message_error_tr("Save first");
        return;
    }
    PPrintVaucher::printVaucher(ui->leVaucher->text());
}
