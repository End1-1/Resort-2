#include "dlgcityadvance.h"
#include "ui_dlgcityadvance.h"
#include "vauchers.h"
#include "voucher_tc.h"
#include "voucher_av.h"
#include "paymentmode.h"

#define SEL_CL 1

DlgCityAdvance::DlgCityAdvance(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgCityAdvance)
{
    ui->setupUi(this);
    fDockCL = new DWSelectorCityLedger(this);
    fDockCL->configure();
    fDockCL->setSelector(ui->leCityCode);
    connect(fDockCL, &DWSelectorCityLedger::cityLedger, [this](CI_CityLedger *ci) {
        dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCityCode, ui->leCityName, ci);
    });
    fDockAdvance = new DWSelectorCLAdvance(this);
    fDockAdvance->configure();
    fDockAdvance->setSelector(ui->leCityCode);
    fDockAdvance->setDialog(this, SEL_CL);
    fDockRoom = new DWSelectorActiveRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->leRoom);
    connect(fDockRoom, &DWSelectorActiveRoom::roomChanged, [this](CI_ActiveRoom *ci) {
        if (ci) {
            ui->leReservation->setText(ci->fCode);
            ui->leInvoice->setText(ci->fInvoice);
            ui->leRoom->setText(ci->fRoomCode);
            ui->leRoomName->setText(ci->fRoomName);
            ui->leGuest->setText(ci->fGuestName);
        } else {
            ui->leReservation->clear();
            ui->leInvoice->clear();
            ui->leRoom->clear();
            ui->leRoomName->clear();
            ui->leGuest->clear();
        }
    });
    fDockRed = new DWSelectorRedReservation(this);
    fDockRed->configure();
    connect(fDockRed, &DWSelectorRedReservation::redReservation, [this](CI_RedReservation *ci) {
        if (ci) {
            ui->leInvoice->setText(ci->fInvoice);
            ui->leReservation->setText(ci->fCode);
            ui->leRoom->setText(ci->fRoom);
            ui->leGuest->setText(ci->fName);
            ui->leAvaiableAmount->setText(ci->fBalance.toDouble());
        } else {
            ui->leInvoice->clear();
            ui->leReservation->clear();
            ui->leRoom->clear();
            ui->leGuest->clear();
            ui->leAvaiableAmount->clear();
        }
    });

    fDockDstRoom = new DWSelectorActiveRoom(this);
    fDockDstRoom->configure();
    fDockDstRoom->setSelector(ui->leDstRoomCode);
    connect(fDockDstRoom, &DWSelectorActiveRoom::roomChanged, [this](CI_ActiveRoom *ci) {
       dockResponse<CI_ActiveRoom, CacheActiveRoom>(ui->leDstRoomCode, ui->leDstRoomName, ci);
       if (ci) {
           ui->leDstRoomCode->setText(ci->fRoomCode);
           ui->leDstInvoice->setText(ci->fInvoice);
           ui->leDstReserv->setText(ci->fCode);
           ui->leDstGuest->setText(ci->fGuestName);
       }
    });

    ui->rbToCityLedger->click();
}

DlgCityAdvance::~DlgCityAdvance()
{
    delete ui;
}

void DlgCityAdvance::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_CL: {
        CI_CLAdvance *ci = value.value<CI_CLAdvance*>();
        dockResponse<CI_CLAdvance, CacheCLAdvance>(ui->leCityCode, ui->leCityName, ci);
        if (ci) {
            ui->leAmount->setText(ci->fAmount);
        }
        }
    }
}

