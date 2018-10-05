#include "vauchers.h"
#include "preferences.h"
#include "dlgpostingcharges.h"
#include "basewidget.h"
#include "dlggposorderinfo.h"
#include "dlgreceiptvaucher.h"
#include "wreservation.h"
#include "winvoice.h"
#include "waccinvoice.h"
#include "dlgadvance.h"
#include "dlgclinitialbalance.h"
#include "dlgdiscount.h"
#include "dlgcityadvance.h"
#include "paymentmode.h"

QString vaucherPaymentName(int code, const QString &cardcode, const QString &clcode) {
    QString payment = "";
    switch (code) {
    case PAYMENT_CASH:
        payment = "CASH";
        break;
    case PAYMENT_CARD:
        if (cardcode.toInt() > 0) {
            payment = CacheCreditCard::instance()->get(cardcode)->fName;
        }
        break;
    case PAYMENT_BANK:
        payment = "BANK";
        break;
    case PAYMENT_CL:
        payment = CacheCityLedger::instance()->get(clcode)->fName;
        break;
    case PAYMENT_CREDIT:
        payment = "CREDIT";
        break;
    default:
        payment = "UNKNOWN";
        break;
    }
    return payment;
}

QString vaucherCode(int code, const QString &prefix)
{
    Preferences p;
    return QString("%1%2").arg(prefix).arg(code, p.getDb(def_vouchers_digits).toInt(), 10, QChar('0'));
}

bool isTaxPay(const QString &code)
{
    return code == VAUCHER_ROOMING_N
            || code == VAUCHER_POSTCHARGE_N
            || code == VAUCHER_POINT_SALE_N
            || code == "CM"
            || code == "PE";
}

bool isTaxPrepay(const QString &code)
{
    return code == VAUCHER_RECEIPT_N
            || VAUCHER_ADVANCE_N;
}

bool openVaucherInvoice(const QString &vaucherId) {
    Database &db = fMainWindow->fDb;
    QMap<QString, QVariant> &v = fMainWindow->fDbBind;
    QList<QList<QVariant> > r = fMainWindow->fDbRows;
    v[":f_id"] = vaucherId;
    db.select("select f_inv from m_register where f_id=:f_id", v, r);
    if (r.count() == 0) {
        message_error_tr("Cannot open invoice");
        return false;
    }
    openInvoiceWithId(r.at(0).at(0).toString());
    return true;
}

void openVaucher(const QString &vaucher, const QString &id)
{
    if (vaucher == "RM" || vaucher == "CM" || vaucher == "CH" || vaucher == "PE") {
        DlgPostingCharges *d = new DlgPostingCharges(fMainWindow->fPreferences.getDefaultParentForMessage());
        d->loadVaucher(id);
        d->exec();
        delete d;
    } else if (vaucher == "PS") {
        DlgGPOSOrderInfo *d = new DlgGPOSOrderInfo(fMainWindow->fPreferences.getDefaultParentForMessage());
        d->setVaucher(id);
        d->exec();
        delete d;
    } else if (vaucher == "RV") {
        DlgReceiptVaucher *d = new DlgReceiptVaucher(fMainWindow->fPreferences.getDefaultParentForMessage());
        d->setVaucher(id);
        d->exec();
        delete d;
    } else if (vaucher == "RS") {
        WReservation::openVaucher(id);
    } else if (vaucher == "AV") {
        DlgAdvance::openAdvance(id);
    } else if (vaucher == "CR") {
        DlgCLInitialBalance::openVaucher(id);
    } else if (vaucher == "DS") {
        DlgDiscount::openVaucher(id);
    } else if (vaucher == "TC") {
        DlgCityAdvance::cityAdvance(id, "", 0);
    }
}

bool removeVaucher(const QString &id, const QString &reason)
{
    Database &db = fMainWindow->fDb;
    QMap<QString, QVariant> &v = fMainWindow->fDbBind;
    QList<QList<QVariant> > &r = fMainWindow->fDbRows;
    v[":f_id"] = id;
    QString src, name;
    int doc, rec, item, fiscal;
    QString f_inv;
    int track = TRACK_VAUCHER;
    db.select("select f_source, f_doc, f_rec, f_itemCode, f_finalName, f_amountAmd, f_fiscal, f_inv "
              "from m_register where f_id=:f_id", v, r);
    if (r.count()) {
        src = r.at(0).at(0).toString();
        doc = r.at(0).at(1).toInt();
        rec = r.at(0).at(2).toInt();
        item = r.at(0).at(3).toInt();
        name = r.at(0).at(4).toString();
        //amount = r.at(0).at(5).toDouble();
        fiscal = r.at(0).at(6).toInt();
        f_inv = r.at(0).at(7).toString();
    } else {
        return false;
    }
    if (src == "PS") {
        v[":f_state"] = ORDER_STATE_REMOVED;
        db.update("o_header", v, where_id(ap(id)));
        v[":f_state"] = DISH_STATE_REMOVED_NOSTORE;
        v[":f_state1"] = DISH_STATE_READY;
        v[":f_header"] = id;
        db.select("update o_dish set f_state=:f_state where f_header=:f_header and f_state=:f_state1 ", v, r);
    } else if (src == "PE") {
        v[":f_canceled"] = 1;
        v[":f_cancelUser"] = fMainWindow->fPreferences.getDb(def_working_user_id).toInt();
        v[":f_cancelDate"] = QDateTime::currentDateTime();
        v[":f_cancelReason"] = reason;
        db.update("m_register", v, where_id(doc));
    } else if (src == "CH") {
        if (item == 17) {
            v[":f_show"] = 0;
            v[":f_doc"] = 0;
            db.update("f_reservation_meal", v, where_field("f_doc", doc));
        }
    } else if (src == "RV" || src == "AV") {
        track = TRACK_RESERVATION;
        if (fiscal > 0) {
            if (!f_inv.isEmpty()) {
                // IN FUTURE: UPDATE PREPAID
            }
        }
    }
    Preferences p;
    v[":f_canceled"] = 1;
    v[":f_cancelUser"] = p.getLocal(def_working_user_id).toInt();
    v[":f_cancelDate"] = QDateTime::currentDateTime();
    v[":f_cancelReason"] = reason;
    db.update("m_register", v, where_id(ap(id)));

    TrackControl::insert(track, "VAUCHER CANCELED", QString("%1%2 %3")
                  .arg(src)
                  .arg(doc)
                  .arg(name), "", id, f_inv);
    Q_UNUSED(rec)
    return true;
}

bool openInvoiceWithId(const QString &invoice)
{
    Database &db = fMainWindow->fDb;
    QMap<QString, QVariant> &v = fMainWindow->fDbBind;
    QList<QList<QVariant> > r = fMainWindow->fDbRows;
    v[":f_invoice"] = invoice;
    db.select("select f_state from f_reservation where f_invoice=:f_invoice", v, r);
    if (r.count() == 0) {
        message_error_tr("Cannot open invoice");
        return false;
    }
    int state = r.at(0).at(0).toInt();
    switch (state) {
    case RESERVE_CHECKIN:
        WInvoice::openInvoiceWindow(invoice);
        break;
    case RESERVE_CHECKOUT:
    case RESERVE_REMOVED: {
        WAccInvoice *ai = addTab<WAccInvoice>();
        ai->load(invoice);
        break;
    }
    }
    return true;
}
