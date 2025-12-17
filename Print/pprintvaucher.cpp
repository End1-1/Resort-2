#include "pprintvaucher.h"
#include "pprintpreview.h"
#include "ptextrect.h"
#include "pimage.h"
#include "cacheusers.h"
#include "message.h"
#include "cachevaucher.h"
#include "basewidget.h"
#include "cachepaymentmode.h"
#include "cachecreditcard.h"
#include "databaseresult.h"
#include "paymentmode.h"
#include "defines.h"

PPrintVaucher::PPrintVaucher() :
    Base()
{
}

void PPrintVaucher::printVaucher(const QString &id)
{
    PPrintVaucher p;
    PPrintPreview *pv = new PPrintPreview(fMainWindow->fPreferences.getDefaultParentForMessage());
    PPrintScene *ps = pv->addScene(0, Portrait);
    p.fDb.fDb.transaction();
    p.fDbBind[":f_id"] = id;
    p.fDb.select("select f_source, f_user, f_room, f_guest, m.f_id, f_wdate, "
                 "f_paymentMode, f_amountAmd, f_amountVat, f_amountAmd / f_amountUsd, "
                 "f_creditCard, f_cityLedger, "
                 "f_canceled, f_cancelDate, u.f_username, m.f_finalName, "
                 "m.f_finance, concat(uo.f_firstName, ' ', uo.f_lastName), "
                 "m.f_paymentMode, m.f_remarks "
                 "from m_register m "
                 "left join users u on u.f_id=m.f_cancelUser "
                 "left join users uo on uo.f_id=m.f_user "
                 "where m.f_id=:f_id", p.fDbBind, p.fDbRows);

    if(p.fDbRows.count() == 0) {
        message_error("Application error. Contact to developer. Message: PrintVoucher row count=0");
        p.fDb.fDb.commit();
        return;
    }

    int room = p.fDbRows.at(0).at(2).toInt();
    DatabaseResult reserv;

    if(room > 0) {
        p.fDbBind[":f_id"] = id;
        QSqlQuery *q = 0;
        q = p.fDb.select(q, "select f_inv from m_register where f_id=:f_id", p.fDbBind);
        q->next();
        p.fDbBind[":f_invoice"] = q->value(0);
        reserv.select(p.fDb, "select r.f_startDate, r.f_checkInUser, r.f_endDate, r.f_checkOutUser, "
                             "r.f_checkInTime, r.f_checkOutTime "
                             "from f_reservation r "
                             "where r.f_invoice=:f_invoice", p.fDbBind);
    }

    CI_Vaucher *cv = CacheVaucher::instance()->get(p.fDbRows.at(0).at(0).toString());

    if(!cv) {
        message_error("Application error. Contact to developer. Message: PrintVoucher cv=0");
        p.fDb.fDb.commit();
        return;
    }

    int top = 10;
    PTextRect *trHeader = new PTextRect(20, top, 2000, 80, cv->fName, 0, QFont("Arial", 50));
    top += trHeader->textHeight();
    trHeader->setBorders(false, false, false, false);
    trHeader->setTextAlignment(Qt::AlignHCenter);
    QString inv = QString("S/N %1").arg(p.fDbRows.at(0).at(4).toString());
    PTextRect *trInvoice = new PTextRect(20, trHeader->textHeight(), 2000, 80, inv, 0, QFont("Arial", 30));
    top += trInvoice->textHeight();
    trInvoice->setTextAlignment(Qt::AlignHCenter);
    trInvoice->setBorders(false, false, false, false);
    PTextRect *trInfo = new PTextRect(1500, 20, 500, 400, fPreferences.getDb(def_vouchers_right_header).toString(),
                                      0, QFont("Arial", 25));
    int rowHeight = 80;
    QPen boldPen(Qt::black);
    boldPen.setWidth(6);
    QFont f("Arial", 30);
    f.setItalic(true);
    f.setBold(true);
    PTextRect th;
    th.setRectPen(boldPen);
    th.setBorders(false, false, false, false);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignHCenter);

    if(p.fDbRows.at(0).at(12).toInt() == 1) {
        top += ps->addTextRect(20, top, 2000, 80, QObject::tr("CORRECTION"), &th)->textHeight();
        f.setPointSize(20);
        f.setItalic(false);
        f.setBold(false);
        th.setFont(f);
        ps->addTextRect(20, top, 2000, 80, QString("[%1: %2  %3: %4]")
                        .arg(QObject::tr("DATE"))
                        .arg(p.fDbRows.at(0).at(13).toDate().toString(def_date_format))
                        .arg(QObject::tr("OPERATOR"))
                        .arg(p.fDbRows.at(0).at(14).toString()),
                        &th);
    }

    top += trInfo->textHeight();
    trInfo->setTextAlignment(Qt::AlignTop | Qt::AlignRight);
    trInfo->setBorders(false, false, false, false);
    ps->addItem(trInfo);
    ps->addItem(trInvoice);
    ps->addItem(trHeader);
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(20, 10, 500, 300));
    top = 320;
    ps->addLine(10, top, 2100, top, boldPen);
    top += 10;
    f.setPointSize(30);
    f.setItalic(false);
    f.setBold(false);
    th.setFont(f);
    th.setBorders(true, true, true, true);
    th.setTextAlignment(Qt::AlignHCenter);
    QList<int> cols;
    QStringList vals;
    int vatWidth = (p.fDbRows.at(0).at(2).toDouble() > 0.1 ? 200 : 0);
    cols << 20 << 230 << 200 << 200 << vatWidth << 1450 - vatWidth;
    vals << QObject::tr("Date")
         << QObject::tr("Room")
         << QObject::tr("Amount")
         << QObject::tr("VAT")
         << QObject::tr("Purpose")
         ;
    ps->addTableRow(top, rowHeight, cols, vals, &th);
    vals << p.fDbRows.at(0).at(5).toDate().toString(def_date_format)
         << p.fDbRows.at(0).at(2).toString()
         << float_str(p.fDbRows.at(0).at(7).toDouble(), 2)
         << (vatWidth == 0 ? "" : p.fDbRows.at(0).at(8).toString())
         << p.fDbRows.at(0).at(15).toString();
    ps->addTableRow(top, rowHeight, cols, vals, &th);

    if(p.fDbRows.at(0).at(16).toInt() == 1) {
        top += 10;
        cols.clear();
        cols << 20 << 1080 << 1000;
        vals << QObject::tr("Mode of Payment")
             << QObject::tr("Additional info");
        ps->addTableRow(top, rowHeight, cols, vals, &th);
        CI_PaymentMode *pm = CachePaymentMode::instance()->get(p.fDbRows.at(0).at(18).toString());

        if(!pm) {
            message_error_tr("Application error. Contact to developer. Message PrintVoucher pm=0");
            return;
        }

        vals << pm->fName;
        QString pmInfo;

        switch(pm->fCode.toInt()) {
        case PAYMENT_CASH:
            break;

        case PAYMENT_CARD:
            pmInfo = CacheCreditCard::instance()->get(p.fDbRows.at(0).at(10).toString())->fName;
            break;
        }

        vals << pmInfo;
        ps->addTableRow(top, rowHeight, cols, vals, &th);
    }

    top += 10;
    cols.clear();
    cols << 20 << 380 << 700 << 400 << 600;
    vals << QObject::tr("Guest name")
         << p.fDbRows.at(0).at(3).toString()
         << QObject::tr("Operator")
         << p.fDbRows.at(0).at(17).toString();
    th.setWrapMode(QTextOption::WordWrap);
    ps->addTableRow(top, rowHeight * 1.5, cols, vals, &th);
    th.setWrapMode(QTextOption::NoWrap);

    if(room > 0 && room < 1000 && reserv.rowCount() > 0) {
        QString checkInUser = "-";
        QString checkOutUser = "-";

        if(CacheUsers::instance()->get(reserv.value(0, "f_checkInUser").toString())) {
            checkInUser = CacheUsers::instance()->get(reserv.value(0, "f_checkInUser").toString())->fFull;
        }

        if(CacheUsers::instance()->get(reserv.value(0, "f_checkOutUser").toString())) {
            checkOutUser = CacheUsers::instance()->get(reserv.value(0, "f_checkOutUser").toString())->fFull;
        }

        cols.clear();
        cols << 20 << 380 << 700 << 400 << 600;
        vals << QObject::tr("Arrival date")
             << reserv.value(0, "f_startDate").toDate().toString(def_date_format)
             << QObject::tr("Operator")
             << checkInUser;
        ps->addTableRow(top, rowHeight, cols, vals, &th);
        vals << QObject::tr("Departure date")
             << reserv.value(0, "f_endDate").toDate().toString(def_date_format)
             << QObject::tr("Operator")
             << checkOutUser;
        ps->addTableRow(top, rowHeight, cols, vals, &th);
    }

    top += rowHeight;
    QString remarks = p.fDbRows.at(0).at(19).toString();

    if(!remarks.isEmpty()) {
        top += (rowHeight / 2);
        th.setTextAlignment(Qt::AlignLeft);
        th.setBorders(false, false, false, false);
        top += ps->addTextRect(20, top, 2100, rowHeight, QObject::tr("Remarks: ") + remarks, &th)->textHeight() + (rowHeight / 2);
        th.setBorders(true, true, true, true);
    }

    th.setBorders(false, false, false, false);
    vals << QObject::tr("Guest signature")
         << ""
         << QObject::tr("Manager signature")
         << "";
    ps->addTableRow(top, rowHeight, cols, vals, &th);
    top += (rowHeight * 2);
    ps->addLine(20, top, 600, top, boldPen);
    ps->addLine(1100, top, 2100, top, boldPen);
    top += rowHeight;
    QString footer = QString("%1: %2, %3: %4")
                     .arg(QObject::tr("Printed"))
                     .arg(QDateTime::currentDateTime().toString(def_date_time_format))
                     .arg(QObject::tr("Operator"))
                     .arg(WORKING_USERNAME);
    f.setBold(true);
    f.setPointSize(18);
    th.setFont(f);
    th.setTextAlignment(Qt::AlignRight);
    ps->addTextRect(20, top, 2100, rowHeight, footer, &th);
    p.fDb.fDb.commit();
    pv->exec();
    delete pv;
}