void DlgCityAdvance::cityAdvance(const QString &id, const QString &invoice, int mode)
{
    DlgCityAdvance *d = new DlgCityAdvance(fPreferences.getDefaultParentForMessage());
    if (id.length() > 0) {
        d->load(id);
    } else {
        if (invoice.length() > 0) {
            d->loadInvoice(invoice);
        }
    }
    switch (mode) {
    case 0:
        d->fDockCL->setSelector(0);
        d->fDockAdvance->setSelector(0);
        break;
    case 1:
        d->ui->rbToCityLedger->setChecked(true);
        d->ui->rbToReservation->setEnabled(false);
        d->fDockCL->setSelector(d->ui->leCityCode);
        d->fDockAdvance->setSelector(0);
        break;
    case 2:
        d->ui->rbToReservation->setEnabled(true);
        d->ui->rbToCityLedger->setEnabled(false);
        d->ui->leRoom->setEnabled(true);
        d->fDockCL->setSelector(0);
        d->fDockAdvance->setSelector(d->ui->leCityCode);
        break;
    }
    d->exec();
    delete d;
}

void DlgCityAdvance::on_btnCancel_clicked()
{
    reject();
}

void DlgCityAdvance::load(const QString &id)
{
    voucher_tc v;
    if (!v.load(id)) {
        message_error(tr("Cannot load voucher"));
        return;
    }
    ui->leCode->setText(v.get("f_id").toString());
    ui->deDate->setDate(v.get("f_wdate").toDate());
    ui->teTime->setTime(v.get("f_time").toTime());
    ui->leInvoice->setText(v.get("f_inv").toString());
    ui->leReservation->setText(v.get("f_res").toString());
    ui->leRoom->setText(v.get("f_room").toString());
    ui->leGuest->setText(v.get("f_guest").toString());
    ui->leCityCode->setText(v.get("f_cityLedger").toString());
    dockResponse<CI_CityLedger, CacheCityLedger>(ui->leCityCode, ui->leCityName, 0);
    ui->leAmount->setText(v.get("f_amountAmd").toDouble());
    ui->btnPrint->setEnabled(true);
    loadInvoice(ui->leInvoice->text());
    ui->rbToCityLedger->setChecked(v.get("f_sign").toInt() == -1);
    ui->rbToCityLedger->setEnabled(false);
    ui->rbToReservation->setEnabled(false);
}

void DlgCityAdvance::loadInvoice(const QString &invoice)
{
    DatabaseResult dr;
    fDbBind[":f_invoice"] = invoice;
    dr.select(fDb, "select r.f_id, r.f_room, g.guest from f_reservation r "
                   "left join guests g on r.f_guest=g.f_id "
                   "where r.f_invoice=:f_invoice", fDbBind);
    if (dr.rowCount() > 0) {
        ui->leReservation->setText(dr.value("f_id").toString());
        ui->leInvoice->setText(invoice);
        ui->leRoom->setText(dr.value("f_room").toString());
        ui->leGuest->setText(dr.value("guest").toString());
    }
    fDbBind[":f_inv"] = invoice;
    dr.select(fDb, "select sum(f_amountAmd * f_sign) as amount from m_register where f_inv=:f_inv and f_finance=1 and f_canceled=0", fDbBind);
    if (dr.rowCount() > 0) {
        double v = dr.value("amount").toDouble();
        if (v > 0.1) {
            v = 0;
        }
        v = abs(v);
        ui->leAvaiableAmount->setText(v + ui->leAmount->asDouble());
    }
}

