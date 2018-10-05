#include "wsyncinvoices.h"
#include "ui_wsyncinvoices.h"
#include "vauchers.h"
#include "dlgexportsinglevaucher.h"
#include "cacheinvoiceitem.h"
#include "dlgchartdaterange.h"
#include "dlgchangeexportvaucher.h"
#include "excel.h"
#include "tablemodel.h"
#include <QInputDialog>

WSyncInvoices::WSyncInvoices(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WSyncInvoices)
{
    ui->setupUi(this);
    ui->tblMain->setMaximumHeight(ui->tblMain->rowCount() * ui->tblMain->verticalHeader()->defaultSectionSize() + 25);
    ui->tblMain->setMinimumHeight(ui->tblMain->rowCount() * ui->tblMain->verticalHeader()->defaultSectionSize() + 25);
    ui->tblMain->setSpan(0, 2, ui->tblMain->rowCount(), 1);
    ui->tblPMain->setMaximumHeight(ui->tblPMain->rowCount() * ui->tblPMain->verticalHeader()->defaultSectionSize() + 25);
    ui->tblPMain->setMinimumHeight(ui->tblPMain->rowCount() * ui->tblPMain->verticalHeader()->defaultSectionSize() + 25);
    ui->tblPMain->setSpan(0, 2, ui->tblPData->rowCount(), 1);
    Utils::tableSetColumnWidths(ui->tblData, ui->tblData->columnCount(),
                                30, 30, 110, 200, 100, 30);
    Utils::tableSetColumnWidths(ui->tblPData, ui->tblPData->columnCount(),
                                30, 30, 110, 200, 100, 30, 0);
    Utils::tableSetColumnWidths(ui->tblMain, ui->tblMain->columnCount(), 100, 200, 20, 100, 100, 100);
    Utils::tableSetColumnWidths(ui->tblPMain, ui->tblPMain->columnCount(), 100, 200, 20, 100, 100, 100);
    Utils::tableSetColumnWidths(ui->tblSearch, ui->tblSearch->columnCount(), 80, 80, 300, 0);
    ui->tblSearch->setMinimumWidth(305);
    ui->tblSearch->setMaximumWidth(305);
    ui->wdtSearch->setMaximumWidth(ui->wdtSearch->width() - 10);
    ui->wdtSearch->setVisible(true);

    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return;
    }
    fSDb.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!fSDb.open()) {
        message_error_tr("Cannot connect to second database");
        return;
    }
}

WSyncInvoices::~WSyncInvoices()
{
    delete ui;
}

void WSyncInvoices::open()
{
    addTab<WSyncInvoices>()->setupTabTextAndIcon(tr("Export invoices"), ":/images/database.png");
}

