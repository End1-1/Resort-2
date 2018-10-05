#include "dlgendofday.h"
#include "ui_dlgendofday.h"
#include "cacheinvoiceitem.h"
#include "wreservation.h"
#include "winvoice.h"
#include "paymentmode.h"
#include "vauchers.h"
#include "databaseresult.h"

DlgEndOfDay::DlgEndOfDay(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgEndOfDay)
{
    ui->setupUi(this);
    Utils::tableSetColumnWidths(ui->tblCharges, 12, 0, 0, 0, 80, 250, 80, 80, 120, 120, 100, 100, 0);
    fCanCharge = true;
    ui->lbLastDate->setText(QString("%1 - %2")
                            .arg(tr("Last charge date"))
                            .arg(WORKING_DATE.addDays(-1).toString(def_date_format)));
    ui->lbCurrentDate->setText(QString("%1 - %2")
                            .arg(tr("Current charge date"))
                            .arg(WORKING_DATE.toString(def_date_format)));
    loadData();
    fTrackControl = new TrackControl(TRACK_RESERVATION);
}

DlgEndOfDay::~DlgEndOfDay()
{
    delete ui;
}

void DlgEndOfDay::openReservation(const QString &id)
{
    WReservation::openReserveWindows(id);
}

void DlgEndOfDay::openInvoice(const QString &id)
{
    WInvoice::openInvoiceWindow(id);
}

void DlgEndOfDay::on_btnCancel_clicked()
{
    reject();
}

