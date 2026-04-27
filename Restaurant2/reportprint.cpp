#include "reportprint.h"
#include <QPainter>
#include <QPrinter>
#include <QPrinterInfo>
#include "branchstoremap.h"
#include "c5printing.h"
#include "database2.h"
#include "defrest.h"
#include "pimage.h"
#include "pprintscene.h"
#include "ptextrect.h"

ReportPrint::ReportPrint() :
    QObject(),
    Base()
{
}

void ReportPrint::printTotal(const QDate &date, const QString &printedBy, const QString &prn)
{
    // 1. Настройка принтера
    QPrinterInfo pi = QPrinterInfo::printerInfo(prn.isEmpty() ? "local" : prn);
    QPrinter printer(pi);
    printer.setPageSize(QPageSize::Custom);
    printer.setFullPage(false);

    QRectF pr = printer.pageRect(QPrinter::DevicePixel);
    constexpr qreal SAFE_RIGHT_MM = 4.0;
    qreal safePx = SAFE_RIGHT_MM * printer.logicalDpiX() / 25.4;

    int bs = 22;
    C5Printing p;
    p.setSceneParams(pr.width() - safePx, pr.height(), printer.logicalDpiX());

    // Логотип
    p.image("./logo_print.png", Qt::AlignHCenter);
    p.br();

    // Заголовок
    p.setFontSize(bs + 4);
    p.setFontBold(true);
    p.ctext(tr("CASH TOTAL REPORT"));
    p.br();

    p.setFontSize(bs);
    p.setFontBold(false);
    p.lrtext(tr("Տպված է"), printedBy); // Твой перевод
    p.br();
    p.lrtext(tr("Ամսաթիվ"), date.toString(def_date_format)); // Твой перевод
    p.br();
    p.line();
    p.br();

    // Расчет по мойке (500 драм)
    double totalff = totalx500(date);

    // 2. БД и Основной запрос
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    db2[":f_state"] = ORDER_STATE_CLOSED;
    db2[":f_dateCash"] = date;
    db2[":f_branch"] = defrest(dr_branch).toInt();

    // Группировка по 1, 2, 3, 4, 5, 8 как в оригинале
    db2.exec("select h.f_name as f_hallname, oh.f_paymentModeComment as f_paymentmodecomment, "
             "t.f_name as f_table_name, oh.f_id, oh.f_paymentModeComment, "
             "sum(d.f_total) as f_total, sum(d.f_totalUSD) as f_totalUSD, oc.f_govnumber, hp.f_discountcard "
             "from o_Header oh "
             "left join o_header_payment hp on hp.f_id=oh.f_id "
             "inner join r_table t on t.f_id=oh.f_table "
             "inner join o_dish d on d.f_header=oh.f_id "
             "left join r_store h on h.f_id=d.f_store "
             "left join o_car oc on oc.f_order=oh.f_id "
             "where oh.f_state=:f_state and d.f_state=1 "
             "and oh.f_dateCash=:f_dateCash and oh.f_branch=:f_branch "
             "group by 1, 2, 3, 4, 5, 8, 9 "
             "order by h.f_id, oh.f_paymentModeComment, oh.f_id ");

    QString currHall = "";
    QString currPayment = "";
    double grandTotal = 0, total = 0, total2 = 0;
    int grandCount = 0, count = 0;

    auto printGroupTotal = [&]() {
        if (count == 0)
            return;
        p.line();
        p.setFontBold(true);
        p.ltext(tr("Total for ") + currHall + "/" + currPayment, 0);
        p.br();
        p.ltext(QString::number(count), 40);
        p.rtext(float_str(total, 2));
        p.br();

        if (total2 > 0.1 && qAbs(total2 - total) > 0.1) {
            p.rtext("Նվ[" + float_str(total2, 2) + "]");
            p.br();
        }
        if (currHall.contains(QString::fromUtf8("ԱՎՏՈԼՎԱՑՈՒՄ"))) {
            p.rtext("Աշ[" + float_str(totalff, 2) + "]");
            p.br();
        }
        p.setFontBold(false);
        p.br();
        grandTotal += total;
        grandCount += count;
    };

    while (db2.next()) {
        QString rowHall = db2.string("f_hallname");
        QString rowPayment = db2.string("f_paymentmodecomment");

        if (currHall != rowHall || currPayment != rowPayment) {
            printGroupTotal();
            total = 0;
            total2 = 0;
            count = 0;
            currHall = rowHall;
            currPayment = rowPayment;

            p.setFontBold(true);
            p.ctext(currHall + "/" + currPayment);
            p.br();
            p.line();
            p.setFontBold(false);
        }

        double s1 = db2.doubleValue("f_total");
        double s2 = db2.doubleValue("f_totalUSD");
        total += s1;
        total2 += s2;
        count++;

        p.ltext(db2.string("f_table_name") + "/" + db2.string("f_govnumber"), 0);
        p.ltext(db2.string("f_id"), 30);

        QString s3 = float_str(s1, 2);
        if (s1 < s2) {
            s3 = QString("%1 [%2]").arg(float_str(s1, 2), float_str(s2, 2));
        }
        p.rtext(s3);
        p.br();

        if (!db2.string("f_paymentmodecomment").isEmpty()) {
            p.ltext(db2.string("f_paymentmodecomment"), 20);
            p.br();
        }
    }
    printGroupTotal();

    // Общий итог
    p.line();
    p.setFontBold(true);
    p.ltext(tr("Ընդամենը"), 0); // Твой перевод Grand Total
    p.br();
    p.ltext(QString::number(grandCount), 40);
    p.rtext(float_str(grandTotal, 2));
    p.br();
    p.line();
    p.br();

    // 3. Common
    db2[":f_dateCash"] = date;
    db2[":f_state"] = ORDER_STATE_CLOSED;
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select sum(p.f_cash) as f_cash, sum(p.f_card) as f_card, "
             "sum(p.f_coupon) as f_coupon, sum(p.f_debt) as f_debt, sum(p.f_discount) as f_discount, "
             "sum(p.f_couponservice) as f_couponservice, sum(p.f_idram) as f_idram "
             "from o_header_payment p "
             "left join o_header h on h.f_id=p.f_id "
             "where h.f_state=:f_state and h.f_dateCash=:f_dateCash and h.f_branch=:f_branch ");

    double commCash = 0, commCard = 0, commIdram = 0, commCoupon = 0, commService = 0, commDebt = 0;
    if (db2.next()) {
        commCash = db2.doubleValue("f_cash");
        commCard = db2.doubleValue("f_card");
        commIdram = db2.doubleValue("f_idram");
        commCoupon = db2.doubleValue("f_coupon");
        commService = db2.doubleValue("f_couponservice");
        commDebt = db2.doubleValue("f_debt");

        p.ctext(tr("Common"));
        p.br();
        auto row = [&](QString l, double v) {
            if (v > 0.01) {
                p.ltext(l, 0);
                p.rtext(float_str(v, 2));
                p.br();
            }
        };
        row(tr("Կանխիկ"), commCash);
        row(tr("Քարտ"), commCard);
        row("Իդրամ", commIdram);
        row("Նվեր քարտ", commCoupon);
        row("Ավտոլվացման կտրոն", commService);
        row(tr("Փոխանցում"), commDebt);
        row(tr("Զեղչ"), db2.doubleValue("f_discount"));
    }

    // 4. Salary & Deduction
    p.line();
    p.br();
    p.ctext(tr("Salary"));
    p.br();

    db2[":f_state1"] = ORDER_STATE_CLOSED;
    db2[":f_state2"] = DISH_STATE_READY;
    db2[":f_dateCash"] = date;
    db2[":f_store"] = storealias(2);
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select sum(d.f_qty)*1000 as f_deduction from o_dish d "
             "left join o_header h on h.f_id=d.f_header "
             "where h.f_state=:f_state1 and d.f_state=:f_state2 "
             "and h.f_dateCash=:f_dateCash and d.f_store=:f_store and h.f_branch=:f_branch ");

    double minusFromSalary = db2.next() ? db2.doubleValue("f_deduction") : 0;

    db2[":f_date"] = date;
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select f_comment, abs(f_amount) as f_amount from c_cash "
             "where f_branch=:f_branch and f_date=:f_date and f_debit=1 and f_credit=2 ");

    double totalSalary = 0;
    while (db2.next()) {
        p.ltext(db2.string("f_comment"), 0);
        p.rtext(float_str(db2.doubleValue("f_amount"), 2));
        p.br();
        totalSalary += db2.doubleValue("f_amount");
    }

    // 5. Total Finally
    p.line();
    p.br();
    p.setFontBold(true);
    p.ctext(tr("Total Finally"));
    p.br();
    p.setFontBold(false);

    double finalWashTotal = commCash + commCard + commDebt + commCoupon + commIdram + commService;

    p.ltext(tr("Total"), 0);
    p.rtext(float_str(finalWashTotal, 2));
    p.br();
    p.ltext(tr("Քարտ"), 0);
    p.rtext(float_str(commCard, 2));
    p.br();
    p.ltext("Իդրամ", 0);
    p.rtext(float_str(commIdram, 2));
    p.br();

    // Бонусные карты 555
    db2[":f_state"] = ORDER_STATE_CLOSED;
    db2[":f_dateCash"] = date;
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select hp.f_id, concat_ws(' ', 'Նվեր քարտ', right(f_discountcard, 4)) as bb from o_header oh "
             "left join o_header_payment hp on hp.f_id=oh.f_id "
             "where oh.f_state=:f_state and oh.f_dateCash=:f_dateCash "
             "and hp.f_discountcard like '555%' and oh.f_branch=:f_branch ");
    while (db2.next()) {
        p.ltext(db2.string("f_id") + " " + db2.string("bb"));
        p.br();
    }

    p.ltext("Ավտոլվացման կտրոն", 0);
    p.rtext(float_str(commService, 2));
    p.br();
    p.ltext(tr("Փոխանցում"), 0);
    p.rtext(float_str(commDebt, 2));
    p.br();
    p.ltext("Պահում (1000դրամ)", 0);
    p.rtext(float_str(minusFromSalary, 2));
    p.br();
    p.ltext(tr("Salary"), 0);
    p.rtext(float_str(totalSalary, 2));
    p.br();

    p.line();
    p.setFontBold(true);
    p.ltext(tr("Finally"), 0);
    p.rtext(float_str(commCash, 2));
    p.br();

    p.print(printer);
}