void WSyncInvoices::loadInvoice(const QString &id)
{
    fDbBind[":f_invoice"] = id;
    fReserv.select(fDb, "select * from f_reservation where f_invoice=:f_invoice", fDbBind);
    if (fReserv.rowCount() == 0) {
        message_error_tr("Incorrect invoice id");
        return;
    }
    if (fReserv.value(0, "f_state").toInt() != RESERVE_CHECKOUT) {
        message_info_tr("Reservation state is not equal to checkout.");
        return;
    }

    fDbBind[":f_reservation"] = fReserv.value(0, "f_id");
    fReservGuests.select(fDb, "select f_reservation, f_guest, f_first from f_reservation_guests where f_reservation=:f_reservation", fDbBind);

    //QString guestIds;
    //fReservGuests.makeIn("f_guest", guestIds);
    fDbBind[":f_id"] = fReserv.value(0, "f_guest");
    fGuests.select(fDb, "select * from f_guests where f_id=:f_id", fDbBind);

    fDbBind[":f_inv"] = id;
    fVauchers.select(fDb, "select * from m_register where f_inv=:f_inv and f_canceled=0 order by f_wdate", fDbBind);

    ui->tblMain->setValue(0, 1, fReserv.value(0, "f_id"));
    ui->tblMain->setValue(1, 1, fGuests.value(0, "f_firstName").toString() + " " + fGuests.value(0, "f_lastName").toString());
    ui->tblMain->setValue(2, 1, fReserv.value(0, "f_startDate"));
    ui->tblMain->setValue(3, 1, fReserv.value(0, "f_endDate"));
    ui->tblMain->setValue(4, 1, fReserv.value(0, "f_invoice"));
    ui->tblMain->setValue(0, 4, fReserv.value(0, "f"));

    QFont boldFont(qApp->font());
    boldFont.setBold(true);
    ui->tblData->setRowCount(fVauchers.rowCount());
    for (int i = 0; i < fVauchers.rowCount(); i++) {
        ui->tblData->setRowHidden(i, false);
        ui->tblData->setItemWithValue(i, 0, "");
        ui->tblData->setItemChecked(i, 0, true);
        ui->tblData->item(i, 0)->setFlags(ui->tblData->item(i, 0)->flags() ^ Qt::ItemIsUserCheckable);
        ui->tblData->setItemWithValue(i, 1, fVauchers.value(i, "f_source"));
        ui->tblData->setItemWithValue(i, 2, fVauchers.value(i, "f_wdate"));
        ui->tblData->setItemWithValue(i, 3, fVauchers.value(i, "f_finalName"));
        ui->tblData->setItemWithValue(i, 4, fVauchers.value(i, "f_amountAmd"));
        ui->tblData->setItemWithValue(i, 5, "");
        ui->tblData->setItemChecked(i, 5, fVauchers.value(i, "f_fiscal").toInt() > 0);
        ui->tblData->item(i, 5)->setFlags(ui->tblData->item(i, 5)->flags() ^ Qt::ItemIsUserCheckable);
        if (fVauchers.value(i, "f_source").toString() == VAUCHER_ADVANCE_N
                || fVauchers.value(i, "f_source").toString() == VAUCHER_RECEIPT_N) {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setFont(boldFont);
            }
        }
        if (fVauchers.value(i, "p").toInt() > 0) {
            for (int j = 0; j < ui->tblData->columnCount(); j++) {
                ui->tblData->item(i, j)->setBackgroundColor(COLOR_VERY_DARK_ROW);
                ui->tblData->item(i, j)->setTextColor(Qt::white);
            }
        }
    }
    countBalance();
    ui->chFinance->setChecked(true);
    ui->chNoFinance->setChecked(false);
    on_chNoFinance_clicked(false);
    makeInvoiceCopy();
}

void WSyncInvoices::makeInvoiceCopy()
{
    QFont boldFont(qApp->font());
    boldFont.setBold(true);
    ui->tblPData->setRowCount(fVauchers.rowCount());
    for (int i = 0; i < fVauchers.rowCount(); i++) {
        ui->tblPData->setRowHidden(i, false);
        ui->tblPData->setItemWithValue(i, 0, "");
        ui->tblPData->setItemChecked(i, 0, true);
        ui->tblPData->setItemWithValue(i, 1, fVauchers.value(i, "f_source"));
        ui->tblPData->setItemWithValue(i, 2, fVauchers.value(i, "f_wdate"));
        ui->tblPData->setItemWithValue(i, 3, fVauchers.value(i, "f_finalName"));
        ui->tblPData->setItemWithValue(i, 4, fVauchers.value(i, "f_amountAmd"));
        ui->tblPData->setItemWithValue(i, 5, "");
        ui->tblPData->setItemChecked(i, 5, fVauchers.value(i, "f_fiscal").toInt() > 0);
        ui->tblPData->item(i, 5)->setFlags(ui->tblPData->item(i, 5)->flags() ^ Qt::ItemIsUserCheckable);
        ui->tblPData->setItemWithValue(i, 6, fVauchers.value(i, "f_finance"));
        if (fVauchers.value(i, "f_source").toString() == VAUCHER_ADVANCE_N
                || fVauchers.value(i, "f_source").toString() == VAUCHER_RECEIPT_N) {
            for (int j = 0; j < ui->tblPData->columnCount(); j++) {
                ui->tblPData->item(i, j)->setFont(boldFont);
            }
        }
    }
    countPBalance();
    ui->chPFinance->setChecked(true);
    ui->chPNoFinance->setChecked(false);
    on_chPNoFinance_clicked(false);
}

void WSyncInvoices::exportInvoice()
{

    Database db;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return;
    }
    db.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!db.open()) {
        message_error_tr("Cannot connect to second database");
        return;
    }
}

void WSyncInvoices::countBalance()
{
    double debit = 0, credit = 0;
    for (int i = 0; i < fVauchers.rowCount(); i++) {
        if (fVauchers.value(i, "f_finance").toInt() == 0) {
            continue;
        }
        if (fVauchers.value(i, "f_sign").toInt() > 0) {
            debit += fVauchers.value(i, "f_amountAmd").toDouble();
        } else {
            credit += fVauchers.value(i, "f_amountAmd").toDouble();
        }
    }
    ui->tblMain->setItemWithValue(5, 1, debit);
    ui->tblMain->setItemWithValue(5, 4, credit);
}

