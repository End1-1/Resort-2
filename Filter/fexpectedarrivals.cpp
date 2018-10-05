#include "fexpectedarrivals.h"
#include "ui_fexpectedarrivals.h"
#include "wreportgrid.h"
#include "dwselectorcardex.h"

FExpectedArrivals::FExpectedArrivals(QWidget *parent) :
    WFilterBase(parent),
    ui(new Ui::FExpectedArrivals)
{
    ui->setupUi(this);
    fReportGrid->setupTabTextAndIcon(tr("Expected arrivals/depatures"), ":/images/arrival.png");

    DWSelectorCardex *dockCardex = new DWSelectorCardex(this);
    dockCardex->configure();
    dockCardex->setSelector(ui->leCardexCode);
    connect(dockCardex, &DWSelectorCardex::cardex, [this](CI_Cardex *c) {
        dock<CI_Cardex, CacheCardex>(ui->leCardexCode, ui->leCardexName, c);
    });
    connect(ui->wd, &WDate2::changed, [this](){
        apply(fReportGrid);
    });

    GOExpextedArrivals g("Expected arrivals/depatures");
    Q_UNUSED(g);
}

FExpectedArrivals::~FExpectedArrivals()
{
    delete ui;
}

void FExpectedArrivals::apply(WReportGrid *rg)
{
    if (ui->rbEntry->isChecked()) {
        printArrival(rg);
    } else {
        printDeparture(rg);
    }
}

QWidget *FExpectedArrivals::firstElement()
{
    return ui->wd->fw();
}

QString FExpectedArrivals::reportTitle()
{
    return QString("%1 %2 From %3 To %4")
            .arg("Expected")
            .arg(ui->rbEntry->isChecked() ? tr("Arrivals") : ui->rbDeparture->isChecked() ? tr("Departures") : tr("Arrivals and departures"))
            .arg(ui->wd->ds1())
            .arg(ui->wd->ds2());
}

GOWidget *FExpectedArrivals::gridOptionWidget()
{
    return new GOExpextedArrivals("Expected arrivals/depatures");
}