void ReportPrint::printTotalShort(const QDate &date, const QString &printedBy, const QString &prn)
{
    QPrinterInfo pi = QPrinterInfo::printerInfo(prn.isEmpty() ? "local" : prn);
    QPrinter printer(pi);
    printer.setPageSize(QPageSize::Custom);
    printer.setFullPage(false);

    QRectF pr = printer.pageRect(QPrinter::DevicePixel);
    qreal safePx = 4.0 * printer.logicalDpiX() / 25.4;

    int bs = 22;
    C5Printing p;
    p.setSceneParams(pr.width() - safePx, pr.height(), printer.logicalDpiX());

    p.image("./logo_print.png", Qt::AlignHCenter);
    p.br();

    p.setFontSize(bs + 4);
    p.setFontBold(true);
    p.ctext(tr("CASH TOTAL REPORT"));
    p.br();

    p.setFontSize(bs);
    p.setFontBold(false);
    p.ltext(tr("Printed by ") + printedBy, 0);
    p.br();
    p.ltext(tr("Date") + ": " + date.toString(def_date_format), 0);
    p.br();
    p.line();
    p.br();

    double totalff = totalx500(date);
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);

    db2[":f_dateCash"] = date;
    db2[":f_state"] = ORDER_STATE_CLOSED;
    db2[":f_branch"] = defrest(dr_branch).toInt();

    db2.exec("select hh.f_name, count(distinct(h.f_id)) as qty, "
             "sum(d.f_total) as f_total, sum(d.f_totalusd) as f_total2 "
             "from o_dish d "
             "inner join o_header h on h.f_id=d.f_header "
             "left join r_hall hh on hh.f_id=h.f_hall "
             "where h.f_state=:f_state and h.f_dateCash=:f_dateCash and d.f_state=1 and h.f_branch=:f_branch "
             "group by 1");

    while (db2.next()) {
        QString hallName = db2.string("f_name");
        double fTotal = db2.doubleValue("f_total");
        double fTotal2 = db2.doubleValue("f_total2");

        p.setFontBold(true);
        p.ctext(hallName);
        p.br();
        p.setFontBold(false);

        p.ltext(db2.string("qty"), 0);
        p.rtext(float_str(fTotal, 2));
        p.br();

        // ВОТ ТУТ БЫЛА ОШИБКА: используем f_total2 для расчетов мойки
        if (hallName.contains(QString::fromUtf8("ԱՎՏՈԼՎԱՑՈՒՄ"), Qt::CaseInsensitive)) {
            p.rtext("Աշխատավարձ[" + float_str(totalff, 2) + "]");
            p.br();
            // Считаем удержание именно от f_total2
            p.rtext("Պահում[" + float_str(fTotal2 - totalff, 2) + "]");
            p.br();
        }

        // Логика бонуса/подарка
        if (fTotal2 - fTotal > 0.001) {
            p.rtext("Նվեր[" + float_str(fTotal2 - fTotal, 2) + "]");
            p.br();
        }

        p.line();
        p.br(10);
    }

    // Зарплаты (Salary)
    p.br(10);
    p.setFontBold(true);
    p.ctext(tr("Salary"));
    p.br();
    p.setFontBold(false);

    db2[":f_date"] = date;
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select c.f_comment, abs(c.f_amount) as f_amount from c_cash c "
             "where f_branch=:f_branch and f_date=:f_date and f_debit=1 and f_credit=2");

    double totalSalary = 0;
    while (db2.next()) {
        double amt = db2.doubleValue("f_amount");
        p.ltext(db2.string("f_comment"), 0);
        p.rtext(float_str(amt, 2));
        p.br();
        totalSalary += amt;
    }

    p.br(10);
    p.line();

    // Итоговый блок
    db2[":f_dateCash"] = date;
    db2[":f_state"] = ORDER_STATE_CLOSED;
    db2[":f_branch"] = defrest(dr_branch).toInt();
    db2.exec("select sum(p.f_cash) as f_cash, sum(p.f_card) as f_card, "
             "sum(p.f_coupon) as f_coupon, sum(p.f_debt) as f_debt, sum(p.f_discount) as f_discount "
             "from o_header_payment p "
             "left join o_header h on h.f_id=p.f_id "
             "where h.f_branch=:f_branch and h.f_state=:f_state and h.f_dateCash=:f_dateCash");

    if (db2.next()) {
        double fCash = db2.doubleValue("f_cash");
        double fCard = db2.doubleValue("f_card");
        double fDebt = db2.doubleValue("f_debt");
        double fCoupon = db2.doubleValue("f_coupon");

        p.setFontBold(true);
        p.ctext(tr("Total Finally"));
        p.br();
        p.setFontBold(false);

        p.ltext(tr("Total"), 0);
        p.rtext(float_str(fCash + fCard + fDebt + fCoupon, 2));
        p.br();

        p.ltext(tr("Card"), 0);
        p.rtext(float_str(fCard, 2));
        p.br();

        p.ltext(tr("Փոխանցում"), 0);
        p.rtext(float_str(fDebt, 2));
        p.br();

        p.ltext(tr("Salary"), 0);
        p.rtext(float_str(totalSalary, 2));
        p.br();

        p.line();
        p.setFontBold(true);
        p.ltext(tr("Finally"), 0);
        p.rtext(float_str(fCash, 2));
        p.br();
    }

    p.br(20);
    p.ctext("_");

    p.print(printer);
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