void WSyncInvoices::countPBalance()
{
    double debit = 0, credit = 0;
    for (int i = 0; i < ui->tblPData->rowCount(); i++) {
        if (ui->tblPData->item(i, 0)->checkState() != Qt::Checked) {
            continue;
        }
        if (ui->tblPData->toInt(i, 6) != 1) {
            continue;
        }
        if (ui->tblPData->toString(i, 1) == VAUCHER_ADVANCE_N
                || ui->tblPData->toString(i, 1) == VAUCHER_RECEIPT_N) {
            credit += ui->tblPData->toDouble(i, 4);
        } else {
            debit += ui->tblPData->toDouble(i, 4);
        }
    }
    if (credit == debit) {
        ui->lbSemaphore->setPixmap(QPixmap(":/images/ball-green.png"));
    } else {
        ui->lbSemaphore->setPixmap(QPixmap(":/images/ball-red.png"));
    }
    ui->tblPMain->setItemWithValue(5, 1, debit);
    ui->tblPMain->setItemWithValue(5, 4, credit);
}

void WSyncInvoices::on_leInvoice_returnPressed()
{
    loadInvoice(ui->leInvoice->text());
}

void WSyncInvoices::on_chFinance_clicked(bool checked)
{
    for (int i = 0; i < fVauchers.rowCount(); i++) {
        if (fVauchers.value(i, "f_finance").toInt() == 1) {
            ui->tblData->setRowHidden(i, !checked);
        }
    }
}

void WSyncInvoices::on_chNoFinance_clicked(bool checked)
{
    for (int i = 0; i < fVauchers.rowCount(); i++) {
        if (fVauchers.value(i, "f_finance").toInt() == 0) {
            ui->tblData->setRowHidden(i, !checked);
        }
    }
}

void WSyncInvoices::on_chPNoFinance_clicked(bool checked)
{
    for (int i = 0; i < ui->tblPData->rowCount(); i++) {
        if (ui->tblPData->toInt(i, 6) == 0) {
            ui->tblPData->setRowHidden(i, !checked);
        }
    }
}

void WSyncInvoices::on_chPFinance_clicked(bool checked)
{
    for (int i = 0; i < ui->tblPData->rowCount(); i++) {
        if (ui->tblPData->toInt(i, 6) == 1) {
            ui->tblPData->setRowHidden(i, !checked);
        }
    }
}

void WSyncInvoices::on_tblPData_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    if (index.column() == 0) {
        countPBalance();
    }
}

void WSyncInvoices::on_deDate_returnPressed()
{
    fDbBind[":f_endDate"] = ui->deDate->date();
    fDbBind[":f_state"] = RESERVE_CHECKOUT;
    fDb.select("select r.f_invoice, r.f_room, concat(g.f_firstName, ' ', g.f_lastName), f, "
               "a.amount "
               "from f_reservation r "
               "left join f_guests g on g.f_id=r.f_guest "
               "left join (select f_inv, sum(f_amountAmd) as amount "
                    "from m_register where f_canceled=0 and f_finance=1 and f_sign=1 "
                    "group by 1) a on a.f_inv=r.f_invoice "
               "where r.f_endDate=:f_endDate and r.f_state=:f_state "
               "order by r.f_room", fDbBind, fDbRows);
    Utils::fillTableWithData(ui->tblSearch, fDbRows);
    for (int i = 0; i < ui->tblSearch->rowCount(); i++) {
        if (ui->tblSearch->toInt(i, 3) > 0) {
            for (int j = 0; j < ui->tblSearch->columnCount(); j++) {
                ui->tblSearch->item(i, j)->setBackgroundColor(COLOR_VERY_DARK_ROW);
                ui->tblSearch->item(i, j)->setTextColor(Qt::white);
            }
        } else {
            for (int j = 0; j < ui->tblSearch->columnCount(); j++) {
                ui->tblSearch->item(i, j)->setBackgroundColor(Qt::white);
                ui->tblSearch->item(i, j)->setTextColor(Qt::black);
            }
        }
    }
}

