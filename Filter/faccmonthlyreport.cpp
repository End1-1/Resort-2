#include "faccmonthlyreport.h"
#include "ui_faccmonthlyreport.h"
#include "wreportgrid.h"
#include "paymentmode.h"

FAccMonthlyReport::FAccMonthlyReport(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FAccMonthlyReport)
{
    ui->setupUi(this);
    setTabText();
    fReportGrid->fTableView->verticalHeader()->setDefaultSectionSize(18);
    ui->cbYear->setCurrentIndex(ui->cbYear->findData(QDate::currentDate().year()));
    ui->cbMonth->setCurrentIndex(QDate::currentDate().month() - 1);
    ui->deStart->setDate(QDate::currentDate().addDays((QDate::currentDate().day() - 1) * -1));
    ui->deEnd->setDate(QDate::currentDate());
}

FAccMonthlyReport::~FAccMonthlyReport()
{
    delete ui;
}

void FAccMonthlyReport::getInvoiceContent(QList<QList<QVariant> > &rows, QMap<QString, int> &dateMap, int col, const QString & item) {
    fDbBind[":f_date1"] = ui->deStart->date();
    fDbBind[":f_date2"] = ui->deEnd->date();
    QString date = "date_format(ic.f_wdate, '%d/%m/%x')";
    if (ui->rbWeek->isChecked()) {
        date = "date_format(ic.f_wdate, '%v, %m/%x')";
    }
    if (ui->rbMonth->isChecked()) {
        date = "date_format(ic.f_wdate, '%m/%x')";
    }
    if (ui->rbYear->isChecked()) {
        date = "date_format(ic.f_wdate, '%x')";
    }
    fDbBind[":f_canceled"] = (ui->chCanceled->isChecked() ? 1 : 0);
    fDb.select("select " + date + ", sum(abs(ic.f_amountAmd*f_sign)) "
               "from m_register ic "
               "where f_canceled=:f_canceled and f_wdate between :f_date1 and :f_date2 "
               "and ic.f_itemCode in (" + item + ") and ic.f_paymentMode<> " + QString::number(PAYMENT_COMPLIMENTARY) + " "
               "group by 1", fDbBind, fDbRows);
    foreach_rows {
        int destRow = dateMap[it->at(0).toString()];
        rows[destRow][col] = it->at(1);
    }
}

void FAccMonthlyReport::getGPOSContent(QList<QList<QVariant> > &rows, QMap<QString, int> &dateMap, int col, const QString &store)
{
    fDbBind[":f_date1"] = ui->deStart->date();
    fDbBind[":f_date2"] = ui->deEnd->date();
    QString date = "date_format(ic.f_date, '%d/%m/%x')";
    if (ui->rbWeek->isChecked()) {
        date = "date_format(o.f_dateCash, '%v, %m/%x')";
    }
    if (ui->rbMonth->isChecked()) {
        date = "date_format(o.f_dateCash, '%m/%x')";
    }
    if (ui->rbYear->isChecked()) {
        date = "date_format(o.f_dateCash, '%x')";
    }
    fDb.select("select " + date + ", sum(d.f_total) "
               "from o_dish d "
               "inner join o_header o on o.f_id=d.f_header "
               "where o.f_state=2 and d.f_state=1 and o.f_dateCash between :f_date1 and :f_date2 "
               "and d.f_store in (" + store + ") "
               "group by 1", fDbBind, fDbRows);
    foreach_rows {
        int destRow = dateMap[it->at(0).toString()];
        rows[destRow][col] = it->at(1);
    }
}