void DlgCityAdvance::on_btnSave_clicked()
{
    if (ui->leCityCode->asInt() == 0) {
        if (ui->rbToCityLedger->isChecked() || ui->rbToReservation->isChecked()) {
            message_error(tr("City ledger not selected"));
            return;
        }
    }
    if (ui->leAmount->asDouble() < 0.1) {
        message_error(tr("Amount not selected"));
        return;
    }
    if (ui->leAvaiableAmount->asDouble() > 0.1) {
        if (ui->leAmount->asDouble() > ui->leAvaiableAmount->asDouble()) {
            message_error_tr("Amount greater than available amount");
            return;
        }
    }
    if (ui->rbAdvanceToRoom->isChecked()) {
        if (ui->leDstRoomCode->asInt() == 0) {
            message_error(tr("Destination room must be selected"));
            return;
        }
    }
    QString trackAction;
    QString fromTo;
    voucher_tc bv;
    if (ui->leCode->isEmpty()) {
        ui->leCode->setText(bv.genId(VAUCHER_TRANSFER_CL_N));
    }
    QString finalName;
    int rb = 0;
    if (ui->rbToCityLedger->isChecked()) {
        finalName = tr("TRANSFER TO C/L") + " " + ui->leCityName->text();
        rb = 0;
        trackAction = "Transfer from room to CL";
        fromTo = QString("%1/%2 -> %3%4, %5")
                .arg(ui->leCityCode->text())
                .arg(ui->leCityName->text())
                .arg(ui->leRoom->text())
                .arg(ui->leGuest->text())
                .arg(ui->leAmount->text());
    }
    if (ui->rbToReservation->isChecked()) {
        finalName = tr("TRANSFER TO") + " " + ui->leRoom->text() + "," + ui->leGuest->text();
        rb = 1;
        trackAction = "Transfer from CL to room";
        fromTo = QString("%1/%2 -> %3%4, %5")
                .arg(ui->leRoom->text())
                .arg(ui->leGuest->text())
                .arg(ui->leCityCode->text())
                .arg(ui->leCityName->text())
                .arg(ui->leAmount->text());
    }
    if (ui->rbAdvanceToRoom->isChecked()) {
        finalName = tr("TRANSFER TO") + " " + ui->leDstRoomCode->text() + "," + ui->leDstGuest->text();
        rb = 2;
        trackAction = "Transfer from advance to room";
        fromTo = QString("%1/%2/%3 -> %4%5, %6")
                .arg(ui->leInvoice->text())
                .arg(ui->leRoom->text())
                .arg(ui->leGuest->text())
                .arg(ui->leDstRoomCode->text())
                .arg(ui->leDstGuest->text())
                .arg(ui->leAmount->text());
    }
    int sign = -1;
    if (ui->rbToCityLedger->isChecked()) {
        sign = 1;
    }
    if (ui->rbToReservation->isChecked()) {
        sign = -1;
    }
    QString room = ui->leRoom->text();
    QString invoice = ui->leInvoice->text();
    QString reserve = ui->leReservation->text();
    QString guest = ui->leGuest->text();
    if (ui->rbAdvanceToRoom->isChecked()) {
        room = ui->leDstRoomCode->text();
        invoice = ui->leDstInvoice->text();
        reserve = ui->leDstReserv->text();
        guest = ui->leDstGuest->text();
    }
    bv.set("f_res", reserve)
            .set("f_wdate", WORKING_DATE)
            .set("f_rdate", QDate::currentDate())
            .set("f_time", QTime::currentTime())
            .set("f_user", WORKING_USERID)
            .set("f_room", room)
            .set("f_guest", guest)
            .set("f_itemCode", VAUCHER_TRANSFER_CL)
            .set("f_finalName", finalName)
            .set("f_amountAmd", ui->leAmount->asDouble())
            .set("f_usedPrepaid", 0)
            .set("f_amountVat", 0)
            .set("f_amountUsd", def_usd)
            .set("f_fiscal", 0)
            .set("f_paymentMode", PAYMENT_TRANSFER)
            .set("f_creditCard", 0)
            .set("f_cityLedger", ui->leCityCode->asInt())
            .set("f_paymentComment", "")
            .set("f_dc", "CREDIT")
            .set("f_sign", sign)
            .set("f_inv", invoice)
            .set("f_finance", 1)
            .set("f_remarks", "")
            .set("f_side", 1)
            .set("f_canceled", 0)
            .set("f_cancelReason", "")
            .set("f_cancelDate", QDate::currentDate())
            .set("f_cancelUser", 0)
            .set("p", 0)
            .set("f_usedTaxId", 0)
            .set("f_rb", rb);
    bv.saveVoucher();
    if (ui->rbAdvanceToRoom->isChecked()) {
        voucher_av av;
        reserve = ui->leReservation->text();
        guest = ui->leGuest->text();
        room = ui->leRoom->text();
        invoice = ui->leInvoice->text();
        if (av.get("f_id").toInt() == 0) {
            av.genId(VAUCHER_ADVANCE_N);
        }
        av.set("f_res", reserve)
                .set("f_wdate", WORKING_DATE)
                .set("f_rdate", QDate::currentDate())
                .set("f_time", QTime::currentTime())
                .set("f_user", WORKING_USERID)
                .set("f_room", room)
                .set("f_guest", guest)
                .set("f_itemCode", fPreferences.getDb(def_advance_voucher_id))
                .set("f_finalName", finalName)
                .set("f_amountAmd", ui->leAmount->asDouble())
                .set("f_usedPrepaid", 0)
                .set("f_amountVat", 0)
                .set("f_amountUsd", def_usd)
                .set("f_fiscal", 0)
                .set("f_paymentMode", PAYMENT_TRANSFER)
                .set("f_creditCard", 0)
                .set("f_cityLedger", ui->leCityCode->asInt())
                .set("f_paymentComment", "")
                .set("f_dc", "DEBIT")
                .set("f_sign", 1)
                .set("f_inv", invoice)
                .set("f_doc", 0)
                .set("f_finance", 1)
                .set("f_remarks", "")
                .set("f_side", 0)
                .set("f_canceled", 0)
                .set("f_cancelReason", "")
                .set("f_cancelDate", QDate::currentDate())
                .set("f_cancelUser", 0)
                .set("p", 0)
                .set("f_usedTaxId", 0)
                .set("f_rb", rb);
        av.saveVoucher();
        fDbBind[":f_doc"] = av.get("f_id");
        fDb.update("m_register", fDbBind, where_id(ap(ui->leCode->text())));
        TrackControl::insert(TRACK_RESERVATION, trackAction, fromTo, "", ui->leCode->text(), ui->leInvoice->text(), ui->leReservation->text());
        TrackControl::insert(TRACK_RESERVATION, trackAction, fromTo, "", av.get("f_id").toString(), ui->leDstInvoice->text(), ui->leDstReserv->text());
    } else {
        TrackControl::insert(TRACK_RESERVATION, trackAction, fromTo, "", ui->leCode->text(), ui->leInvoice->text(), ui->leReservation->text());
    }
    ui->btnPrint->setEnabled(true);
}