void WSyncInvoices::on_btnSearchInvoice_clicked()
{
    ui->wdtSearch->setVisible(true);
    ui->deDate->setFocus();
}

void WSyncInvoices::on_tblSearch_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->leInvoice->setText(ui->tblSearch->toString(index.row(), 0));
    on_leInvoice_returnPressed();
    if (!ui->chDontHideSearch->isChecked()) {
        ui->wdtSearch->setVisible(false);
    }
}

void WSyncInvoices::on_btnSaveCopy_clicked()
{
    bool result = true;
    QString errors;
    fDb.fDb.transaction();
    fSDb.fDb.transaction();
    if (fReserv.value(0, "f").toInt() > 0) {
        if (message_confirm_tr("This invoice already exported. Export again?") != QDialog::Accepted) {
            return;
        }
    }
    QDate minDate = QDate::currentDate();
    QDate maxDate = QDate::currentDate().addDays(-360);
    QDate maxDate2 = QDate::currentDate().addDays(-360);
    QMap<QString, QVariant> sdbBind;

    QMap<QString, QVariant> tb;
    QList<QList<QVariant> > trr;

    /* RESERVATION */
    fReserv.getBindValues(0, sdbBind);
    QString reservCode = sdbBind[":f_id"].toString();
    if (ui->chOverrideDuplicate->isChecked()) {
        fSDb.select("delete from f_reservation where f_id=" + ap(reservCode), tb, trr);
    }
    result = result && fSDb.insertWithoutId("f_reservation", sdbBind);
    if (!result) {
        errors += "[Dst f_reservation] " + fSDb.fLastError + "<br>";
    }

    /* RESERVATION GUESTS */
    tb[":f_reservation"] = reservCode;
    fSDb.select("delete from f_reservation_guests where f_reservation=:f_reservation", tb, trr);
    for (int i = 0; i < fReservGuests.rowCount(); i++) {
        fReservGuests.getBindValues(i, sdbBind);
        result = result && fSDb.insertWithoutId("f_reservation_guests", sdbBind);
        if (!result) {
            errors += "[Dst] " + fSDb.fLastError + "<br>";
            break;
        }
    }

    /* VAUCHERS */
    QStringList fPSCodes;
    for (int i = 0; i < fVauchers.rowCount(); i++) {
        if (ui->tblPData->item(i, 0)->checkState() != Qt::Checked) {
            continue;
        }
        fVauchers.getBindValues(i, sdbBind);
        if (sdbBind[":f_cityLedger"].toInt() > 0) {
            QMap<QString, QVariant> be;
            be[":f_id"] = sdbBind[":f_cityLedger"];
            DatabaseResult drbe;
            drbe.select(fSDb, "select * from f_city_ledger where f_id=:f_id", be);
            if (drbe.rowCount() == 0) {
                result = false;
                message_error(tr("City ledger is not exists in destination database"));
            } else {
                if (sdbBind[":f_source"].toString() == VAUCHER_CHECKOUT_N
                        || sdbBind[":f_source"].toString() == VAUCHER_REFUND_N
                        || sdbBind[":f_source"].toString() == VAUCHER_RECEIPT_N
                        || sdbBind[":f_source"].toString() == VAUCHER_ROOM_RATE_N
                        || sdbBind[":f_source"].toString() == VAUCHER_POINT_SALE_N) {
                    sdbBind[":f_paymentComment"] = drbe.value("f_name").toString();

                    if (sdbBind[":f_source"].toString() == VAUCHER_RECEIPT_N) {
                        sdbBind[":f_finalName"] = tr("PAYMENT") + " " + drbe.value("f_name").toString();
                    }
                    if (sdbBind[":f_source"].toString() == VAUCHER_REFUND_N) {
                        sdbBind[":f_finalName"] = tr("REFUND") + " " + drbe.value("f_name").toString();
                    }
                }
            }
        }
        QString mregCode = sdbBind[":f_id"].toString();
        if (sdbBind[":f_source"] == VAUCHER_ROOMING_N) {
            minDate = sdbBind[":f_wdate"].toDate() < minDate ? sdbBind[":f_wdate"].toDate() : minDate;
            maxDate = sdbBind[":f_wdate"].toDate() > maxDate ? sdbBind[":f_wdate"].toDate() : maxDate;
        }
        maxDate2 = sdbBind[":f_wdate"].toDate() > maxDate2 ? sdbBind[":f_wdate"].toDate() : maxDate2;

        if (sdbBind[":f_source"].toString() == VAUCHER_POINT_SALE_N) {
            fPSCodes.append(sdbBind[":f_id"].toString());
        }

        if (ui->chOverrideDuplicate->isChecked()) {
            fSDb.select("delete from m_register where f_id=" + ap(sdbBind[":f_id"].toString()), tb, trr);
        }
        result = result && fSDb.insertWithoutId("m_register", sdbBind);
        if (!result) {
            errors += "[Dst m_register] " + fSDb.fLastError + "<br>";
            break;
        }

        sdbBind[":p"] = 1;
        result = result && fDb.update("m_register", sdbBind, where_id(ap(mregCode)));
        if (!result) {
            errors += "[Src] " + fDb.fLastError + "<br>";
        }

    }
    /* POINT OF SALE */
    foreach (QString s, fPSCodes) {
        DatabaseResult dHeader, dDish;
        /* GET FROM MAIN */
        fDbBind[":f_id"] = s;
        dHeader.select(fDb, "select * from o_header where f_id=:f_id", fDbBind);
        fDbBind[":f_header"] = s;
        fDbBind[":f_state"] = DISH_STATE_READY;
        dDish.select(fDb, "select * from o_dish where f_header=:f_header and f_state=:f_state", fDbBind);
        /* EXPORT COPY */
        dHeader.getBindValues(0, sdbBind);
        if (ui->chOverrideDuplicate->isChecked()) {
            fSDb.select("delete from o_header where f_id=" + ap(s), tb, trr);
        }
        result = result && fSDb.insertWithoutId("o_header", sdbBind);
        if (!result) {
            errors += "[Dst o_header] " + fSDb.fLastError + "<br>";
        }

        for (int i = 0; i < dDish.rowCount(); i++) {
            dDish.getBindValues(i, sdbBind);
            if (ui->chOverrideDuplicate->isChecked()) {
                fSDb.select("delete from o_dish where f_id=" + ap(sdbBind[":f_id"].toString()), tb, trr);
            }
            result = result && fSDb.insertWithoutId("o_dish", sdbBind);
            if (!result) {
                errors += "[Dst o_dish] " + fSDb.fLastError + "<br>";
                break;
            }
        }
        fDbBind[":f"] = 1;
        result = result && fDb.update("o_header", fDbBind, where_id(ap(s)));
        if (!result) {
            errors += "[Src] " + fSDb.fLastError + "<br>";
        }

    }
    sdbBind.clear();
    sdbBind[":f_startDate"] = minDate;
    sdbBind[":f_checkInDate"] = minDate;
    sdbBind[":f_endDate"] = maxDate2;
    result = result && fSDb.update("f_reservation", sdbBind, where_id(ap(reservCode)));
    if (!result) {
        errors += "[Dst f_reservation] " + fSDb.fLastError + "<br>";
    }

    /* GUESTS */
    fDbBind[":f_reservation"] = reservCode;
    DatabaseResult guests;
    guests.select(fDb, "select * from f_guests where f_id in "
                  "(select f_guest from f_reservation_guests where f_reservation=:f_reservation)", fDbBind);
    for (int i = 0; i < guests.rowCount(); i++) {
        guests.getBindValues(i, sdbBind);
        int id = sdbBind[":f_id"].toInt();
        fDbBind[":f_id"] = id;
        fSDb.select("delete from f_guests where f_id=:f_id", fDbBind, fDbRows);
        fSDb.insertWithoutId("f_guests", sdbBind);
    }

    fDbBind[":f"] = 1;
    fDb.update("f_reservation", fDbBind, where_id(ap(reservCode)));
    /* SET COLOR IN SEARCH TABLE, IF INVOICE NUMBER EXISTS */
    for (int i = 0; i < ui->tblSearch->rowCount(); i++) {
        if (ui->tblSearch->toString(i, 0) == ui->tblMain->toString(4, 1)) {
            for (int j = 0; j < ui->tblSearch->columnCount(); j++) {
                ui->tblSearch->item(i, j)->setBackgroundColor(COLOR_VERY_DARK_ROW);
                ui->tblSearch->item(i, j)->setTextColor(Qt::white);
            }
        }
    }
    if (result) {
        fDb.fDb.commit();
        fSDb.fDb.commit();
        message_info_tr("Export completed");
        if (maxDate.daysTo(maxDate2) > 1) {
            message_info(tr("Difference between departure date and last charge more the one!"));
        }
    } else {
        fDb.fDb.rollback();
        fSDb.fDb.rollback();
        message_error("<b><font color=\"red\">Export failed. </font></b><br>" + errors);
    }
}