void DlgEndOfDay::on_btnOk_clicked()
{
    if (!fCanCharge) {
        message_error_tr("Unable to complete automatic charging, check the booking");
        return;
    }
    fCompanySideItems.clear();
    /*
    fDbBind[":f_date"] = WORKING_DATE.addDays(1);
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_id, ri.f_itemId "

                "inner join f_reservation r on r.f_id=rm.f_reservation "
                "inner join f_reservation_item_side ri on ri.f_reservation=r.f_id "
                "where rm.f_date=:f_date and r.f_state=:f_state ", fDbBind, fDbRows);
    foreach_rows {
        fCompanySideItems[it->at(0).toInt()].append(it->at(1).toInt());
    }
    */
    QString query;
    bool first = true;
    bool result = true;
    fDb.fDb.transaction();
    for (int i = 0, count = ui->tblCharges->rowCount(); i < count; i++) {

        double amount = ui->tblCharges->item(i, 8)->data(Qt::EditRole).toFloat();
        double vatAmount = 0;
        int vat = ui->tblCharges->item(i, 1)->data(Qt::EditRole).toInt();
        switch (vat) {
        case VAT_NOVAT:
            break;
        case VAT_INCLUDED:
            vatAmount = amount - (amount / ((20.0 / 100.0) + 1));
            break;
        case VAT_WITHOUT:
            vatAmount = amount * (20.0 / 100.0);
            break;
        }

        QString invoice = ui->tblCharges->item(i, 0)->data(Qt::EditRole).toString();
        QString rid = uuid(VAUCHER_ROOMING_N, fAirDb);
        result = fDb.insertId("m_register", rid);
        if (!result) {
            break;
        }
        fDbBind[":f_source"] = VAUCHER_ROOMING_N;
        fDbBind[":f_res"] = ui->tblCharges->toString(i, 2);
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_rdate"] = QDate::currentDate();
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_room"] = ui->tblCharges->item(i, 3)->data(Qt::EditRole).toInt();
        fDbBind[":f_guest"] = ui->tblCharges->item(i, 4)->data(Qt::EditRole).toString();
        fDbBind[":f_itemCode"] = fPreferences.getDb(def_room_charge_id);
        fDbBind[":f_finalName"] = QString("%1 %2 - %3")
                .arg(CacheInvoiceItem::instance()->get(fPreferences.getDb(def_room_charge_id).toString())->fName)
                .arg(WORKING_DATE.toString(def_date_format))
                .arg(ui->tblCharges->item(i, 11)->data(Qt::EditRole).toString());
        fDbBind[":f_amountAmd"] = amount;
        fDbBind[":f_amountVat"] = vatAmount;
        fDbBind[":f_amountUsd"] = def_usd;
        fDbBind[":f_fiscal"] = 0;
        fDbBind[":f_paymentMode"] = PAYMENT_CREDIT;
        fDbBind[":f_creditCard"] = 0;
        fDbBind[":f_cityLedger"] = 0;
        fDbBind[":f_paymentComment"] = "";
        fDbBind[":f_dc"] = "DEBIT";
        fDbBind[":f_sign"] = 1;
        fDbBind[":f_doc"] = "";
        fDbBind[":f_rec"] = "";
        fDbBind[":f_inv"] = invoice;
        fDbBind[":f_finance"] = 1;
        fDbBind[":f_remarks"] = "";
        fDbBind[":f_canceled"] = 0;
        fDbBind[":f_cancelReason"] = "";
        fDbBind[":f_side"] = (fCompanySideItems[ui->tblCharges->item(i, 2)->data(Qt::EditRole).toInt()].contains(ui->tblCharges->item(i, 2)->data(Qt::EditRole).toInt()) ? 1 : 0);
        fDbBind[":f_cash"] = 0;
        result = result && fDb.update("m_register", fDbBind, where_id(ap(rid)));
        fTrackControl->resetChanges();
        fTrackControl->fRecord = rid;
        fTrackControl->fInvoice = invoice;
        fTrackControl->fReservation = ui->tblCharges->toString(i, 2);
        fTrackControl->insert("EOD ROOMING", "", QString("%1 %2AMD")
                .arg(ui->tblCharges->item(i, 3)->data(Qt::EditRole).toInt())
                .arg(amount));
        if (!result) {
            break;
        }
    }

    query = QString("update f_global_settings set f_value='%1' where f_settings=1 and f_key='%2'")
            .arg(WORKING_DATE.addDays(1).toString(def_date_format))
            .arg(def_working_day);
    if (result) {
        result = fDb.queryDirect(query);
    }

    /* --------------- BEGIN ROOM VACANCY HISTORY ---------------- */
    fDb.select("select f_id, f_state from f_room", fDbBind, fDbRows);
    first = true;
    query = "insert into f_room_vacancy (f_room, f_date, f_state) values ";
    QListIterator<QList<QVariant> > it(fDbRows);
    while (it.hasNext()) {
        QList<QVariant> row = it.next();
        if (first) {
            first = false;
        } else {
            query += ",";
        }
        query += QString("(%1, '%2', %3)")
                .arg(row.at(0).toInt())
                .arg(WORKING_DATE.toString(def_mysql_date_format))
                .arg(row.at(1).toInt());
    }
    if (!first && result) {
        result = result && fDb.queryDirect(query);
    }
    /* --------------- END ROOM VACANCY HISTORY ---------------- */

    /* --------------- BEGIN GUESTS COUNT HISTORY ----------------*/
    fDb.select("select f_reservation, f_guest from f_reservation_guests "
               "where f_reservation in (select f_id from f_reservation where f_state=1)", fDbBind, fDbRows);
    query = "insert into f_reservation_guests_history (f_date, f_reservation,f_guest) values ";
    first = true;
    foreach_rows {
        if (first) {
            first = false;
        } else {
            query += ",";
        }
        query += QString("('%1', '%2', %3)")
                .arg(WORKING_DATE.toString(def_mysql_date_format))
                .arg(it->at(0).toString())
                .arg(it->at(1).toInt());
    }
    if (result) {
        result = result && fDb.queryDirect(query);
    }
    /* --------------- END GUESTS COUNT HISTORY ----------------*/

    /*------- CORRECT DATE CASH FOR OPENED ORDERS AT RESTAURANT-------*/
    if (result) {
        fDbBind[":f_dateCash"] = WORKING_DATE.addDays(1);
        result = result && fDb.update("o_header", fDbBind, where_field("f_state", 1));
    }
    /*------- END CORRECT DATE CASH FOR OPENED ORDERS AT RESTAURANT-------*/

    /*-------------------- MEAL ---------------------------------------*/
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_id, f_man + f_woman + f_child "
               "from f_reservation r "
               "where r.f_state=:f_state ", fDbBind, fDbRows);
    query = "";
    first = true;
    foreach_rows {
        if (first) {
            first = false;
        } else {
            query += ",";
        }
        query += QString("('%1', '%2', %3, %4, %5)")
                .arg(it->at(0).toInt())
                .arg(WORKING_DATE.addDays(1).toString(def_mysql_date_format))
                .arg(it->at(1).toString())
                .arg(0)
                .arg(0)
                ;
    }
    if (!query.isEmpty()) {
        query = "insert into f_reservation_meal (f_reservation, f_date, f_guests, f_show, f_doc) values "
                + query;
        result = result && fDb.queryDirect(query);
    }
    /*-------------------- END MEAL ---------------------------------------*/


    /*-------------------- CURRENCIES RATES ----------------------*/
    if (result) {
        fDb.select("select f_id, f_rate from f_acc_currencies", fDbBind, fDbRows);
        QStringList l;
        foreach_rows {
            l << QString("insert into f_acc_currency_rate (f_date, f_currency, f_rate) values ('%1', %2, %3)")
                 .arg(WORKING_DATE.toString(def_mysql_date_format))
                 .arg(it->at(0).toInt())
                 .arg(it->at(1).toDouble());
        }
        foreach (QString s, l) {
            fDb.queryDirect(s);
        }
    }

    /* SET AUTOMATICALLY TAX FISCAL FLAG */
    fDbBind[":f_itemCode"] = fPreferences.getDb(def_room_charge_id).toInt();
    fDbBind[":f_wdate"] = WORKING_DATE;
    fDb.select("select f_id, f_inv, f_amountAmd from m_register where f_itemCode=:f_itemCode "
               "and f_wdate=:f_wdate and f_fiscal=0 and f_canceled=0", fDbBind, fDbRows);
    /* OLD VERSION */
    DatabaseResult tax;
    tax.select(fDb, "select f_id, f_invoice, f_amount, f_source from m_free_tax where f_used=0 and f_source='RM' and f_taxCode <= 1", fDbBind);
    foreach_rows {
        QString inv = it->at(1).toString();
        for (int i = 0; i < tax.rowCount(); i++) {
            if (inv == tax.value(i, "f_invoice").toString() && it->at(2).toDouble() == tax.value(i, "f_amount").toDouble()) {
                fDbBind[":f_fiscal"] = 1;
                fDb.update("m_register", fDbBind, where_id(ap(it->at(0).toString())));
                fDbBind[":f_used"] = 1;
                fDb.update("m_free_tax", fDbBind, where_id(tax.value(i, "f_id").toInt()));
                tax.removeRow(i);
                break;
            }
        }
    }
    /* NEW VERSION */
    tax.select(fDb, "select f_id, f_invoice, f_amount, f_source, f_taxCode from m_free_tax where f_used=0 and f_source='RM' and f_taxCode>1", fDbBind);
    foreach_rows {
        QString inv = it->at(1).toString();
        for (int i = 0; i < tax.rowCount(); i++) {
            if (inv == tax.value(i, "f_invoice").toString() && it->at(2).toDouble() == tax.value(i, "f_amount").toDouble()) {
                fDbBind[":f_fiscal"] = tax.value(i, "f_taxCode");
                fDb.update("m_register", fDbBind, where_id(ap(it->at(0).toString())));
                fDbBind[":f_used"] = 1;
                fDb.update("m_free_tax", fDbBind, where_id(tax.value(i, "f_id").toInt()));
                tax.removeRow(i);
                break;
            }
        }
    }

    /* ROOM STATE. OUT OF ORDER AND OUT OF INVENTORY */
    DatabaseResult dbOO;
    fDbBind[":f_state1"] = RESERVE_OUTOFINVENTORY;
    fDbBind[":f_state2"] = RESERVE_OUTOFROOM;
    fDbBind[":f_startDate"] = WORKING_DATE;
    dbOO.select(fDb, "select f_room, f_state from f_reservation where f_state=:f_state1 or f_state=:f_state2 "
                " and f_startDate=:f_startDate", fDbBind);
    for (int i = 0; i < dbOO.rowCount(); i++) {
        int state = 0;
        switch (dbOO.value(i, "f_state").toInt()) {
        case RESERVE_OUTOFINVENTORY:
            state = ROOM_STATE_OUTINVENTORY;
            break;
        case RESERVE_OUTOFROOM:
            state = ROOM_STATE_OUTOF;
            break;
        }
        DatabaseResult droom;
        fDbBind[":f_room"] = dbOO.value(i, "f_room");
        droom.select(fDb, "select f_state from f_room where f_id=:f_room", fDbBind);
        fDbBind[":f_state"] = state;
        fDb.update("f_room", fDbBind, where_id(dbOO.value(i, "f_room").toInt()));
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_oldState"] = droom.value("f_state");
        fDbBind[":f_newState"] = state;
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comment"] = "EOD";
        fDb.insert("f_room_state_change", fDbBind);
    }

    /* remove expired out of order and out of inventory */
    fDbBind[":f_state1"] = RESERVE_OUTOFINVENTORY;
    fDbBind[":f_state2"] = RESERVE_OUTOFROOM;
    fDbBind[":f_endDate"] = WORKING_DATE;
    dbOO.select(fDb, "select f_room, f_state from f_reservation where f_state=:f_state1 or f_state=:f_state2 "
                " and f_endDate=:f_endDate", fDbBind);
    for (int i = 0; i < dbOO.rowCount(); i++) {
        DatabaseResult droom;
        fDbBind[":f_room"] = dbOO.value(i, "f_room");
        droom.select(fDb, "select f_state from f_room where f_id=:f_room", fDbBind);
        fDbBind[":f_state"] = ROOM_STATE_NONE;
        fDb.update("f_room", fDbBind, where_id(dbOO.value(i, "f_room").toInt()));
        fDbBind[":f_date"] = QDate::currentDate();
        fDbBind[":f_wdate"] = WORKING_DATE;
        fDbBind[":f_time"] = QTime::currentTime();
        fDbBind[":f_oldState"] = droom.value("f_state");
        fDbBind[":f_newState"] = ROOM_STATE_NONE;
        fDbBind[":f_user"] = WORKING_USERID;
        fDbBind[":f_comment"] = "EOD";
        fDb.insert("f_room_state_change", fDbBind);
    }

    if (result) {
        fDb.fDb.commit();
        fPreferences.setLocalDate(def_working_day, WORKING_DATE.addDays(1));
        BroadcastThread::cmdCommand(cmd_end_of_day, QMap<QString, QString>());
        accept();
    } else {
        fDb.fDb.rollback();
        message_error_tr("Error in query, automatic charging not complete");
    }

}