void FAccMonthlyReport::apply(WReportGrid *rg)
{
    DatabaseResult dr;
    dr.select(fDb, "select * from serv_monthly order by f_id", fDbBind);
    QMap<QString, QVariant> row;
    row["#"] = 0;
    row["date"] = 1;
    row["rooming"] = 2;
    row["extrabed"] = 3;
    row["minibar"] = 4;
    row["laundry"] = 5;
    row["phone"] =  6;
    row["conference"] = 7;
    row["other"] = 8;
    row["roscafe"] = 9;
    row["restaurant"] = 10;
    row["airport"] = 11;
    row["banq"] = 12;
    row["swiming"] = 13;
    row["total"] = 14;
    row["avroom"]  = 15;
    row["occroom"] = 16;
    row["free"] = 18;
    row["avgrate"] = 19;
    row["pax"] = 20;
    row["rev"] = 21;
    QList<int> totalColumns;
    totalColumns << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20;
    rg->fModel->clearColumns();
    rg->fModel->setColumn(30, "", tr("#")) //0
            .setColumn(80, "", tr("Date")); //1
    for (int i = 0; i < dr.rowCount(); i++) {
        rg->fModel->setColumn(dr.value(i, "f_width").toInt(), "", dr.value(i, "f_title").toString().replace("/", "\r\n"));
    }
    rg->fModel->setColumn(90, "", tr("Total")) //13
            .setColumn(50, "", tr("Av.\nroom")) //14
            .setColumn(50, "", tr("Occ.\nroom")) //15
            .setColumn(30, "", tr("Fr\nee")) //16
            .setColumn(70, "", tr("Avg.rate")) //17
            .setColumn(50, "", tr("Pax")) //18
            .setColumn(100, "", tr("REV")); //19;
    QList<QVariant> rowTemplate;
    for (int i = 0; i < row.count(); i++) {
        rowTemplate << QVariant("");
    }
    QMap<QString, int> dateMap;
    QList<QList<QVariant> > rows;
    QString dateToStr = "dd/MM/yyyy";
    if (ui->rbWeek->isChecked()) {
        dateToStr = "MM/yyyy";
    }
    if (ui->rbMonth->isChecked()) {
        dateToStr = "MM/yyyy";
    }
    if (ui->rbYear->isChecked()) {
        dateToStr = "yyyy";
    }
    for (QDate d = ui->deStart->date(); d < ui->deEnd->date().addDays(1); d = d.addDays(1)) {
        QString dateStr = d.toString(dateToStr);
        if (ui->rbWeek->isChecked()) {
            dateStr = QString::number(d.weekNumber()) + ", " + d.toString(dateToStr);
        }
        if (!dateMap.contains(dateStr)) {
            rows.append(rowTemplate);
            rows[rows.count() - 1][0] = rows.count();
            rows[rows.count() - 1][1] = dateStr;
            dateMap[dateStr] = rows.count() - 1;
        }
    }
    for (int i = 0; i < dr.rowCount(); i++) {
        getInvoiceContent(rows, dateMap, i + 2, dr.value(i, "f_items").toString());
    }
//    getInvoiceContent(rows, dateMap, 2, "1,66,69,11,12,14,15,65"); //rooming all rm
//    getInvoiceContent(rows, dateMap, 3, "13"); //extrabed
//    getInvoiceContent(rows, dateMap, 4, "16"); //minibar
//    getInvoiceContent(rows, dateMap, 5, "5"); //laundry
//    getInvoiceContent(rows, dateMap, 6, "2,3,4"); //Phone, fax
//    getInvoiceContent(rows, dateMap, 7, "7"); //Conf
//    getInvoiceContent(rows, dateMap, 8, "10,6"); // other, massage
//    getInvoiceContent(rows, dateMap, 9, "19"); //roscafe
//    getInvoiceContent(rows, dateMap, 10, "18"); //rest
//    getInvoiceContent(rows, dateMap, 11, "9"); //airport
//    getInvoiceContent(rows, dateMap, 12, "68,73"); //bank/events
//    getInvoiceContent(rows, dateMap, 13, "17"); //break
    //Total
    for (int i = 0; i < rows.count(); i++) {
        rows[i][14] = 0.0;
        for (int j = 2; j < 14; j++) {
            rows[i][14] = rows[i][14].toDouble() + rows[i][j].toDouble();
        }
        if (i > 0) {
            rows[i][20] = rows[i - 1][20].toDouble() + rows[i][14].toDouble();
        } else {
            rows[i][20] = rows[i][14];
        }

        if (rows[i][16].toDouble() > 0) {
            rows[i][18] = rows[i][2].toDouble() / rows[i][16].toDouble();
        }

    }

    QString date = "date_format(f_date, '%d/%m/%x')";
    if (ui->rbWeek->isChecked()) {
        date = "date_format(f_date, '%v, %m/%x')";
    }
    if (ui->rbMonth->isChecked()) {
        date = "date_format(f_date, '%m/%x')";
    }
    if (ui->rbYear->isChecked()) {
        date = "date_format(f_date, '%x')";
    }

    //Occupied date, av room rooms, rate, free room, pax
    fDbBind[":date1"] = ui->deStart->date();
    fDbBind[":date2"] = ui->deEnd->date();
    fDb.select("call occupied_room_in_range(:date1, :date2)", fDbBind, fDbRows);
    fDb.select("select " + date + ", sum(f_avroom), sum(f_room), sum(f_rate), sum(f_free), sum(f_pax) from map group by 1", fDbBind, fDbRows);
    foreach_rows {
        int destRow = dateMap[it->at(0).toString()];
        rows[destRow][15] = it->at(1);
        rows[destRow][16] = it->at(2);
        rows[destRow][17] = it->at(4);
        rows[destRow][18] = it->at(3);
        rows[destRow][19] = it->at(5);
    }
    fDb.select("drop temporary table map", fDbBind, fDbRows);

    for (int i = 0; i < rows.count(); i++) {

        if (rows[i][16].toDouble() > 0) {
            rows[i][18] = rows[i][2].toDouble() / rows[i][16].toDouble();
        }

    }

    rg->fModel->setDataFromSource(rows);
    QList<double> totalValues;
    rg->fModel->sumOfColumns(totalColumns, totalValues);
    totalValues[totalValues.count() - 1] = rows.at(rows.count() - 1).at(20).toDouble();
    rg->setTblTotalData(totalColumns, totalValues);

    if (fReportGrid->fTableTotal->toDouble(0, 16) > 0) {
        fReportGrid->fTableTotal->setItemWithValue(0, 18, fReportGrid->fTableTotal->toDouble(0, 2) / fReportGrid->fTableTotal->toDouble(0, 16));
    }

}