void WSyncInvoices::on_btnExportSingleVaucher_clicked()
{
    QModelIndexList sel = ui->tblData->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        message_error_tr("Nothing is selected");
        return;
    }
    int result = DlgExportSingleVaucher::exportSingleVaucher(fVauchers.value(sel.at(0).row(), "f_id").toString());
    if (result == QDialog::Accepted) {
        ui->tblData->selectionModel()->clear();
        for (int i = 0; i < ui->tblData->columnCount(); i++) {
            ui->tblData->item(sel.at(0).row(), i)->setBackgroundColor(COLOR_VERY_DARK_ROW);
            ui->tblData->item(sel.at(0).row(), i)->setTextColor(Qt::white);
        }
    }
}

void WSyncInvoices::on_tblPData_doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QMap<QString, QVariant> bind;
    fVauchers.getBindValues(index.row(), bind);
    switch (index.column()) {
    case 2: {
        QDate date = bind[":f_wdate"].toDate();
        if (DlgChartDateRange::getDate(date)) {
            fVauchers.setValue(index.row(), "f_wdate", date);
            fVauchers.setValue(index.row(), "f_rdate", date);
            ui->tblPData->setItemWithValue(index.row(), 2, date);
        }
        break;
        }
    case 3: {
        DlgChangeExportVaucher *d = new DlgChangeExportVaucher(fVauchers, this);
        d->setDatabaseResult(index.row());
        if (d->exec()) {
            ui->tblPData->setItemWithValue(index.row(), 2, fVauchers.value(index.row(), "f_wdate"));
            ui->tblPData->setItemWithValue(index.row(), 3, fVauchers.value(index.row(), "f_finalName"));
            ui->tblPData->setItemWithValue(index.row(), 4, fVauchers.value(index.row(), "f_amountAmd"));
            ui->tblPData->setItemWithValue(index.row(), 1, fVauchers.value(index.row(), "f_source"));
            countPBalance();
        }
        delete d;
        break;
    }
    case 4:
        double newAmount = QInputDialog::getDouble(this, tr("New amount"), tr("New amount"), bind[":f_amountAmd"].toDouble());
        if (newAmount < 1) {
            return;
        }
        fVauchers.setValue(index.row(), "f_amountAmd", newAmount);
        ui->tblPData->setItemWithValue(index.row(), 4, newAmount);
        countPBalance();
    }
}

