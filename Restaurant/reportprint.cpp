#include "reportprint.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "pimage.h"
#include <QPrinter>
#include <QPainter>

ReportPrint::ReportPrint() :
    QObject(),
    Base()
{

}

void ReportPrint::printTotal(const QDate &date, const QString &printedBy, const QString &prn)
{
    QList<PPrintScene*> lps;
    PPrintScene *ps = new PPrintScene(Portrait);
    lps.append(ps);
    PTextRect th;
    QFont f("Arial", 30);
    th.setTextAlignment(Qt::AlignHCenter);
    th.setFont(f);
    th.setBorders(false, false, false, false);
    int top = 10;
    th.setTextAlignment(Qt::AlignHCenter);
    int rowHeight = 60;
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(200, top, 400, 250));
    top += 250;
    top += ps->addTextRect(new PTextRect(10, top, 750, rowHeight, QString("%1").arg(tr("CASH TOTAL REPORT")), &th, f))->textHeight();
    f.setPointSize(24);
    th.setFont(f);
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("Printed by ") + printedBy, &th, f))->textHeight();
    top += ps->addTextRect(new PTextRect(10, top, 680, rowHeight, tr("Date") + ": " + date.toString(def_date_format), &th, f))->textHeight();
    ps->addLine(10, top, 750, top);

    th.setTextAlignment(Qt::AlignLeft);
    ReportPrint rp;
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDb.select("select h.f_name, pm.f_" + def_lang + ", t.f_name, oh.f_id, oh.f_paymentModeComment, oh.f_total "
                  "from o_Header oh "
                  "inner join r_hall h on h.f_id=oh.f_hall "
                  "inner join r_table t on t.f_id=oh.f_table "
                  "inner join f_payment_type pm on pm.f_id=oh.f_paymentMode "
                  "where oh.f_state=:f_state "
                  "and oh.f_dateCash=:f_dateCash "
                  "group by 1, 2, 3, 4 "
                  "order by 1, 2, oh.f_id ",
                  rp.fDbBind, rp.fDbRows);
    QString currHall = "";
    QString currPayment = "";
    double grandTotal = 0;
    int grandCount = 0;
    double total = 0;
    int count = 0;
    QPen dotPen(Qt::DotLine);
    foreach (QList<QVariant> row, rp.fDbRows) {
        if (currHall != row.at(0).toString() || currPayment != row.at(1).toString()) {
            top += 2;
            ps->addLine(10, top, 750, top);
            if (total > 0.1) {
                grandTotal += total;
                grandCount += count;
                top += 2;
                ps->addLine(10, top, 750, top);
                th.setTextAlignment(Qt::AlignLeft);
                f.setBold(true);
                th.setFont(f);
                top += ps->addTextRect(10, top, 680, rowHeight, tr("Total for ") + currHall + "/" + currPayment, &th)->textHeight();
                ps->addTextRect(310, top, 150, rowHeight, QString::number(count), &th);
                top += ps->addTextRect(460, top, 200, rowHeight, float_str(total, 2), &th)->textHeight();
                top += rowHeight;
                f.setBold(false);
                th.setFont(f);
                if (top > sizePortrait.height() - 200) {
                    top = 10;
                    ps = new PPrintScene(Portrait);
                    lps.append(ps);
                }
            }
            total = 0.0;
            count = 0;
            currHall = row.at(0).toString();
            currPayment = row.at(1).toString();
            top += 2;
            th.setTextAlignment(Qt::AlignCenter);
            top += ps->addTextRect(new PTextRect(10, top, 750, rowHeight, currHall + "/" + currPayment, &th, f))->textHeight();
            ps->addLine(10, top, 750, top);
            th.setTextAlignment(Qt::AlignLeft);
            top += 2;
        }
        total += row.at(5).toDouble();
        if (row.at(5).toDouble() > 0.1) {
            count ++;
        }
        ps->addTextRect(10, top, 250, rowHeight, row.at(2).toString(), &th);
        ps->addTextRect(255, top, 195, rowHeight, row.at(3).toString(), &th);
        top += ps->addTextRect(new PTextRect(460, top, 200, rowHeight,  row.at(5).toString(), &th, f))->textHeight();
        if ( row.at(4).toString().length() > 0) {
            top += ps->addTextRect(10, top, 680, rowHeight, row.at(4).toString(), &th)->textHeight();
            top += 5;
        }
        ps->addLine(10, top, 680, top, dotPen);
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    if (total > 0.1) {
        grandTotal += total;
        grandCount += count;
        top += 2;
        ps->addLine(10, top, 750, top);
        th.setTextAlignment(Qt::AlignLeft);
        f.setBold(true);
        th.setFont(f);
        top += ps->addTextRect(10, top, 680, rowHeight, tr("Total for ") + currHall + "/" + currPayment, &th)->textHeight();
        ps->addTextRect(310, top, 150, rowHeight, QString::number(count), &th);
        top += ps->addTextRect(460, top, 200, rowHeight, float_str(total, 2), &th)->textHeight();
        top += rowHeight;
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }

    top += (rowHeight * 3);
    f.setBold(true);
    th.setFont(f);
    ps->addTextRect(10, top, 300, rowHeight, tr("Grand total ") , &th);
    ps->addTextRect(310, top, 150, rowHeight, QString::number(grandCount), &th);
    top += ps->addTextRect(460, top, 200, rowHeight, float_str(grandTotal, 2), &th)->textHeight();
    top += 20;
    ps->addTextRect(10, top, 300, rowHeight, "_", &th);


    QPrinter printer;
    printer.setPrinterName(prn);
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
    qDeleteAll(lps);
}