void FExpectedArrivals::printArrival(WReportGrid *rg)
{
    rg->fModel->clearData();
    QList<int> cols;
    cols << 100 //id
         << 100 // room
         << 300 // guest
         << 30 // man
         << 30 // female
         << 30 // child
         << 50 // nigths
         << 150 // cardex
         << 80 // advance
         << 0 // cardex rate
         << 80 // rate
         << 30 // status
         << 100 //departure
            ;

    QStringList titles ;
    titles << tr("Voucher")
           << tr("Room")
           << tr("Guest")
           << tr("M")
           << tr("F")
           << tr("C")
           << tr("Nights")
           << tr("Cardex")
           << tr("Advance")
           << tr("CardexRate")
           << tr("Balance")
           << tr("RS")
           << tr("Departure");
    rg->fModel->clearColumns();
    QList<QVariant> emptyRow;
    for (int i = 0; i < cols.count(); i++) {
        rg->fModel->setColumn(cols.at(i), "", titles.at(i));
        emptyRow << QVariant();
    }
    QString where = QString ("where %1 between %2 and %3 %4")
            .arg(ui->rbEntry->isChecked() ? "r.f_startDate" : "r.f_endDate")
            .arg(ui->wd->ds1())
            .arg(ui->wd->ds2())
            .arg(ui->rbEntry->isChecked() ? " and r.f_state = 2 " : " and r.f_state in (1, 2) ");
    // - cdx.cardex_rate
    if (ui->leCardexCode->text().length() > 0) {
        where += " and r.f_cardex='" + ui->leCardexCode->text() + "' ";
    }
    QString query = "select r.f_startDate, r.f_id, rm.f_short, concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName), "
            "r.f_man, r.f_woman, r.f_child, datediff(r.f_endDate, r.f_startDate), cx.f_name, adv.total, '-', "
            "r.f_pricePerNight, "
            "left(s.f_en, 1), r.f_endDate, r.f_remarks "
            "from f_reservation r "
            "left join f_room rm on rm.f_id=r.f_room "
            "left join f_guests g on g.f_id=r.f_guest "
            "left join f_cardex cx on cx.f_cardex=r.f_cardex "
            "left join f_reservation_status s on s.f_id=r.f_reserveState "
            "left join (select a.f_res, sum(f_amountAmd) as total "
                "from m_register a where f_canceled=0 and f_source='AV' group by 1) adv on adv.f_res=r.f_id "
            + where +
            "order by " + GOExpextedArrivals::value("sort order", "Expected arrivals/depatures").toString();
            ;
    QList<int> entryRows;
    QList<int> mainRows;
    QList<int> addRows;
    QList<int> totalRows;
    QList<int> rowsHeights;
    int count = 0;
    int guestCount = 0;
    double totalAdv = 0;
    double totalPosted = 0;
    int r = 0;
    QDate entryDate;
    fDb.select(query, fDbBind, fDbRows);
    QList<QVariant> row;
    foreach_rows {
        if (entryDate != it->at(0).toDate()) {
            if (count > 0) {
                row = emptyRow;
                row[0] = tr("Total");
                row[1] = count;
                row[8] = totalAdv;
                row[10] = totalPosted;
                rg->fModel->appendRow(row);
                totalRows << r;
                r++;
                count = 0;
                totalAdv = 0;
                totalPosted = 0;
                rg->fModel->appendRow(emptyRow);
                entryRows << r;
                r++;
            }
            row = emptyRow;
            entryDate = it->at(0).toDate();
            row[0] = (ui->rbEntry->isChecked() ? tr("Entry date: ") : tr("Departure date: ")) + entryDate.toString(def_date_format);
            rg->fModel->appendRow(row);
            entryRows << r;
            r++;
        }
        totalAdv += it->at(9).toDouble();
        totalPosted += it->at(11).toDouble();
        guestCount += (it->at(4).toInt() + it->at(5).toInt() + it->at(6).toInt());
        row = emptyRow;
        for (int i = 1; i < 14; i++) {
            row[i - 1] = it->at(i);
        }
        rg->fModel->appendRow(row);
        mainRows << r;
        r++;
        if (it->at(14).toString().length() > 0) {
            row = emptyRow;
            row[0] = it->at(14).toString().replace("\r", " ").replace("\n", " ");;
            rg->fModel->appendRow(row);
            addRows << r;
            r++;
        }
        rg->fModel->appendRow(emptyRow);
        entryRows << r;
        rowsHeights << r;
        r++;
        count ++;
    }
    if (count > 0) {
        row = emptyRow;
        row[0] = tr("Total");
        row[1] = count;
        row[2] = tr("Total guests count") + ": " + QString::number(guestCount);
        row[8] = totalAdv;
        row[10] = totalPosted;
        rg->fModel->appendRow(row);
        totalRows << r;
        r++;
        count = 0;
        totalAdv = 0;
        totalPosted = 0;
        rg->fModel->appendRow(emptyRow);
        r++;
    }
   // rg->fModel->setSqlQuery("");
    rg->fModel->applyFinal(rg, true);
    foreach (int i, entryRows) {
        rg->fTableView->setSpan(i, 0, 1, 14);
    }
    foreach (int i, addRows) {
        rg->fTableView->setSpan(i, 0, 1, 14);
    }
    foreach (int i, totalRows) {
        rg->fTableView->setSpan(i, 2, 1, 6);
    }
    for (int i = 0, rowCount = rg->fModel->rowCount(); i < rowCount; i++) {
        rg->fTableView->setRowHeight(i, rg->fTableView->verticalHeader()->defaultSectionSize());
    }
    foreach (int i, rowsHeights) {
        rg->fTableView->setRowHeight(i, 10);
    }
    for (int i = 0, count = rg->fModel->rowCount(); i < count; i++) {
        if (rg->fModel->data(i, 0).toString().isEmpty()) {
            rg->fModel->setBackgroundColor(i, COLOR_DARK_ROW);
        }
    }
    if (rg->fModel->rowCount() == 0) {
        if (ui->rbEntry->isChecked() && ui->rbDeparture->isChecked()) {
            message_info_tr("There are no arrivals and departures");
            return;
        }
        if (ui->rbEntry->isChecked()) {
            message_info_tr("There are no arrivals");
        }
        if (ui->rbDeparture->isChecked()) {
            message_info_tr("There are no departures");
        }
    }
}