void WSyncInvoices::on_btnCloseSearch_clicked()
{
    ui->wdtSearch->setVisible(false);
}

void WSyncInvoices::on_btnNextDate_clicked()
{
    ui->deDate->setDate(ui->deDate->date().addDays(1));
    on_deDate_returnPressed();
}

void WSyncInvoices::on_btnPrevDate_clicked()
{
    ui->deDate->setDate(ui->deDate->date().addDays(-1));
    on_deDate_returnPressed();
}

void WSyncInvoices::on_btnExcel_clicked()
{
    Excel e;
    int colCount = ui->tblSearch->columnCount();
    int rowCount = ui->tblSearch->rowCount();
//    for (int i = 0; i < colCount; i++) {
//        e.setValue(fModel->columnTitle(i), 1, i + 1);
//        e.setColumnWidth(i + 1, fModel->columnWidth(i) / 7);
//    }
//    e.setFontBold(e.address(0, 0), e.address(0, colCount - 1));
//    e.setHorizontalAlignment(e.address(0, 0), e.address(0, colCount - 1), Excel::hCenter);

    for (int j = 0; j < rowCount; j++) {
        for (int i = 0; i < colCount; i++) {
            e.setValue(ui->tblSearch->toString(j, i), j + 2, i + 1);
        }
        QColor color = ui->tblSearch->item(j, 0)->backgroundColor();
        e.setBackground(e.address(j, 0), e.address(j, colCount - 1),
                         color.red(), color.green(), color.blue());
        QColor fcolor = ui->tblSearch->item(j, 0)->textColor();
        e.setFontColor(e.address(j, 0), e.address(j, colCount - 1),
                       fcolor.red(), fcolor.green(), fcolor.blue());

    }
    e.setFontSize(e.address(0, 0), e.address(rowCount - 1, colCount - 1), 10);
    e.show();
}
