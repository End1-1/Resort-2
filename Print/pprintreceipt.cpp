#include "pprintreceipt.h"
#include "databaseresult.h"
#include "pprintscene.h"
#include "trackcontrol.h"
#include "ptextrect.h"
#include "pimage.h"
#include "message.h"
#include "paymentmode.h"
#include "cacheusers.h"
#include "printtaxn.h"
#include "database2.h"
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QJsonObject>
#include <QJsonDocument>

PPrintReceipt::PPrintReceipt(const QString &printerName, const QString &number, int user) :
    Base()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":f_id"] = number;
    db2.exec("select f_tax from o_header where f_id=:f_id");
    if (!db2.next()) {
        message_error(QObject::tr("Not valid order id"));
        return;
    }
    int fiscalnumber = db2.integer("f_tax");
    QString sn, firm, address, fiscal, hvhh, rseq, devnum, time;

    db2[":f_fiscal"] = fiscalnumber;
    db2.exec("select * from o_tax_log where f_fiscal=:f_fiscal");
    if (!db2.next()) {
        message_error(QObject::tr("Not valid fiscal number"));
        return;
    }
    PrintTaxN::parseResponse(db2.string("f_out"), firm, hvhh, fiscal, rseq, sn, address, devnum, time);
    QJsonObject jo = QJsonDocument::fromJson(db2.string("f_in").toUtf8()).object();
    QString partnerTin = jo["partnerTin"].toString();


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

    if (drh.rowCount() == 0) {
        QMessageBox::warning(0, QObject::tr("Print receipt"), QObject::tr("Incorrect order number"));
        return;
    }

    DatabaseResult drd;
    fDbBind[":f_header"] = number;
    drd.select(fDb, "select od.f_state, d.f_" + def_lang + ", od.f_qty, od.f_price, od.f_total, od.f_complex \
               from o_dish od \
               left join r_dish d on d.f_id=od.f_dish \
               where od.f_header=:f_header and od.f_state in (1, 2, 3) \
               and (od.f_complex=0 or (od.f_complexId=od.f_complex and od.f_complex>0)) \
               order by od.f_row ", fDbBind);


    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
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
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight + 10, drh.value("hname").toString(), &th, f))->textHeight();
    top += 20;
    f.setPointSize(30);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QString("%1 %2")
                                     .arg(QObject::tr("Receipt S/N "))
                                     .arg(number),
                                     &th, f))->textHeight();
    th.setTextAlignment(Qt::AlignLeft);
    if (fiscalnumber > 0) {
        top += ps->addTextRect(new PTextRect(10, top, 200, rowHeight, firm, &th, f))->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Taxpayer id"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, hvhh, &th, f))
                ->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Device number"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, devnum, &th, f))
                ->textHeight();ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Serial"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, sn, &th, f))
                ->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Fiscal"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, fiscal, &th, f))
                ->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Receipt number"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, rseq, &th, f))
                ->textHeight();
        ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Date"), &th, f));
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, time, &th, f))
                ->textHeight();
        top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, QObject::tr("(F)"), &th, f))
                ->textHeight();
        if (!partnerTin.isEmpty()) {
            ps->addTextRect(new PTextRect(10, top, 200, rowHeight, QObject::tr("Partner tin"), &th, f));
            top += ps->addTextRect(new PTextRect(210, top, 450, rowHeight, partnerTin, &th, f))
                    ->textHeight();
        }
    }

    f.setPointSize(24);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignLeft);
    ps->addTextRect(new PTextRect(10, top, 150, rowHeight, QObject::tr("Table"), &th, f));
    ps->addTextRect(new PTextRect(160, top, 200, rowHeight, drh.value("tname").toString(), &th, f));
    ps->addTextRect(new PTextRect(340, top, 230, rowHeight, QObject::tr("Date"), &th, f));
    top += ps->addTextRect(new PTextRect(450, top, 250, rowHeight, drh.value("f_dateCash").toDate().toString(def_date_format), &th, f))->textHeight();
    /*
    ps->addTextRect(new PTextRect(10, top, 150, rowHeight, QObject::tr("Time"), &th, f));
    top += ps->addTextRect(new PTextRect(160, top, 200, rowHeight, QTime::currentTime().toString(def_time_format), &th, f))->textHeight();
    */
    ps->addTextRect(new PTextRect(10, top, 150, rowHeight, QObject::tr("Waiter"), &th, f));
    top += ps->addTextRect(new PTextRect(160, top, 500, rowHeight, drh.value("staff").toString(), &th, f))->textHeight();

    ps->addTextRect(new PTextRect(10, top, 150, rowHeight, QObject::tr("Opened"), &th, f));
    top += ps->addTextRect(new PTextRect(160, top, 350, rowHeight, drh.value("f_dateOpen").toDateTime().toString(def_date_time_format), &th, f))->textHeight();
    ps->addTextRect(new PTextRect(10, top, 150, rowHeight, QObject::tr("Closed"), &th, f));
    top += ps->addTextRect(new PTextRect(160, top, 350, rowHeight, drh.value("f_dateClose").toDateTime().toString(def_date_time_format), &th, f))->textHeight();

    top += 2;
    ps->addLine(10, top, 680, top);
    top ++;
    ps->addTextRect(new PTextRect(10, top, 100, rowHeight, QObject::tr("Qty"), &th, f));
    ps->addTextRect(new PTextRect(110, top, 390, rowHeight, QObject::tr("Description"), &th, f));
    top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, QObject::tr("Amount"), &th, f))->textHeight();
    ps->addLine(10, top, 680, top);
    top ++;
    f.setPointSize(18);
    f.setBold(true);
    th.setFont(f);

    for (int i = 0; i < drd.rowCount(); i++) {
        if (drd.value(i, "f_state").toInt() != DISH_STATE_READY) {
            continue;
        }
        ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(drd.value(i, "f_qty").toDouble(), 1), &th, f));
        ps->addTextRect(new PTextRect(110, top, 390, rowHeight, drd.value(i, "f_" + def_lang).toString(), &th, f));
        top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(drd.value(i, "f_total").toDouble(), 2), &th, f))->textHeight();
        if (top > sizePortrait.height()  - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    ps->addLine(10, top, 680, top);
    top += 2;
    f.setPointSize(24);
    th.setFont(f);
    ps->addTextRect(new PTextRect(10, top, 400, rowHeight, QObject::tr("Total, AMD"), &th, f));
    top += ps->addTextRect(new PTextRect(500, top, 200, rowHeight, float_str(drh.value("f_total").toDouble(), 2), &th, f))->textHeight();

    top += rowHeight;
    f.setPointSize(28);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignHCenter);

    if (top > sizePortrait.height()  - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    if (!drh.value("f_roomComment").toString().isEmpty()) {
       top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, drh.value("f_roomComment").toString(), &th, f))->textHeight();
       top += rowHeight;
       top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QObject::tr("Signature"), &th, f))->textHeight();
       top += rowHeight + 2;
       ps->addLine(150, top, 680, top);
    }

    if (top > sizePortrait.height()  - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    if (drh.value("f_paymentMode").toInt() == PAYMENT_COMPLIMENTARY) {
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QObject::tr("COMPLIMENTARY"), &th, f))->textHeight();
    } else {
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QObject::tr("SALES"), &th, f))->textHeight();
    }

    top += rowHeight;
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QObject::tr("Mode Of Payment"), &th, f))->textHeight();
    switch (drh.value("f_paymentMode").toInt()) {
    case PAYMENT_CASH:
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QObject::tr("CASH") + "/" + drh.value("f_paymentComment").toString(), &th, f))->textHeight();
        break;
    case PAYMENT_CARD:
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, QObject::tr("CARD") + "/" + drh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
        break;
    case PAYMENT_ROOM:
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, drh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
        break;
    case PAYMENT_CL:
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, "CL/" +
                                             drh.value("f_paymentModeComment").toString() +
                                             "(" + drh.value("f_cityLedger").toString() + ")", &th, f))->textHeight();
        break;
    case PAYMENT_COMPLIMENTARY:
        top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, drh.value("f_paymentModeComment").toString(), &th, f))->textHeight();
        break;
    }

    top += 10;
    f.setPointSize(28);
    f.setBold(true);
    th.setFont(f);

    bool voida = false;
    for (int i = 0; i < drd.rowCount(); i++) {
        if (drd.value(i, "f_state").toInt() != DISH_STATE_REMOVED_STORE) {
            continue;
        }
        voida = true;
    }
    if (voida) {
        top += (rowHeight * 3);
        top += ps->addTextRect(10, top, 600, rowHeight, QObject::tr("****VOID****"), &th)->textHeight();
        for (int i = 0; i < drd.rowCount(); i++) {
            if (drd.value(i, "f_state").toInt() != DISH_STATE_REMOVED_STORE) {
                continue;
            }
            ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(drd.value(i, "f_qty").toDouble(), 1), &th, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, drd.value(i, "f_" + def_lang).toString(), &th, f));
            top += ps->addTextRect(new PTextRect(400, top, 200, rowHeight, float_str(drd.value(i, "f_total").toDouble(), 2), &th, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
    }

    bool mistakea = false;
    for (int i = 0; i < drd.rowCount(); i++) {
        if (drd.value(i, "f_state").toInt() != DISH_STATE_REMOVED_STORE) {
            continue;
        }
        mistakea = true;
    }
    if (mistakea) {
        top += rowHeight;
        top += ps->addTextRect(10, top, 600, rowHeight, QObject::tr("****MISTAKE****"), &th)->textHeight();
        for (int i = 0; i < drd.rowCount(); i++) {
            if (drd.value(i, "f_state").toInt() != DISH_STATE_REMOVED_NOSTORE) {
                continue;
            }
            ps->addTextRect(new PTextRect(10, top, 100, rowHeight, float_str(drd.value(i, "f_qty").toDouble(), 1), &th, f));
            ps->addTextRect(new PTextRect(110, top, 390, rowHeight, drd.value(i, "f_" + def_lang).toString(), &th, f));
            top += ps->addTextRect(new PTextRect(400, top, 200, rowHeight, float_str(drd.value(i, "f_total").toDouble(), 2), &th, f))->textHeight();
            if (top > sizePortrait.height()  - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
        }
    }

    //Finish
    top += rowHeight;
    ps->addTextRect(new PTextRect(10, top, 680, rowHeight, "_", &th, f));

    top += 10;
    f.setPointSize(18);
    f.setBold(false);
    th.setFont(f);
    top += ps->addTextRect(10, top, 680, rowHeight, QString("Printed %1").arg(QDateTime::currentDateTime().toString(def_date_time_format)), &th)->textHeight();
    CI_User *u = CacheUsers::instance()->get(user);
    if (u) {
        top += ps->addTextRect(10, top, 680, rowHeight, QString("By ") + u->fFull, &th)->textHeight();
    }

    QPrinter printer;
    printer.setPrinterName(printerName.toUpper());
//    QPrintDialog pd(&printer);
//    Q_UNUSED(pd);
//    QPrinterInfo pi(printer);
//    QList<int> res = pi.supportedResolutions();
//    int maxRes = 1200;
//    foreach (int r , res) {
//        maxRes = maxRes > r ? r : maxRes;
//        qDebug() << r << "/" << printer.resolution();
//    }
//    //printer.setResolution(maxRes);
    QMatrix m;
#ifdef QT_DEBUG
    m.scale(1, 1);
#else
    m.scale(3, 3);
#endif
    QPainter painter(&printer);
    painter.setMatrix(m);
    for (int i = 0; i < lps.count(); i++) {
        if (i > 0) {
            printer.newPage();
        }
        lps[i]->render(&painter);
    }

    fDbBind[":f_print"] = drh.value("f_print").toInt() + 1;
    fDb.update("o_header", fDbBind, where_id(ap(number)));
    TrackControl::insert(TRACK_REST_ORDER, "Print receipt", "", "", "", number);
}

void PPrintReceipt::printOrder(const QString &printerName, const QString &number, int user)
{
    PPrintReceipt(printerName, number, user);
}