void FExpectedArrivals::printDeparture(WReportGrid *rg)
{
    rg->fModel->clearData();
    QList<int> cols;
    cols << 100 //id
         << 100 // room
         << 300 // guest
         << 30 // man
         << 30 // female
         << 30 // child
         << 50 // nigths
         << 150 // cardex
         << 80 // advance
         << 0 // cardex rate
         << 80 // rate
         << 30 // status
         << 100 //departure
            ;

    QStringList titles ;
    titles << tr("Voucher")
           << tr("Room")
           << tr("Guest")
           << tr("M")
           << tr("F")
           << tr("C")
           << tr("Nights")
           << tr("Cardex")
           << tr("Advance")
           << tr("CardexRate")
           << tr("Balance")
           << tr("RS")
           << tr("Departure");
    rg->fModel->clearColumns();
    QList<QVariant> emptyRow;
    for (int i = 0; i < cols.count(); i++) {
        rg->fModel->setColumn(cols.at(i), "", titles.at(i));
        emptyRow << QVariant();
    }
    QString where = QString ("where %1 between %2 and %3 and r.f_state=%4 ")
            .arg("r.f_endDate")
            .arg(ui->wd->ds1())
            .arg(ui->wd->ds2())
            .arg(RESERVE_CHECKIN);
    if (ui->leCardexCode->text().length() > 0) {
        where += " and r.f_cardex='" + ui->leCardexCode->text() + "' ";
    }
    // - cdx.cardex_rate
    QString query = "select r.f_startDate, r.f_id, rm.f_short, "
            "concat(g.f_title, ' ', g.f_firstName, ' ', g.f_lastName), "
            "r.f_man, r.f_woman, r.f_child, datediff(r.f_endDate, r.f_startDate), cx.f_name, adv.total, '-', "
            "bl.total, "
            "left(s.f_en, 1), r.f_endDate, r.f_remarks "
            "from f_reservation r "
            "inner join f_room rm on rm.f_id=r.f_room "
            "inner join f_guests g on g.f_id=r.f_guest "
            "left join f_cardex cx on cx.f_cardex=r.f_cardex "
            "inner join f_reservation_status s on s.f_id=r.f_reserveState "
            "left join (select a.f_res, sum(f_amountAmd) as total from m_register a where f_source='AV' and f_finance=1 and f_canceled=0 group by 1) adv on adv.f_res=r.f_id "
            "left join (select f_inv, sum(f_amountAmd*f_sign) as total from m_register where f_canceled=0 and f_finance=1 group by 1) bl on bl.f_inv=r.f_invoice "
            + where +
            "order by r.f_startDate, r.f_room "
            ;
    QList<int> entryRows;
    QList<int> mainRows;
    QList<int> addRows;
    QList<int> totalRows;
    QList<int> rowsHeights;
    int count = 0;
    int guestCount = 0;
    double totalAdv = 0;
    double totalPosted = 0;
    int r = 0;
    QDate entryDate;
    fDb.select(query, fDbBind, fDbRows);
    QList<QVariant> row;
    foreach_rows {
        if (entryDate != it->at(13).toDate()) {
            if (count > 0) {
                row = emptyRow;
                row[0] = tr("Total");
                row[1] = count;
                row[8] = totalAdv;
                row[10] = totalPosted;
                rg->fModel->appendRow(row);
                totalRows << r;
                r++;
                count = 0;
                totalAdv = 0;
                totalPosted = 0;
                rg->fModel->appendRow(emptyRow);
                entryRows << r;
                r++;
            }
            row = emptyRow;
            entryDate = it->at(13).toDate();
            row[0] = tr("Entry date: ") + entryDate.toString(def_date_format);
            rg->fModel->appendRow(row);
            entryRows << r;
            r++;
        }
        totalAdv += it->at(9).toDouble();
        totalPosted += it->at(11).toDouble();
        guestCount += (it->at(4).toInt() + it->at(5).toInt() + it->at(6).toInt());
        row = emptyRow;
        for (int i = 1; i < 14; i++) {
            row[i - 1] = it->at(i);
        }
        rg->fModel->appendRow(row);
        mainRows << r;
        r++;
        if (it->at(14).toString().length() > 0) {
            row = emptyRow;
            row[0] = it->at(14).toString().replace("\r", " ").replace("\n", " ");
            rg->fModel->appendRow(row);
            addRows << r;
            r++;
        }
        rg->fModel->appendRow(emptyRow);
        entryRows << r;
        rowsHeights << r;
        r++;
        count ++;
    }
    if (count > 0) {
        row = emptyRow;
        row[0] = tr("Total");
        row[1] = count;
        row[2] = tr("Total guests count") + ": " + QString::number(guestCount);
        row[8] = totalAdv;
        row[10] = totalPosted;
        rg->fModel->appendRow(row);
        totalRows << r;
        r++;
        count = 0;
        totalAdv = 0;
        totalPosted = 0;
        rg->fModel->appendRow(emptyRow);
        r++;
    }
    rg->fModel->applyFinal(rg, true);
    foreach (int i, entryRows) {
        rg->fTableView->setSpan(i, 0, 1, 14);
    }
    foreach (int i, addRows) {
        rg->fTableView->setSpan(i, 0, 1, 14);
    }
    foreach (int i, totalRows) {
        rg->fTableView->setSpan(i, 2, 1, 6);
    }
    for (int i = 0, rowCount = rg->fModel->rowCount(); i < rowCount; i++) {
        rg->fTableView->setRowHeight(i, rg->fTableView->verticalHeader()->defaultSectionSize());
    }
    foreach (int i, rowsHeights) {
        rg->fTableView->setRowHeight(i, 10);
    }
}
