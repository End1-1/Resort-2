#include "reportprint.h"
#include "pprintscene.h"
#include "ptextrect.h"
#include "defrest.h"
#include "branchstoremap.h"
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

    ReportPrint rp;
    /* print carash - x*500 dram */

    double totalff = totalx500(date);
    /* end of print 500 dram */


    th.setTextAlignment(Qt::AlignLeft);
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    rp.fDb.select("select h.f_name, '', t.f_name, oh.f_id, oh.f_paymentModeComment, "
                  "sum(d.f_total), sum(d.f_totalUSD), oc.f_govnumber, hp.f_discountcard "
                  "from o_Header oh "
                  "left join o_header_payment hp on hp.f_id=oh.f_id "
                  // "inner join r_hall h on h.f_id=oh.f_hall "
                  "inner join r_table t on t.f_id=oh.f_table "
                  "inner join o_dish d on d.f_header=oh.f_id "
                  "left join r_store h on h.f_id=d.f_store "
                  "left join o_car oc on oc.f_order=oh.f_id "
                  "where oh.f_state=:f_state and d.f_state=1 "
                  "and oh.f_dateCash=:f_dateCash and oh.f_branch=:f_branch "
                  "group by 1, 2, 3, 4, 5, 8 "
                  "order by h.f_id, 2, oh.f_id ",
                  rp.fDbBind, rp.fDbRows);
    QString currHall = "";
    QString currPayment = "";
    double grandTotal = 0;
    int grandCount = 0;
    double total = 0;
    double total2 = 0;
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
                ps->addTextRect(200, top, 150, rowHeight, QString::number(count), &th);
                if (total2 > 0.1 && total2 != total) {
                    top += ps->addTextRect(350, top, 200, rowHeight, float_str(total, 2), &th)->textHeight();
                    if (top > sizePortrait.height() - 200) {
                        top = 10;
                        ps = new PPrintScene(Portrait);
                        lps.append(ps);
                    }
                    top += ps->addTextRect(350, top, 200, rowHeight, "Նվ[" + float_str(total2, 2) + "]", &th)->textHeight();
                    if (top > sizePortrait.height() - 200) {
                        top = 10;
                        ps = new PPrintScene(Portrait);
                        lps.append(ps);
                    }
                } else {
                    top += ps->addTextRect(350, top, 200, rowHeight, float_str(total, 2), &th)->textHeight();
                    if (top > sizePortrait.height() - 200) {
                        top = 10;
                        ps = new PPrintScene(Portrait);
                        lps.append(ps);
                    }
                }
                if (currHall.contains(QString::fromUtf8("ԱՎՏՈԼՎԱՑՈՒՄ"))) {
                    top += ps->addTextRect(350, top, 200, rowHeight, "Աշ[" + float_str(totalff, 2) + "]", &th)->textHeight();
                }
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
            total2 = 0.0;
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
        total2 += row.at(6).toDouble();
        count++;
        ps->addTextRect(10, top, 250, rowHeight, row.at(2).toString() + "/" + row.at(7).toString(), &th);
        ps->addTextRect(255, top, 195, rowHeight, row.at(3).toString(), &th);
        double s1 = row.at(5).toDouble();
        double s2 = row.at(6).toDouble();
        QString s3 = float_str(s1, 2);
        bool addline = false;
        DatabaseResult drcard;
        if (s1 < s2) {
            s3 = QString("%1 [%2]").arg(float_str(s1, 2), float_str(s2, 2));
            qDebug() << row;
            rp.fDbBind[":f_card"] = row.at(8);
            drcard.select(rp.fDb, "select f_name from d_car_client where f_card=:f_card", rp.fDbBind);
            if (drcard.rowCount() > 0) {
                addline = true;
            }

        }
        top += ps->addTextRect(new PTextRect(460, top, 200, rowHeight,  s3, &th, f))->textHeight();
        if ( row.at(4).toString().length() > 0) {
            top += ps->addTextRect(10, top, 680, rowHeight, row.at(4).toString(), &th)->textHeight();
            top += 5;
        }
        if (addline) {
            /*
            top += ps->addTextRect(0, top, 680, rowHeight, drcard.value(0).toString(), &th)->textHeight();
            if (top > sizePortrait.height() - 200) {
                top = 10;
                ps = new PPrintScene(Portrait);
                lps.append(ps);
            }
            */
        }
        ps->addLine(10, top, 680, top, dotPen);
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    if (total > 0.1 || total2 > 0.1) {
        grandTotal += total;
        grandCount += count;
        top += 2;
        ps->addLine(10, top, 750, top);
        th.setTextAlignment(Qt::AlignLeft);
        f.setBold(true);
        th.setFont(f);
        top += ps->addTextRect(10, top, 680, rowHeight, tr("Total for ") + currHall + "/" + currPayment, &th)->textHeight();
        ps->addTextRect(200, top, 150, rowHeight, QString::number(count), &th);
        if (total2 > 0.1 && total != total2) {
            top += ps->addTextRect(350, top, 200, rowHeight, float_str(total, 2), &th)->textHeight();
            top += ps->addTextRect(350, top, 200, rowHeight, "Նվ[" + float_str(total2, 2) + "]", &th)->textHeight();
        } else {
            top += ps->addTextRect(350, top, 200, rowHeight, float_str(total, 2), &th)->textHeight();
        }
        if (currHall.contains(QString::fromUtf8("ԱՎՏՈԼՎԱՑՈՒՄ"))) {
            top += ps->addTextRect(350, top, 200, rowHeight, "Աշ[" + float_str(totalff, 2) + "]", &th)->textHeight();
        }
        top += rowHeight;
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    f.setBold(true);
    th.setFont(f);
    ps->addTextRect(10, top, 300, rowHeight, tr("Grand total ") , &th);
    ps->addTextRect(310, top, 150, rowHeight, QString::number(grandCount), &th);
    top += ps->addTextRect(460, top, 200, rowHeight, float_str(grandTotal, 2), &th)->textHeight();
    top += 20;
    ps->addLine(10, top, 680, top, dotPen);
    top ++;

    DatabaseResult dr;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    dr.select(rp.fDb, "select sum(p.f_cash) as f_cash, sum(p.f_card) as f_card, "
              "sum(p.f_coupon) as f_coupon, sum(p.f_debt) as f_debt, sum(p.f_discount) as f_discount, "
              "sum(p.f_couponservice) as f_couponservice, sum(p.f_idram) as f_idram "
              "from o_header_payment p "
              "left join o_header h on h.f_id=p.f_id "
              "where h.f_state=:f_state and h.f_dateCash=:f_dateCash and h.f_branch=:f_branch ", rp.fDbBind);
    th.setTextAlignment(Qt::AlignHCenter);
    f.setBold(true);
    th.setFont(f);
    PTextRect trl(th, "");
    trl.setTextAlignment(Qt::AlignLeft);
    PTextRect trr(th, "");
    trr.setTextAlignment(Qt::AlignRight);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Common"), &th)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, tr("Cash"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_cash").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, tr("Card"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_card").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, "Իդրամ", &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_idram").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, "Նվեր քարտ", &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_coupon").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, "Ավտոլվացման կտրոն", &trl);
    QString finalCoupoService = dr.value("f_couponservice").toString();
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_couponservice").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, tr("Փոխանցում"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_debt").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, tr("Discount"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, dr.value("f_discount").toString(), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    top += 20;

    ps->addLine(10, top, 680, top, dotPen);
    top += 20;
    th.setTextAlignment(Qt::AlignCenter);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Salary"), &th)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    DatabaseResult drsal;
    rp.fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_state2"] = DISH_STATE_READY;
    rp.fDbBind[":f_dateCash"] =  date;
    rp.fDbBind[":f_store"] = storealias(2);
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
//    drsal.select(rp.fDb,
//                 QString("select sum(d.f_qty)*1000 as f_deduction, sum(d.f_totalUSD) as f_total from o_dish d "
//                 "left join o_header h on h.f_id=d.f_header "
//                 "where h.f_state=:f_state1 and d.f_state=:f_state2 "
//                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch "
//                "and d.f_dish not in (%1)").arg("159,171,158,169,153,165,386,387,388,389,390,391"), rp.fDbBind);
    drsal.select(rp.fDb,
                 QString("select sum(d.f_qty)*1000 as f_deduction, sum(d.f_totalUSD) as f_total from o_dish d "
                 "left join o_header h on h.f_id=d.f_header "
                 "where h.f_state=:f_state1 and d.f_state=:f_state2 "
                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch "), rp.fDbBind);
    double minusfromsalary = drsal.value("f_deduction").toInt();

    rp.fDbBind[":f_date"] = date;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    drsal.select(rp.fDb, "select c.f_comment, abs(c.f_amount) as f_amount from c_cash c where f_branch=:f_branch and f_date=:f_date and f_debit=1 and f_credit=2 ", rp.fDbBind);
//    drsal.select(rp.fDb, "select ug.f_en, sum(s2.f_amount) as f_amount from "
//                 "salary2 s2 "
//                 "inner join users u on u.f_id=s2.f_employee "
//                 "inner join users_groups ug on ug.f_id=u.f_group "
//                 "where s2.f_date=:f_date "
//                 "group by 1 ", rp.fDbBind);

    double totalSalary = 0;
    for (int i = 0; i < drsal.rowCount(); i++) {
        ps->addTextRect(10, top, 680, rowHeight, drsal.value(i, "f_comment").toString(), &trl);
        top += ps->addTextRect(10, top,  680, rowHeight, float_str(drsal.value(i, "f_amount").toDouble(), 2), &trr)->textHeight();
        totalSalary += drsal.value(i, "f_amount").toDouble();
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }

    DatabaseResult drFinalWash;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    drFinalWash.select(rp.fDb, "select sum(p.f_cash) as f_cash, sum(p.f_card) as f_card, "
              "sum(p.f_coupon) as f_coupon, sum(p.f_debt) as f_debt, "
            "sum(p.f_discount) as f_discount, "
                "sum(p.f_idram) as f_idram "
              "from o_header_payment p "
              "left join o_header h on h.f_id=p.f_id "
              "where h.f_state=:f_state and h.f_dateCash=:f_dateCash and h.f_branch=:f_branch ", rp.fDbBind);;

    th.setTextAlignment(Qt::AlignCenter);
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Total Finally"), &th)->textHeight();
    ps->addTextRect(10, top, 680, rowHeight, tr("Total"), &trl);
    double finalWashTotal = drFinalWash.value("f_cash").toDouble();
    finalWashTotal += drFinalWash.value("f_card").toDouble();
    finalWashTotal += drFinalWash.value("f_debt").toDouble();
    finalWashTotal += drFinalWash.value("f_coupon").toDouble();
    finalWashTotal += drFinalWash.value("f_idram").toDouble();
    finalWashTotal += str_float(finalCoupoService);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(finalWashTotal, 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, tr("Card"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_card").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, "Իդրամ", &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_idram").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    /* i dont know why */
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    rp.fDb.select(QString("select hp.f_id,  '%1' as bb "
                  "from o_Header oh "
                  "left join o_header_payment hp on hp.f_id=oh.f_id "
                  "where oh.f_state=:f_state and oh.f_dateCash=:f_dateCash and hp.f_discountcard like '555%' and oh.f_branch=:f_branch ")
                  .arg(QString::fromUtf8("Նվեր քարդ")),
                  rp.fDbBind, rp.fDbRows);
    foreach (QList<QVariant> row, rp.fDbRows) {
        top += ps->addTextRect(0, top, 680, rowHeight, row.at(0).toString() + " " + row.at(1).toString(), &th)->textHeight();
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }

    ps->addTextRect(10, top, 680, rowHeight, "Ավտոլվացման կտրոն", &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, finalCoupoService, &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, tr("Փոխանցում"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_debt").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, "Պահում (1000դրամ)", &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(minusfromsalary, 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, tr("Salary"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(totalSalary, 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }


    ps->addTextRect(10, top, 680, rowHeight, tr("Finally"), &trl);
    //top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_cash").toDouble() - totalSalary, 2), &trr)->textHeight();
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_cash").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 300, rowHeight, "_", &th);


    QPrinter printer;
    printer.setPrinterName(prn);
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
    qDeleteAll(lps);
}

void ReportPrint::printTotalShort(const QDate &date, const QString &printedBy, const QString &prn)
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
    double totalff = totalx500(date);
    ReportPrint rp;

    DatabaseResult dr;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    dr.select(rp.fDb, "select hh.f_name, count(distinct(h.f_id)) as qty, "
              "sum(d.f_total) as f_total, sum(d.f_totalusd) as f_total2 "
              "from o_dish d  "
              "inner join o_header h on h.f_id=d.f_header "
              "left join r_hall hh on hh.f_id=h.f_hall "
              "where h.f_state=:f_state and h.f_dateCash=:f_dateCash and d.f_state=1 and h.f_branch=:f_branch "
              "group by 1 ", rp.fDbBind);
    th.setTextAlignment(Qt::AlignHCenter);
    f.setBold(true);
    th.setFont(f);
    PTextRect trl(th, "");
    trl.setTextAlignment(Qt::AlignLeft);
    PTextRect trr(th, "");
    trr.setTextAlignment(Qt::AlignRight);
    for (int i = 0; i < dr.rowCount(); i++) {
        top += ps->addTextRect(10, top, 680, rowHeight, dr.value(i, "f_name").toString(), &th)->textHeight();
        ps->addTextRect(10, top, 680, rowHeight, dr.value(i, "qty").toString(), &trl);
        top += ps->addTextRect(10, top,  680, rowHeight, dr.value(i, "f_total").toString(), &trr)->textHeight();
        if (dr.value(i, "f_name").toString().contains(QString::fromUtf8("ԱՎՏՈԼՎԱՑՈՒՄ"), Qt::CaseInsensitive)) {
            top += ps->addTextRect(10, top,  680, rowHeight, "Աշխատավարձ[" + float_str(totalff, 2) + "]", &trr)->textHeight();
            top += ps->addTextRect(10, top,  680, rowHeight, "Պահում[" + float_str(dr.value(i, "f_total2").toDouble() - totalff, 2) + "]", &trr)->textHeight();
        }
        if (dr.value(i, "f_total2").toDouble() - dr.value(i, "f_total").toDouble() > 0.001) {
            top += ps->addTextRect(10, top,  680, rowHeight, "Նվեր[" + float_str(dr.value(i, "f_total2").toDouble()-dr.value(i, "f_total").toDouble(), 2) + "]", &trr)->textHeight();
        }
        ps->addLine(10, top, 750, top);
        top += 20;
    }

    QPen dotPen(Qt::DotLine);
    ps->addLine(10, top, 680, top, dotPen);
    top += 20;
    th.setTextAlignment(Qt::AlignCenter);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Salary"), &th)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    DatabaseResult drsal;
    rp.fDbBind[":f_date"] = date;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    drsal.select(rp.fDb, "select c.f_comment, abs(c.f_amount) as f_amount from c_cash c where f_branch=:f_branch and f_date=:f_date and f_debit=1 and f_credit=2 ", rp.fDbBind);

    double totalSalary = 0;
    for (int i = 0; i < drsal.rowCount(); i++) {
        ps->addTextRect(10, top, 680, rowHeight, drsal.value(i, "f_comment").toString(), &trl);
        top += ps->addTextRect(10, top,  680, rowHeight, float_str(drsal.value(i, "f_amount").toDouble(), 2), &trr)->textHeight();
        totalSalary += drsal.value(i, "f_amount").toDouble();
        if (top > sizePortrait.height() - 200) {
            top = 10;
            ps = new PPrintScene(Portrait);
            lps.append(ps);
        }
    }
    top += 20;
    ps->addLine(10, top, 680, top, dotPen);
    top ++;

    DatabaseResult drFinalWash;
    rp.fDbBind[":f_dateCash"] = date;
    rp.fDbBind[":f_state"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    drFinalWash.select(rp.fDb, "select sum(p.f_cash) as f_cash, sum(p.f_card) as f_card, "
              "sum(p.f_coupon) as f_coupon, sum(p.f_debt) as f_debt, sum(p.f_discount) as f_discount "
              "from o_header_payment p "
              "left join o_header h on h.f_id=p.f_id "
              "where h.f_branch=:f_branch and h.f_state=:f_state and h.f_dateCash=:f_dateCash ", rp.fDbBind);;

    th.setTextAlignment(Qt::AlignCenter);
    top += ps->addTextRect(10, top, 680, rowHeight, tr("Total Finally"), &th)->textHeight();
    ps->addTextRect(10, top, 680, rowHeight, tr("Total"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_cash").toDouble()
                                                               +drFinalWash.value("f_card").toDouble()
                                                               +drFinalWash.value("f_debt").toDouble()
                                                               +drFinalWash.value("f_coupon").toDouble()
                                                               , 2), &trr)->textHeight();
    ps->addTextRect(10, top, 680, rowHeight, tr("Card"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_card").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, tr("Փոխանցում"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_debt").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }
    ps->addTextRect(10, top, 680, rowHeight, tr("Salary"), &trl);
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(totalSalary, 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 680, rowHeight, tr("Finally"), &trl);
    //top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_cash").toDouble() - totalSalary, 2), &trr)->textHeight();
    top += ps->addTextRect(10, top,  680, rowHeight, float_str(drFinalWash.value("f_cash").toDouble(), 2), &trr)->textHeight();
    if (top > sizePortrait.height() - 200) {
        top = 10;
        ps = new PPrintScene(Portrait);
        lps.append(ps);
    }

    ps->addTextRect(10, top, 300, rowHeight, "_", &th);


    QPrinter printer;
    printer.setPrinterName(prn);
    QMatrix m;
#ifdef QT_DEBUG
    m.scale(0.5, 0.5);
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
    qDeleteAll(lps);
}

double ReportPrint::totalx500(const QDate &date)
{
    ReportPrint rp;
    DatabaseResult drsal1;
    rp.fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_state2"] = DISH_STATE_READY;
    rp.fDbBind[":f_dateCash"] =  date;
    rp.fDbBind[":f_store"] = storealias(2);
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
    drsal1.select(rp.fDb, "select sum(d.f_totalUSD) as f_total from o_dish d "
                 "left join o_header h on h.f_id=d.f_header "
                 "where h.f_state=:f_state1 and d.f_state=:f_state2 and h.f_branch=:f_branch "
                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store", rp.fDbBind);

    double totalff = drsal1.value("f_total").toDouble();

    rp.fDbBind[":f_state1"] = ORDER_STATE_CLOSED;
    rp.fDbBind[":f_state2"] = DISH_STATE_READY;
    rp.fDbBind[":f_dateCash"] =  date;
    rp.fDbBind[":f_store"] = storealias(2);
    rp.fDbBind[":f_branch"] = defrest(dr_branch).toInt();
//    drsal1.select(rp.fDb,
//                 QString("select sum(d.f_qty)*1000 as f_deduction, sum(d.f_totalUSD) as f_total from o_dish d "
//                 "left join o_header h on h.f_id=d.f_header "
//                 "where h.f_state=:f_state1 and d.f_state=:f_state2 "
//                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch "
//                "and d.f_dish not in (%1)").arg("159,171,158,169,153,165,386,387,388,389,390,391"), rp.fDbBind);
    drsal1.select(rp.fDb,
                 QString("select sum(d.f_qty)*1000 as f_deduction, sum(d.f_totalUSD) as f_total from o_dish d "
                 "left join o_header h on h.f_id=d.f_header "
                 "where h.f_state=:f_state1 and d.f_state=:f_state2 "
                 "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch "), rp.fDbBind);
    totalff -= drsal1.value("f_deduction").toDouble();
    return totalff;

}