void DlgCityAdvance::on_btnPrint_clicked()
{
    voucher_tc::print(ui->leCode->text());
}

void DlgCityAdvance::on_rbToCityLedger_clicked()
{
    ui->leRoom->setEnabled(false);
    ui->leReservation->setEnabled(false);
    fDockCL->setSelector(ui->leCityCode);
    fDockAdvance->setSelector(0);
    fDockRed->setSelector(0);
    ui->wdtCityLedger->setVisible(true);
    ui->wdtDestRoom->setVisible(false);
    adjustSize();
}

void DlgCityAdvance::on_rbToReservation_clicked()
{
    ui->leRoom->setEnabled(true);
    ui->leReservation->setEnabled(false);
    fDockCL->setSelector(0);
    fDockAdvance->setSelector(ui->leCityCode);
    fDockRed->setSelector(0);
    ui->wdtCityLedger->setVisible(true);
    ui->wdtDestRoom->setVisible(false);
    adjustSize();
}

void DlgCityAdvance::on_rbAdvanceToRoom_clicked()
{
    ui->leRoom->setEnabled(false);
    fDockCL->setSelector(0);
    fDockAdvance->setSelector(0);
    fDockRed->setSelector(ui->leReservation);
    ui->leReservation->setEnabled(true);
    ui->wdtCityLedger->setVisible(false);
    ui->wdtDestRoom->setVisible(true);
    adjustSize();
}
