#include "pexportinvoicetoexcel.h"
#include "excel.h"
#include "message.h"
#include "cacheusers.h"
#include "paymentmode.h"

PExportInvoiceToExcel::PExportInvoiceToExcel() :
    QObject(),
    Base()
{

}

void PExportInvoiceToExcel::exportInvoice(const QString &invoice, int side)
{
    PExportInvoiceToExcel p;
    p.run(invoice, side);
}

void PExportInvoiceToExcel::run(const QString &invoice, int fSide)
{
    Excel e;
    e.setColumnWidth(1, 2);
    e.setColumnWidth(2, 12);
    e.setColumnWidth(3, 40);
    e.setColumnWidth(4, 6);
    e.setColumnWidth(5, 10);
    e.setColumnWidth(6, 10);
    e.setColumnWidth(7, 10);
    int numNights = 0;
    fDbBind[":f_invoice"] = invoice;
    QString query;
    if (fSide > -1) {
        fDbBind[":f_side"] = fSide;
        query = "select ic.f_sign, ic.f_wdate, ic.f_paymentMode, ic.f_finalName, "
                "ic.f_amountAmd, ic.f_amountVat, ic.f_dc, ic.f_remarks "
                "from m_register ic "
                "where ic.f_inv=:f_invoice and ic.f_side=:f_side "
                "and ic.f_canceled=0 and ic.f_finance=1 "
                "order by ic.f_wdate, ic.f_id ";
    } else {
        query = "select ic.f_sign, ic.f_wdate, ic.f_paymentMode, ic.f_finalName, "
                "ic.f_amountAmd, ic.f_amountVat, ic.f_dc, ic.f_remarks "
                "from m_register ic "
                "where ic.f_inv=:f_invoice and ic.f_canceled=0 and ic.f_finance=1 "
                "order by ic.f_wdate, ic.f_id ";
    }
    DatabaseResult drvoucher;
    drvoucher.select(fDb, query, fDbBind);
    if (drvoucher.rowCount() == 0) {
        message_error(tr("Cannot print empty invoice. Contact with program developer."));
        return;
    }

    DatabaseResult drheader;
    fDbBind[":f_invoice"] = invoice;
    drheader.select(fDb, "select concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName) as guest, g.f_nation, "
               "cat.f_short, cat.f_description, r.f_room, r.f_startDate, "
               "r.f_checkInDate, r.f_checkinTime, r.f_endDate, r.f_man+r.f_woman+r.f_child as guest_qty, "
               "r.f_checkOutTime, r.f_author, ar.f_" + def_lang + " as f_arr, r.f_cardex, ca.f_name as cardexname, "
               "r.f_vatMode, vm.f_" + def_lang + " as vatmode, r.f_upgradeFrom, g.f_address, r.f_checkinUser,"
               "nights.ntotal, r.f_checkOutUser "
               "from f_reservation r "
               "inner join f_guests g on g.f_id=r.f_guest "
               "inner join f_room rm on rm.f_id=r.f_room "
               "inner join f_room_classes cat on cat.f_id=rm.f_class "
               "left join f_cardex ca on ca.f_cardex=r.f_cardex "
               "left join f_room_arrangement ar on ar.f_id=r.f_arrangement "
               "inner join f_vat_mode vm on vm.f_id=r.f_vatMode "
               "left join (select f_inv, count(f_id) as ntotal from m_register where f_canceled=0 and f_source='RM' and f_inv=:f_invoice) nights on nights.f_inv=r.f_invoice "
               "where r.f_invoice=:f_invoice ", fDbBind);


    fDbBind[":f_invoice"] = invoice;
    DatabaseResult drh;
    drh.select(fDb, "select f_state from f_reservation where f_invoice=:f_invoice", fDbBind);
    numNights = drheader.value("ntotal").toInt();


    QString invHeader = drh.value("f_state").toInt() == RESERVE_CHECKOUT ? tr("SETTLEMENT / TAX INVOICE") : tr ("PROFORMA INVOICE");
    e.setValue(invHeader, 1, 1);
    e.mergeCells("A1", "E1", true);
    e.setHorizontalAlignment("A1", "E1", Excel::hCenter);

    e.setValue(fPreferences.getDb(def_vouchers_right_header).toString(), 1, 6);
    QString inv = QString("%1 #%2")
            .arg(tr("ROOM"))
            .arg(QString("%1").arg(drheader.value("f_room").toString()));
    e.setValue(inv, 2, 1);
    e.mergeCells("A2", "G2", true);
    e.setHorizontalAlignment("A2", "G2", Excel::hCenter);
    /*
    PImage *logo = new PImage("logo_print.png");
    ps->addItem(logo);
    logo->setRect(QRectF(20, 10, 500, 300));
    */

    e.setValue(drheader.value("guest").toString(), 3, 1);
    e.mergeCells("A3", "G3", true);
    int row = 4;
    if (!drheader.value("f_address").toString().isEmpty()) {
        e.setValue(tr("Address: ") + drheader.value("f_address").toString(), row, 1);
        e.mergeCells("A4", "G4", true);
        row++;
    }
    row++;

    e.setValue(tr("Nationality"), row, 1);
    e.setValue(drheader.value("f_nation").toString(), row, 3);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    //row 1
    e.setValue(tr("Arrival date"), row, 4);
    e.setValue(drheader.value("f_startDate").toString(), row, 6);
    row++;
    //row 2
    e.setValue(tr("Room category"), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(drheader.value("f_description").toString(), row, 3);
    e.setValue(tr("Departure date"), row, 4);
    e.mergeCells("D" + QString::number(row), "E" + QString::number(row), true);
    e.setValue(drheader.value("f_endDate").toString(), row, 6);
    row++;
    //row 3
    QString room = drheader.value("f_room").toString();
    if (drheader.value("f_upgradeFrom").toInt() > 0) {
        room += "           " + drheader.value("f_upgradeFrom").toString();
    }
    e.setValue(tr("S/N "), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(invoice, row, 3);
    e.setValue(tr("Number of nights"), row, 4);
    e.mergeCells("D" + QString::number(row), "E" + QString::number(row), true);
    e.setValue(QString::number(numNights), row, 6);
    row++;
    //row 4
    e.setValue(tr("CheckIn"), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(drheader.value("f_checkInDate").toString(), row, 3);
    e.setValue(tr("Number of guests"), row, 4);
    e.mergeCells("D" + QString::number(row), "E" + QString::number(row), true);
    e.setValue(drheader.value("guest_qty").toString(), row, 6);
    row++;
    //row 5
    e.setValue(tr("CheckIn time"), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(drheader.value("f_checkinTime").toString(), row, 3);
    e.setValue(tr("CheckOut date"), row, 4);
    e.mergeCells("D" + QString::number(row), "E" + QString::number(row), true);
    e.setValue(drheader.value("f_checkOutTime").toString(), row, 6);
    row++;
    //row 6
    e.setValue(tr("Operator in"), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(drheader.value("f_checkinUser").toString(), row, 3);
    e.setValue(tr("CheckOut time"), row, 4);
    e.mergeCells("D" + QString::number(row), "E" + QString::number(row), true);
    e.setValue(drheader.value("f_checkOutTime").toString(), row, 6);
    row++;
    //row 7
    e.setValue(tr("Arrangement"), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(drheader.value("f_arr").toString(), row, 3);
    e.setValue(tr("CheckOut Op."), row, 4);
    e.mergeCells("D" + QString::number(row), "E" + QString::number(row), true);
    CI_User *u = CacheUsers::instance()->get(drheader.value("f_checkOutUser").toString());
    if (u) {
        e.setValue(u->fFull, row, 6);
    } else {
        e.setValue("-", row, 6);
    }
    row++;
    //row 8
    e.setValue(tr("Cardex"), row, 1);
    e.mergeCells("A" + QString::number(row), "B" + QString::number(row), true);
    e.setValue(drheader.value("f_cardex").toString() + "/" + drheader.value("cardexname").toString(), row, 3);
    e.setHorizontalAlignment("C5", "C13", Excel::hLeft);
    e.setHorizontalAlignment("F5", "F13", Excel::hRight);
    row++;
    row++;


    //table header
    int r1 = row;
    e.setValue(tr("*"), row, 1);
    e.setValue(tr("Date"), row, 2);
    e.setValue(tr("Description"), row, 3);
    e.setValue(tr("Cur"), row, 4);
    e.setValue(tr("Debit\n") + " " + drheader.value("vatmode").toString(), row, 5);
    e.setValue(tr("Credit"), row, 6);
    e.setValue(tr("Balance"), row, 7);
    e.setHorizontalAlignment("A" + QString::number(row), "G" + QString::number(row), Excel::hCenter);
    e.setVerticalAlignment("A" + QString::number(row), "G" + QString::number(row), Excel::vCenter);
    e.setFontBold("A" + QString::number(row), "G" + QString::number(row), true);
    row++;

    //table data

    int rowNum = 1;
    double lastBalance = 0;
    double totalVat = 0;
    double totalCredit = 0;
    double totalDebet = 0;
    double totalCash = 0;
    double totalCard = 0;
    double totalOther = 0;
    for (int i = 0; i < drvoucher.rowCount(); i++) {
        double debet, credit;
        if (drvoucher.value(i, "f_sign").toInt() == 1) {
            debet =  drvoucher.value(i, "f_amountAmd").toDouble();
            credit = 0;
        } else {
            debet = 0;
            credit = drvoucher.value(i, "f_amountAmd").toDouble();
        }

        e.setValue(QString::number(rowNum++), row, 1);
        e.setValue(drvoucher.value(i, "f_wdate").toString(), row, 2);
        e.setValue(drvoucher.value(i, "f_finalName").toString() + " " + drvoucher.value(i, "f_remarks").toString(), row, 3);
        e.setValue("AMD", row, 4);

        int pMode = drvoucher.value(i, "f_paymentMode").toInt();
        if (drvoucher.value(i, "f_sign").toInt() <0 ) {
            switch (pMode) {
            case PAYMENT_CARD:
                totalCard += credit;
                break;
            case PAYMENT_CASH:
                totalCash += credit;
                break;
            default:
                totalOther += credit;
                break;
            }
        }
        totalVat += drvoucher.value(i, "f_amountVat").toDouble();
        totalCredit += credit;
        totalDebet += debet;
        lastBalance -= -1 * ((debet) - credit);
        e.setValue(float_printout(debet), row, 5);
        e.setValue(float_printout(credit), row, 6);
        e.setValue(float_printout(lastBalance), row, 7);
        row++;
    }
    e.setBorder("A" + QString::number(r1), "G" + QString::number(row - 1), 1);
    row++;

    //Footer
    r1 = row;
    e.setValue(tr("Total amount"), row, 3);
    e.mergeCells("C" + QString::number(row), "D" + QString::number(row), true);
    e.setHorizontalAlignment("C" + QString::number(row), "D" + QString::number(row), Excel::hRight);
    e.setValue(float_printout(totalDebet), row, 5);
    e.setValue(float_printout(totalCredit), row, 6);
    e.setValue(float_printout(lastBalance), row, 7);
    row++;
    e.setValue(tr("Total cash"), row, 3);
    e.mergeCells("C" + QString::number(row), "D" + QString::number(row), true);
    e.setHorizontalAlignment("C" + QString::number(row), "D" + QString::number(row), Excel::hRight);
    e.setValue(float_printout(totalCash), row, 5);
    row++;
    e.setValue(tr("Total cashless"), row, 3);
    e.mergeCells("C" + QString::number(row), "D" + QString::number(row), true);
    e.setHorizontalAlignment("C" + QString::number(row), "D" + QString::number(row), Excel::hRight);
    e.setValue(float_printout(totalCard + totalOther), row, 5);
    row++;
    e.setValue(tr("Being the equivalent of USD"), row, 3);
    e.mergeCells("C" + QString::number(row), "D" + QString::number(row), true);
    e.setHorizontalAlignment("C" + QString::number(row), "D" + QString::number(row), Excel::hRight);
    e.setValue(float_printout(totalDebet / def_usd), row, 5);
    e.setValue(float_printout(totalCredit / def_usd), row, 6);
    e.setValue(float_printout(lastBalance / def_usd), row, 7);
    row++;

    if (drheader.value("vatmode").toInt() == VAT_INCLUDED) {
        e.setValue(tr("VAT 20%"), row, 3);
        e.mergeCells("C" + QString::number(row), "D" + QString::number(row), true);
        e.setHorizontalAlignment("C" + QString::number(row), "D" + QString::number(row), Excel::hRight);
        e.setValue(float_printout(totalVat), row, 5);
        row++;;
    }
    e.setBorder("C" + QString::number(r1), "G" + QString::number(row - 1), 1);
    row++;

    e.setValue(tr("Guest signature"), row, 1);
    row++;
    e.setValue(tr("The sum of only ") + Utils::numberToWords(totalCredit), row, 1);
    e.mergeCells("A" + QString::number(row), "G" + QString::number(row), true);
    row++;
    e.setValue(fPreferences.getDb(def_vouchers_invoice_footer).toString(), row, 1);
    e.mergeCells("A" + QString::number(row), "G" + QString::number(row), true);
    e.show();
}
