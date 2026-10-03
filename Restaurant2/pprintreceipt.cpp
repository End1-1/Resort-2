#include "pprintreceipt.h"
#include "c5printing.h"
#include "cacheusers.h"
#include "databaseresult.h"
#include "database2.h"
#include "message.h"
#include "paymentmode.h"
#include "restaurantc5print.h"
#include "trackcontrol.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

namespace {

void addLabelValue(C5Printing &doc, const QString &label, const QString &value)
{
    doc.ltext(label, 0, 32);
    doc.ltext(value, 32, 0);
    doc.br();
}

} // namespace

PPrintReceipt::PPrintReceipt(const QString &printerName, int number, int user)
    : Base()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_id"] = number;
    db2.exec("select * from o_header where f_id=:f_id");
    if(!db2.next()) {
        message_error(QObject::tr("Not valid order id"));
        return;
    }
    const int fiscalnumber = db2.integer("f_tax");
    const int orderstate = db2.integer("f_state");
    QString partnerTin;
    QJsonObject jh;
    if(fiscalnumber > 0) {
        db2[":f_fiscal"] = fiscalnumber;
        db2.exec("select * from o_tax_log where f_fiscal=:f_fiscal");
        if(!db2.next()) {
            message_error(QObject::tr("Not valid fiscal number"));
            return;
        }
        QJsonObject jo = QJsonDocument::fromJson(db2.string("f_in").toUtf8()).object();
        jh = QJsonDocument::fromJson(db2.string("f_out").toUtf8()).object();
        partnerTin = jo["partnerTin"].toString();
    }

    db2[":f_order"] = number;
    db2.exec("select * from o_car where f_order=:f_order");
    if(db2.next()) {
        const int costname = db2.integer("f_costumer");
        if(costname > 0) {
            db2[":f_id"] = costname;
            db2.exec("select * from o_debt_holder where f_id=:f_id");
        }
    }

    DatabaseResult drh;
    fDbBind[":f_id"] = number;
    drh.select(fDb, "select h.f_name as hname, t.f_name as tname, concat(u.f_firstName, ' ', u.f_lastName)  as staff,\
               oh.f_dateOpen, oh.f_dateClose, oh.f_dateCash, oh.f_total, oh.f_roomComment, oh.f_paymentMode, \
               oh.f_cityLedger, oh.f_paymentModeComment \
               from o_header oh \
               left join r_hall h on h.f_id=oh.f_hall \
               left join r_table t on t.f_id=oh.f_table \
               left join users u on u.f_id=oh.f_staff \
               where oh.f_id=:f_id ", fDbBind);

    if(drh.rowCount() == 0) {
        QMessageBox::warning(0, QObject::tr("Print receipt"), QObject::tr("Incorrect order number"));
        return;
    }

    DatabaseResult drd;
    fDbBind[":f_header"] = number;
    drd.select(fDb,
               "select od.f_state, d.f_" + def_lang + ", od.f_qty, od.f_price, od.f_total, od.f_complex \
               from o_dish od \
               left join r_dish d on d.f_id=od.f_dish \
               where od.f_header=:f_header and od.f_state in (1, 2, 3) \
               order by od.f_row ",
               fDbBind);

    if(printerName.isEmpty()) {
        return;
    }

    ReceiptPrinter printer(printerName);
    C5Printing doc;
    setupC5Printing(doc, printer.printer());

    doc.image("./logo_print.png", Qt::AlignHCenter);
    doc.br(4);

    doc.setFontSize(receiptFontPt(12));
    doc.setFontBold(true);
    doc.ctext(drh.value("hname").toString());
    doc.br();

    doc.setFontSize(receiptFontPt(10));
    doc.setFontBold(false);
    if(orderstate == ORDER_STATE_REMOVED) {
        doc.setFontBold(true);
        doc.ctext(QStringLiteral("ՉԵՂԱՐԿՎԱԾ"));
        doc.br();
        doc.setFontBold(false);
    }

    doc.ctext(QString("%1 %2").arg(QObject::tr("Receipt S/N ")).arg(number));
    doc.br();

    if(fiscalnumber > 0) {
        doc.ctext(jh["taxpayer"].toString());
        doc.br();
        addLabelValue(doc, QObject::tr("Taxpayer id"), jh["tin"].toString());
        addLabelValue(doc, QObject::tr("Device number"), jh["crn"].toString());
        addLabelValue(doc, QObject::tr("Serial"), jh["sn"].toString());
        addLabelValue(doc, QObject::tr("Fiscal"), jh["fiscal"].toString());
        addLabelValue(doc, QObject::tr("Receipt number"), jh["rseq"].toString());
        addLabelValue(doc, QObject::tr("Date"),
                       QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jh["time"].toDouble()))
                           .addSecs(3600 * 4)
                           .toString("dd.MM.yyyy HH:mm:ss"));
        doc.ltext(QObject::tr("(F)"), 32, 0);
        doc.br();
        if(!partnerTin.isEmpty()) {
            addLabelValue(doc, QObject::tr("Partner tin"), partnerTin);
        }
    }

    addLabelValue(doc, QObject::tr("Table"), drh.value("tname").toString());
    addLabelValue(doc, QObject::tr("Date"), drh.value("f_dateCash").toDate().toString(def_date_format));
    addLabelValue(doc, QObject::tr("Waiter"), drh.value("staff").toString());
    addLabelValue(doc, QObject::tr("Opened"), drh.value("f_dateOpen").toDateTime().toString(def_date_time_format));
    addLabelValue(doc, QObject::tr("Closed"), drh.value("f_dateClose").toDateTime().toString(def_date_time_format));

    doc.line();
    doc.br(2);
    doc.setFontBold(true);
    doc.ltext(QObject::tr("Qty"), 0, 14);
    doc.ltext(QObject::tr("Description"), 14, 50);
    doc.ltext(QObject::tr("Amount"), 64, 0);
    doc.br();
    doc.setFontBold(false);
    doc.line();
    doc.br(2);

    for(int i = 0; i < drd.rowCount(); i++) {
        if(drd.value(i, "f_state").toInt() != DISH_STATE_READY) {
            continue;
        }
        doc.ltext(float_str(drd.value(i, "f_qty").toDouble(), 1), 0, 14);
        doc.ltext(drd.value(i, "f_" + def_lang).toString(), 14, 50);
        doc.rtext(float_str(drd.value(i, "f_total").toDouble(), 2));
        doc.br();
    }

    doc.line();
    doc.br(2);
    doc.setFontBold(true);
    doc.ltext(QObject::tr("Total, AMD"), 0, 50);
    doc.rtext(float_str(drh.value("f_total").toDouble(), 2));
    doc.br();
    doc.setFontBold(false);
    doc.br(2);

    if(!drh.value("f_roomComment").toString().isEmpty()) {
        doc.ctext(drh.value("f_roomComment").toString());
        doc.br();
        doc.ctext(QObject::tr("Signature"));
        doc.br();
        doc.line();
        doc.br();
    }

    if(drh.value("f_paymentMode").toInt() == PAYMENT_COMPLIMENTARY) {
        doc.ctext(QObject::tr("COMPLIMENTARY"));
    } else {
        doc.ctext(QObject::tr("SALES"));
    }
    doc.br();
    doc.ctext(QObject::tr("Mode Of Payment"));
    doc.br();

    switch(drh.value("f_paymentMode").toInt()) {
    case PAYMENT_CASH:
        doc.ctext(QObject::tr("CASH") + "/" + drh.value("f_paymentComment").toString());
        break;
    case PAYMENT_CARD:
        doc.ctext(QObject::tr("CARD") + "/" + drh.value("f_paymentModeComment").toString());
        break;
    case PAYMENT_ROOM:
        doc.ctext(drh.value("f_paymentModeComment").toString());
        break;
    case PAYMENT_CL:
        doc.ctext("CL/" + drh.value("f_paymentModeComment").toString()
                  + "(" + drh.value("f_cityLedger").toString() + ")");
        break;
    case PAYMENT_COMPLIMENTARY:
        doc.ctext(drh.value("f_paymentModeComment").toString());
        break;
    default:
        break;
    }
    doc.br();

    bool voida = false;
    for(int i = 0; i < drd.rowCount(); i++) {
        const int st = drd.value(i, "f_state").toInt();
        if(st == DISH_STATE_REMOVED_STORE || st == DISH_STATE_REMOVED_NOSTORE) {
            voida = true;
            break;
        }
    }
    if(voida) {
        doc.br(4);
        doc.setFontBold(true);
        doc.ctext(QObject::tr("****VOID****"));
        doc.br();
        doc.setFontBold(false);
        for(int i = 0; i < drd.rowCount(); i++) {
            const int st = drd.value(i, "f_state").toInt();
            if(st != DISH_STATE_REMOVED_STORE && st != DISH_STATE_REMOVED_NOSTORE) {
                continue;
            }
            doc.ltext(float_str(drd.value(i, "f_qty").toDouble(), 1), 0, 14);
            doc.ltext(drd.value(i, "f_" + def_lang).toString(), 14, 50);
            doc.rtext(float_str(drd.value(i, "f_total").toDouble(), 2));
            doc.br();
        }
    }

    doc.br(2);
    doc.ctext("_");
    doc.br();
    doc.setFontSize(receiptFontPt(9));
    doc.ltext(QString("Printed %1").arg(QDateTime::currentDateTime().toString(def_date_time_format)), 0);
    doc.br();
    CI_User *u = CacheUsers::instance()->get(user);
    if(u) {
        doc.ltext(QString("By ") + u->fFull, 0);
        doc.br();
    }

    printC5(doc, printer.printer());

    fDbBind[":f_print"] = drh.value("f_print").toInt() + 1;
    fDb.update("o_header", fDbBind, where_id(ap(number)));
    TrackControl::insert(TRACK_REST_ORDER, "Print receipt", "", "", "", QString::number(number));
}

void PPrintReceipt::printOrder(const QString &printerName, int number, int user)
{
    PPrintReceipt(printerName, number, user);
}
