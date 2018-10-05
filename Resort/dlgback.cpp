#include "dlgback.h"
#include "ui_dlgback.h"
#include "message.h"
#include <QTextBlock>

DlgBack::DlgBack(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgBack)
{
    ui->setupUi(this);
}

DlgBack::~DlgBack()
{
    delete ui;
}

void DlgBack::on_btnStart_clicked()
{
    QFile errFile("./err.log");
    errFile.open(QIODevice::WriteOnly);

    ui->btnStart->setEnabled(false);
    log("Started at" + QDateTime::currentDateTime().toString(def_date_time_format));
    Database sDb;
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        ui->btnStart->setEnabled(true);
        return;
    }
    sDb.setConnectionParams(dbParams[0], dbParams[1], dbParams[2], dbParams[3]);
    if (!sDb.open()) {
        message_error_tr("Cannot connect to second database");
        ui->btnStart->setEnabled(true);
        return;
    }
    DatabaseResult dr;
    int err = 0;

    /*
    log("Call log");
    err = 0;
    dr.select(sDb, "select * from f_call_log", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        fDbBind.remove(":f_id");
        if (!fDb.insertWithoutId("f_call_log", fDbBind)) {
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
            err++;
        }
        log(QString("Insert into call log. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }


    log("Tracking");
    dr.select(sDb, "select * from f_changes_tracking", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        fDbBind.remove(":f_id");
        fDb.insertWithoutId("f_changes_tracking", fDbBind);
        log(QString("Insert into tracking. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }


    dr.select(sDb, "select * from serv_id", fDbBind);
    log("Insert into serv_id");
    err = 0;
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("serv_id", fDbBind)) {
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
            err++;
        }
        log(QString("Insert into serv_id. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }
    */

    DatabaseResult dOldOnline;
    dOldOnline.select(fDb, "select f_id from o_header where f_state=1", fDbBind);
    log("Delete old online restaurant");
    for (int i = 0; i < dOldOnline.rowCount(); i++) {
        fDbBind[":f_id"] = dOldOnline.value(i, "f_id");
        fDb.select("delete from o_header where f_id=:f_id", fDbBind, fDbRows);
        fDbBind[":f_header"] = dOldOnline.value(i, "f_id");
        fDb.select("delete from o_dish where f_header=:f_header", fDbBind, fDbRows);
        log(QString("Delete online order. %1 of %2").arg(i + 1).arg(dOldOnline.rowCount()), false);
    }

    log("Get all orders after 2017-12-18 00:00:00");
    err = 0;
    dr.select(sDb, "select * from o_header where f_dateOpen>'2017-12-18 00:00:00'", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("o_header", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Insert into o_header. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    log("Get all dishes after 2017-12-18 00:00:00");
    err = 0;
    dr.select(sDb, "select * from o_dish where f_header in (select f_id from o_header where f_dateOpen>'2017-12-18 00:00:00')", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("o_dish", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Insert into o_dish. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    log("Get back old online orders with changes");
    err = 0;
    for (int i = 0; i < dOldOnline.rowCount(); i++) {
        fDbBind[":f_id"] = dOldOnline.value(i, "f_id");
        dr.select(sDb, "select * from o_header where f_id=:f_id", fDbBind);
        dr.getBindValues(0, fDbBind);
        if (!fDb.insertWithoutId("o_header", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        fDbBind[":f_header"] = dOldOnline.value(i, "f_id");
        dr.select(sDb, "select * from o_dish where f_header=:f_header", fDbBind);
        for (int j = 0; j < dr.rowCount(); j++) {
            dr.getBindValues(j, fDbBind);
            if (!fDb.insertWithoutId("o_dish", fDbBind)) {
                err++;
                errFile.write(fDb.fLastError.toUtf8());
                errFile.write("\r\n");
            }
        }
        log(QString("Insert old changed online. %1 of %2. Errors: %3").arg(i + 1).arg(dOldOnline.rowCount()).arg(err), false);
    }

    log("Update current r_table information");
    err = 0;
    dr.select(sDb, "select f_id, f_order from r_table", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_order"] = dr.value(i, "f_order");
        if (!fDb.update("r_table", fDbBind, where_id(dr.value(i, "f_id").toInt()))) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Update r_table. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    // ---------------------------- RESERVATION -----------------------------

    log("Remove old checkin and reserved information");
    DatabaseResult dReserv;
    dReserv.select(fDb, "select * from f_reservation where f_state in (1, 2)", fDbBind);
    err = 0;
    for (int i = 0; i < dReserv.rowCount(); i++) {
        fDbBind[":f_id"] = dReserv.value(i, "f_id");
        fDb.select("delete from f_reservation where f_id=:f_id", fDbBind, fDbRows);
        log(QString("Remove old reservations. %1 of %2. Errors: %3").arg(i + 1).arg(dReserv.rowCount()).arg(err), false);
    }

    log("Remove old vachers of reservations");
    err = 0;
    for (int i = 0; i < dReserv.rowCount(); i++) {
        fDbBind[":f_inv"] = dReserv.value(i, "f_invoice");
        fDb.select("delete from m_register where f_inv=:f_inv", fDbBind, fDbRows);
        log(QString("Remove old vouchers. %1 of %2. Errors: %3").arg(i + 1).arg(dReserv.rowCount()).arg(err), false);
    }

    log("Remove old guest of reservations");
    err = 0;
    for (int i = 0; i < dReserv.rowCount(); i++) {
        fDbBind[":f_reservation"] = dReserv.value(i, "f_id");
        fDb.select("delete from f_reservation_guests where f_reservation=:f_reservation", fDbBind, fDbRows);
        log(QString("Remove old guest of reservations. %1 of %2. Errors: %3").arg(i + 1).arg(dReserv.rowCount()).arg(err), false);
    }

    log("Geg other state reservations");
    err = 0;
    dr.select(sDb, "select f_id, f_state from f_reservation where f_state not in (1, 2, 3)", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_state"] = dr.value(i, "f_state");
        fDb.update("f_reservation", fDbBind, where_id(ap(dr.value(i, "f_id").toString())));
        log(QString("Update other state reservations. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    log("Get current checkin, reserved and checkout (>=2017-12-18) reservations  ");
    err = 0;
    DatabaseResult drNewReserve;
    drNewReserve.select(sDb, "select * from f_reservation where f_state in (1, 2) or (f_state=3 and f_endDate>='2017-12-18')", fDbBind);
    for (int i = 0; i < drNewReserve.rowCount(); i++) {
        drNewReserve.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("f_reservation", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Insert Reservation. %1 of %2. Errors: %3").arg(i + 1).arg(drNewReserve.rowCount()).arg(err), false);
    }

    log("Reservation guests");
    err = 0;
    for (int i = 0; i < drNewReserve.rowCount(); i++) {
        fDbBind[":f_reservation"] = drNewReserve.value(i, "f_id");
        dr.select(sDb, "select f_guest from m_register where f_reservation=:f_reservation", fDbBind);
        for (int j = 0; j < dr.rowCount(); j++) {
            dr.getBindValues(j, fDbBind);
            fDbBind[":f_reservation"] = drNewReserve.value(i, "f_id");
            if (!fDb.insertWithoutId("f_reservation_guests", fDbBind)) {
                err++;
                errFile.write(fDb.fLastError.toUtf8());
                errFile.write("\r\n");
            }
        }
        log(QString("Insert reservation guest. %1 of %2. Errors: %3").arg(i + 1).arg(drNewReserve.rowCount()).arg(err), false);
    }

    log("Get reservation vouchers");
    err = 0;
    for (int i = 0; i < drNewReserve.rowCount(); i++) {
        fDbBind[":f_inv"] = drNewReserve.value(i, "f_invoice");
        dr.select(sDb, "select * from m_register where f_inv=:f_inv", fDbBind);
        for (int j = 0; j < dr.rowCount(); j++) {
            dr.getBindValues(j, fDbBind);
            if (!fDb.insertWithoutId("m_register", fDbBind)) {
                err++;
                errFile.write(fDb.fLastError.toUtf8());
                errFile.write("\r\n");
            }
        }
        log(QString("Insert reservation vauchers. %1 of %2. Errors: %3").arg(i + 1).arg(drNewReserve.rowCount()).arg(err), false);
    }

    log("Get single vauchers");
    err = 0;
    DatabaseResult dsVauchers;
    dsVauchers.select(sDb, "select * from m_register where (f_inv is null or f_inv='' or f_inv='0') and f_rdate>='2017-12-18' ", fDbBind);
    for (int i = 0; i < dsVauchers.rowCount(); i++) {
        dsVauchers.getBindValues(i, fDbBind);
        fDb.insertWithoutId("m_register", fDbBind);
        if (dsVauchers.value(i, "f_source").toString() == "PE") {
            fDbBind[":f_id"] = dsVauchers.value(i, "f_id");
            dr.select(sDb, "select * from o_event where f_id=:f_id", fDbBind);
            dr.getBindValues(0, fDbBind);
            fDb.insertWithoutId("o_event", fDbBind);
        } else if (dsVauchers.value(i, "f_source").toString() == "CH") {
            if (dsVauchers.value(i, "f_itemCode").toInt() == fPreferences.getDb(def_auto_breakfast_id).toInt()) {
                fDbBind[":f_id"] = dsVauchers.value(i, "f_id");
                dr.select(sDb, "select * from o_breakfast where f_id=:f_id", fDbBind);
                dr.getBindValues(0, fDbBind);
                fDb.insertWithoutId("o_breakfast", fDbBind);
            }
        }
        log(QString("Insert single vauchers. %1 of %2. Errors: %3").arg(i + 1).arg(dsVauchers.rowCount()).arg(err), false);
    }

    log("Update room states");
    err = 0;
    dr.select(sDb, "select f_id, f_state from f_room", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_state"] = dr.value(i, "f_state");
        fDb.update("f_room", fDbBind, where_id(dr.value(i, "f_id").toInt()));
        log(QString("Update room states. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    log("Insert new guests");
    err = 0;
    dr.select(fDb, "select max(f_id) as f_id from f_guests", fDbBind);
    fDbBind[":f_id"] = dr.value("f_id");
    dr.select(sDb, "select * from f_guests where f_id>:f_id", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("f_guests", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Insert guests. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }
    log("Insert new cardex");
    err = 0;
    dr.select(sDb, "select * from f_cardex", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("f_cardex", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Insert cardex. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    log("Insert new city ledger");
    err = 0;
    dr.select(sDb, "select * from f_city_ledger", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, fDbBind);
        if (!fDb.insertWithoutId("f_city_ledger", fDbBind)) {
            err++;
            errFile.write(fDb.fLastError.toUtf8());
            errFile.write("\r\n");
        }
        log(QString("Insert city ledger. %1 of %2. Errors: %3").arg(i + 1).arg(dr.rowCount()).arg(err), false);
    }

    log("End at " + QDateTime::currentDateTime().toString(def_date_time_format));
    log("Dont forget about working date!");
    log("Check the tax paramters!");
    log("HEY HO!");
    errFile.close();
}

void DlgBack::log(const QString &text, bool append)
{
    if (!append) {
        QTextBlock tb = ui->teLog->document()->findBlockByLineNumber(ui->teLog->document()->lineCount() - 1);
        if (tb.isValid()) {
            QTextCursor cursor(tb);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
        }
    }
    ui->teLog->appendPlainText(text);
    qApp->processEvents();
}