void DlgEndOfDay::on_btnRefresh_clicked()
{
    loadData();
}

void DlgEndOfDay::loadData()
{
    ui->tblCharges->clearContents();
    ui->tblCharges->setRowCount(0);
    ui->lbSemaphore->setPixmap(QPixmap(":/images/ball-green.png"));
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_invoice, r.f_vatMode, r.f_id, r.f_room, "
               "concat(g.f_title, ' ', g.f_firstName, ' ' , g.f_lastName), r.f_man + r.f_woman + r.f_child, "
               "r.f_startDate, r.f_endDate,"
               "r.f_pricePerNight, "
               " '', '', ra.f_en "
               "from f_reservation r  "
               "inner join f_guests g on g.f_id=r.f_guest "
               "inner join f_room_arrangement ra on ra.f_id=r.f_arrangement "
               "where r.f_state=:f_state", fDbBind, fDbRows);
    ui->tblCharges->setRowCount(fDbRows.count());
    Utils::fillTableWithData(ui->tblCharges, fDbRows);
    for (int i = 0, count = ui->tblCharges->rowCount(); i < count; i++) {
        ui->tblCharges->item(i, 9)->setData(Qt::EditRole, tr("Ready"));
    }
    int start = ui->tblCharges->rowCount();
    fDbBind[":f_date"] = WORKING_DATE;
    fDbBind[":f_state"] = RESERVE_RESERVE;
    fDb.select("select r.f_invoice, r.f_vatMode, r.f_id,  r.f_room, concat(g.f_title, ' ', g.f_firstName, ' ' , g.f_lastName), r.f_man + r.f_woman + r.f_child, "
               "r.f_startDate, r.f_endDate,"
               "r.f_pricePerNight, "
               " '', '', '' "
               "from f_reservation r "
               "inner join f_guests g on g.f_id=r.f_guest "
               "where r.f_startDate=:f_date and r.f_state=:f_state", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        fCanCharge = false;
        ui->lbSemaphore->setPixmap(QPixmap(":/images/ball-red.png"));
        ui->btnOk->setEnabled(false);
    }
    Utils::fillTableWithData(ui->tblCharges, fDbRows, true);
    for (int i = start, count = ui->tblCharges->rowCount(); i < count; i++) {
        ui->tblCharges->item(i, 9)->setData(Qt::EditRole, tr("Checkin"));
        EPushButton *b = new EPushButton();
        b->setIcon(QIcon(":/images/double-right.png"));
        b->fId = ui->tblCharges->item(i, 2)->data(Qt::EditRole).toString();
        connect(b, SIGNAL(clickedWithId(QString)), this, SLOT(openReservation(QString)));
        ui->tblCharges->setCellWidget(i, 10, b);
    }
    start = ui->tblCharges->rowCount();
    fDbBind[":f_date"] = WORKING_DATE;
    fDbBind[":f_state"] = RESERVE_CHECKIN;
    fDb.select("select r.f_invoice, r.f_vatMode, r.f_id, r.f_room, concat(g.f_title, ' ', g.f_firstName, ' ' , g.f_lastName), r.f_man + r.f_woman + r.f_child, "
               "r.f_startDate, r.f_endDate,"
               "r.f_pricePerNight, "
               " '', '', '' "
               "from f_reservation r "
               "inner join f_guests g on g.f_id=r.f_guest "
               "where r.f_endDate=:f_date and r.f_state=:f_state", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        fCanCharge = false;
        ui->lbSemaphore->setPixmap(QPixmap(":/images/ball-red.png"));
        ui->btnOk->setEnabled(false);
    }
    Utils::fillTableWithData(ui->tblCharges, fDbRows, true);
    for (int i = start, count = ui->tblCharges->rowCount(); i < count; i++) {
        ui->tblCharges->item(i, 9)->setData(Qt::EditRole, tr("Checkout"));
        EPushButton *b = new EPushButton();
        b->setIcon(QIcon(":/images/double-right.png"));
        b->fId = ui->tblCharges->item(i, 0)->data(Qt::EditRole).toString();
        connect(b, SIGNAL(clickedWithId(QString)), this, SLOT(openInvoice(QString)));
        ui->tblCharges->setCellWidget(i, 10, b);
    }
    if (WORKING_DATE.daysTo(QDate::currentDate()) <= 0) {
        ui->btnOk->setEnabled(false);
    }
}