QWidget *FAccMonthlyReport::firstElement()
{
    return ui->deStart;
}

QString FAccMonthlyReport::reportTitle()
{
    QString text;
    if (ui->rbDay->isChecked()) {
        text = tr("By day");
    } else if (ui->rbWeek->isChecked()) {
        text = tr("By week");
    } else if (ui->rbMonth->isChecked()) {
        text = tr("By month");
    } else if (ui->rbYear->isChecked()) {
        text = tr("By year");
    }
    return QString("%1 From %2 To %3, %4 %5")
            .arg(tr("Monthly report"))
            .arg(ui->deStart->text())
            .arg(ui->deEnd->text())
            .arg(text)
            .arg(ui->chCanceled->isChecked() ? tr("Canceled") : "");
}

void FAccMonthlyReport::setTabText()
{
    fReportGrid->setupTabTextAndIcon(QString("%1 %2-%3")
                                     .arg(tr("Monthly report"))
                                     .arg(ui->deStart->date().toString(def_date_format))
                                     .arg(ui->deEnd->date().toString(def_date_format)),
                                     ":/images/graph.png");
}

void FAccMonthlyReport::setDateRange()
{
    QString d1 = QString("01/%1/%2")
            .arg(ui->cbMonth->currentIndex() + 1, 0, 10, QChar('0'))
            .arg(ui->cbYear->currentText());
    QDate date1 = QDate::fromString(d1, def_date_format);

    QString d2 = QString("%1/%2/%3")
            .arg(date1.daysInMonth())
            .arg(ui->cbMonth->currentIndex() + 1, 0, 10, QChar('0'))
            .arg(ui->cbYear->currentText());
    QDate date2 = QDate::fromString(d2, def_date_format);
    ui->deStart->setDate(date1);
    ui->deEnd->setDate(date2);
}

void FAccMonthlyReport::on_deStart_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
    setTabText();
}

void FAccMonthlyReport::on_deEnd_dateChanged(const QDate &date)
{
    Q_UNUSED(date)
    setTabText();
}

void FAccMonthlyReport::on_cbYear_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    setDateRange();
}

void FAccMonthlyReport::on_cbMonth_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    setDateRange();
}
