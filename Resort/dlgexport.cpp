#include "dlgexport.h"
#include "ui_dlgexport.h"
#include "vauchers.h"
#include "fvauchers.h"
#include <QTextBlock>

DlgExport::DlgExport(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgExport)
{
    ui->setupUi(this);
    QStringList dbParams = fPreferences.getDb("AHC").toString().split(";", QString::SkipEmptyParts);
    if (dbParams.count() < 4) {
        message_error_tr("Setup second database parameters");
        return;
    }
    ui->leHost->setText(dbParams[0]);
    ui->leDatabase->setText(dbParams[1]);
    ui->leUsername->setText(dbParams[2]);
    ui->lePassword->setText(dbParams[3]);
}

DlgExport::~DlgExport()
{
    delete ui;
}

void DlgExport::start()
{
    DlgExport *d = new DlgExport(fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}

void DlgExport::on_btnStart_clicked()
{
    if (message_confirm(tr("Confirm to start")) != QDialog::Accepted) {
        return;
    }

    QStringList ids;

    Database sDb;
    sDb.setConnectionParams(ui->leHost->text(), ui->leDatabase->text(), ui->leUsername->text(), ui->lePassword->text());
    if (!sDb.open()) {
        ui->ptLog->appendPlainText("Connection error, " + sDb.fLastError);
        return;
    }
    sDb.fDb.transaction();
    ui->ptLog->appendPlainText("Started...");

    bool err = false;
    QList<QList<QVariant> > o;
    QMap<QString, QVariant> b;
    QMap<QString, QVariant> sb;

    // WORKING DATE
    ui->ptLog->appendPlainText("Working date");
    b[":f_key"] = def_working_day;
    err = sDb.select("delete from f_global_settings where f_key=:f_key", b, o) == -1;

    if (err) {
        goto ERR;
    }

    b[":f_key"] = def_working_day;
    b[":f_settings"] = 1;
    b[":f_value"] = WORKING_DATE.toString("dd/MM/yyyy");
    err = sDb.insert("f_global_settings", b) == -1;

    if (err) {
        goto ERR;
    }
    ui->chWorkingDate->setChecked(true);
    // END OF WORKING DATE

    //CHECKIN RESERVATION
    {
        ui->ptLog->appendPlainText("Checkin and reservations");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult dr;
        b[":f_state1"] = RESERVE_CHECKIN;
        b[":f_state2"] = RESERVE_RESERVE;
        dr.select(fDb, "select * from f_reservation where f_state=:f_state1 or f_state=:f_state2", b);
        for (int i = 0; i < dr.rowCount(); i++) {
            dr.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            ids << sb[":f_id"].toString();
            sDb.select("delete from f_reservation where f_id=:f_id", b, o);
            err = sDb.insertWithoutId("f_reservation", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr.rowCount()));
        }
        ui->chCheckinReserve->setChecked(true);
        // END CHECKIN RESERVATION

        ui->ptLog->appendPlainText("Reservation groups");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        int resCount = 0;
        for (int i = 0; i < dr.rowCount(); i++) {
            if (dr.value(i, "f_group").toInt() > 0) {
                resCount++;
                DatabaseResult dt;
                b[":f_id"] = dr.value(i, "f_group").toInt();
                err = dt.select(fDb, "select * from f_reservation_group where f_id=:f_id", b) == false;
                if (err) {
                    goto ERR;
                }
                if (dt.rowCount() == 0) {
                    ui->ptLog->appendPlainText("Missing group with id " + dr.value(i, "f_group").toString());
                    ui->ptLog->appendPlainText("Continue...");
                    continue;
                }
                b[":f_id"] = dr.value(i, "f_group").toInt();
                err = sDb.select("delete from f_reservation_group where f_id=:f_id", b, o) == -1;
                if (err) {
                    goto ERR;
                }
                dt.getBindValues(0, sb);
                err = sDb.insertWithoutId("f_reservation_group", sb) == false;
                if (err) {
                    goto ERR;
                }
            }
            replacePtLastLine(QString("%1 of %2 entries, found %3").arg(i + 1).arg(dr.rowCount()).arg(resCount));
        }
        ui->ptLog->appendPlainText("Reservation groups extra names");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        {
            DatabaseResult drge;
            if (!drge.select(fDb, "select * from f_reservation_group where length(f_extra1)>0", b)) {
                goto ERR;
            }
            for (int i = 0; i < drge.rowCount(); i++) {
                drge.getBindValues(i, sb);
                b[":f_name"] = sb[":f_extra1"];
                sDb.update("f_reservation_group", b, where_id(sb[":f_id"].toInt()));
                replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(drge.rowCount()));
            }
            sb.clear();
        }
        ui->chReserveGroups->setChecked(true);

        //VOUCHERS FOR CHECKIN AND RESERVATIONS
        ui->ptLog->appendPlainText("Vouchers for checkin reservation");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult dr2;
        b[":f_state1"] = RESERVE_CHECKIN;
        b[":f_state2"] = RESERVE_RESERVE;
        dr2.select(fDb, "select * from m_register where f_inv in (select f_invoice from f_reservation where f_state=:f_state1 or f_state=:f_state2)", b);
        for (int i = 0; i < dr2.rowCount(); i++) {
            dr2.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            ids << sb[":f_id"].toString();
            sDb.select("delete from m_register where f_id=:f_id", b, o);
            err = sDb.insertWithoutId("m_register", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr2.rowCount()));
        }
        ui->chVouchers->setChecked(true);
        //END VOUCHERS FOR CHECKIN AND RESERVATIONS

        //RESTAURANT VOUCHERS OF ONLINE RESERVATIONS
        ui->ptLog->appendPlainText("Restaurant vouchers for checkin reservation");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        int restCount = 0;
        for (int i = 0; i < dr2.rowCount(); i++) {
            if (dr2.value(i, "f_source").toString() == VAUCHER_POINT_SALE_N) {
                restCount++;
                b[":f_id"] = dr2.value(i, "f_id");
                DatabaseResult dt1;
                dt1.select(fDb, "select * from o_header where f_id=:f_id", b);
                if (dt1.rowCount() == 0) {
                    ui->ptLog->appendPlainText(QString("Missing o_header for %1").arg(dr2.value(i, "f_id").toString()));
                    ui->ptLog->appendPlainText("Continue...");
                    continue;
                }
                dt1.getBindValues(0, sb);
                b[":f_id"] = dr2.value(i, "f_id");
                sDb.select("delete from o_header where f_id=:f_id", b, o);
                err = sDb.insertWithoutId("o_header", sb) == false;
                if (err) {
                    goto ERR;
                }
                b[":f_header"] = dr2.value(i, "f_id");
                DatabaseResult dt2;
                dt2.select(fDb, "select * from o_dish where f_header=:f_header", b);
                for (int j = 0; j < dt2.rowCount(); j++) {
                    dt2.getBindValues(j, sb);
                    b[":f_id"] = sb[":f_id"];
                    ids << sb[":f_id"].toString();
                    sDb.select("delete from o_dish where f_id=:f_id", b, o);
                    err = sDb.insertWithoutId("o_dish", sb) == false;
                    if (err) {
                        goto ERR;
                    }
                }
            }
            replacePtLastLine(QString("%1 of %2 entries checked, %3 found").arg(i + 1).arg(dr2.rowCount()).arg(restCount));
        }
        ui->chRoomRestaurant->setChecked(true);
        //END RESTAURANT VOUCHERS

        //GUESTS
        ui->ptLog->appendPlainText("Guests");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult dr3;
        b[":f_state1"] = RESERVE_CHECKIN;
        b[":f_state2"] = RESERVE_RESERVE;
        dr3.select(fDb, "select * from f_guests where f_id in (select f_guest from f_reservation where f_state=:f_state1 or f_state=:f_state2) "
                   "or f_id in (select f_guest from f_reservation_guests where f_reservation in (select f_id from f_reservation where f_state=:f_state1 or f_state=:f_state2))", b);
        if (dr3.rowCount() == 0) {
            goto ERR;
        }
        for (int i = 0; i < dr3.rowCount(); i++) {
            dr3.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            sDb.select("delete from f_guests where f_id=:f_id", b, o);
            err = sDb.insertWithoutId("f_guests", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr3.rowCount()));
        }
        ui->chGuests->setChecked(true);
        //END GUESTS

        //F_RESERVATION_GUESTS
        ui->ptLog->appendPlainText("Reservation guests");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult dr4;
        b[":f_state1"] = RESERVE_CHECKIN;
        b[":f_state2"] = RESERVE_RESERVE;
        dr4.select(fDb, "select * from f_reservation_guests where f_reservation in (select f_id from f_reservation where f_state=:f_state1 or f_state=:f_state2)", b);
        if (dr4.rowCount() == 0) {
            goto ERR;
        }
        ui->ptLog->appendPlainText("Deleting old Reservation guests");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        for (int i = 0; i < dr4.rowCount(); i++) {
            dr4.getBindValues(i, sb);
            b[":f_reservation"] = sb[":f_reservation"];
            err = sDb.select("delete from f_reservation_guests where f_reservation=:f_reservation", b, o) == -1;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr4.rowCount()));
        }
        ui->ptLog->appendPlainText("Inserting new Reservation guests");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        for (int i = 0; i < dr4.rowCount(); i++) {
            dr4.getBindValues(i, sb);
            sb.remove(":f_id");
            err = sDb.insertWithoutId("f_reservation_guests", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr4.rowCount()));
        }
        ui->ptLog->appendPlainText("Inserting new Reservation guests");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        ui->chReservationGuest->setChecked(true);

        ui->ptLog->appendPlainText("Room states");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult dr8;
        dr8.select(fDb, "select * from f_room", b);
        for (int i = 0; i < dr8.rowCount(); i++) {
            dr8.getBindValues(i, sb);
            b[":f_state"] = sb[":f_state"];
            b[":f_id"] = sb[":f_id"];
            err = sDb.select("update f_room set f_state=:f_state where f_id=:f_id", b, o) == -1;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr8.rowCount()));
        }
        sb.clear();
        ui->chRoomstates->setChecked(true);
        //END F_RESERVATION_GUESTS
    }
    if (!processTable(sDb, "r_hall", ui->chHall)) {
        return;
    }
    if (!processTable(sDb, "r_table", ui->chTable)) {
        return;
    }
    if (!processTable(sDb, "users_groups", ui->chUsersGroups)) {
        return;
    }
    if (!processTable(sDb, "users", ui->chUsers)) {
        return;
    }
    {
        //ONLINE RESTAURANT
        ui->ptLog->appendPlainText("Online restaurant");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult dr5;
        ui->ptLog->appendPlainText("Clear current table states");
        qApp->processEvents();
        sDb.select("update r_table set f_order=''", b, o);
        b[":f_state1"] = ORDER_STATE_OPENED;
        b[":f_state2"] = ORDER_STATE_CLOSED;
        b[":f_dateCash"] = WORKING_DATE;
        err = dr5.select(fDb, "select * from o_header where f_state=:f_state1 or (f_state=:f_state2 and f_dateCash=:f_dateCash and f_tax>0)", b) == false;
        if (err) {
            goto ERR;
        }
        ui->ptLog->appendPlainText("Proceed o_header");
        ui->ptLog->appendPlainText("Go..");
        qApp->processEvents();
        for (int i = 0; i < dr5.rowCount(); i++) {
            dr5.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            ids << sb[":f_id"].toString();
            err = sDb.select("delete from o_header where f_id=:f_id", b, o) == -1;
            if (err) {
                goto ERR;
            }
            err = sDb.insertWithoutId("o_header", sb) == false;
            if (err) {
                goto ERR;
            }
            if (dr5.value(i, "f_state").toInt() == ORDER_STATE_OPENED) {
                b[":f_order"] = dr5.value(i, "f_id");
                b[":f_id"] = dr5.value(i, "f_table");
                err = sDb.select("update r_table set f_order=:f_order where f_id=:f_id", b, o) == -1;
                if (err) {
                    goto ERR;
                }
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr5.rowCount()));
        }
        ui->ptLog->appendPlainText("Proceed o_dish");
        ui->ptLog->appendPlainText("Go..");
        qApp->processEvents();
        DatabaseResult dr6;
        b[":f_state1"] = ORDER_STATE_OPENED;
        b[":f_state2"] = ORDER_STATE_CLOSED;
        b[":f_dateCash"] = WORKING_DATE;
        err = dr6.select(fDb, "select * from o_dish where f_header in (select f_id from o_header where f_state=:f_state1 or (f_state=:f_state2 and f_dateCash=:f_dateCash and f_tax>0))", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr6.rowCount(); i++) {
            dr6.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            ids << sb[":f_id"].toString();
            err = sDb.select("delete from o_dish where f_id=:f_id", b, o) == -1;
            if (err) {
                goto ERR;
            }
            err = sDb.insertWithoutId("o_dish", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr6.rowCount()));
        }
        ui->ptLog->appendPlainText("Proceed m_register for closed orders");
        ui->ptLog->appendPlainText("Go..");
        int restCount = 0;
        for (int i = 0; i < dr5.rowCount(); i++) {
            if (dr5.value(i, "f_state").toInt() == ORDER_STATE_CLOSED) {
                restCount++;
                DatabaseResult dt1;
                b[":f_id"] = dr5.value(i, "f_id");
                err = dt1.select(fDb, "select * from m_register where f_id=:f_id", b) == false;
                if (dt1.rowCount() == 0) {
                    ui->ptLog->appendPlainText(QString("No vaucher for header %1").arg(dr5.value(i, "f_id").toString()));
                    ui->ptLog->appendPlainText("Continue...");
                    continue;
                }
                dt1.getBindValues(0, sb);
                b[":f_id"] = sb[":f_id"];
                err = sDb.select("delete from m_register where f_id=:f_id", b, o) == -1;
                if (err) {
                    goto ERR;
                }
                err = sDb.insertWithoutId("m_register", sb) == false;
                if (err) {
                    goto ERR;
                }
            }
            replacePtLastLine(QString("%1 of %2 entries, found %3").arg(i + 1).arg(dr5.rowCount()).arg(restCount));
        }
        ui->ptLog->appendPlainText("Proceed events");
        ui->ptLog->appendPlainText("Go..");
        qApp->processEvents();
        DatabaseResult dr7;
        b[":f_wdate"] = WORKING_DATE;
        if(!dr7.select("select * from o_event where f_state=0 or (f_state=1 and f_tax>0 and f_wdate=:f_wdate)", b)) {
            goto ERR;
        }
        for (int i = 0; i < dr7.rowCount(); i++) {
            dr7.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            ids << sb[":f_id"].toString();
            err = sDb.select("delete from o_event where f_id=:f_id", b, o) == -1;
            if (err) {
                goto ERR;
            }
            err = sDb.insertWithoutId("o_event", sb) == false;
            if (err) {
                goto ERR;
            }
            DatabaseResult dt;
            b[":f_id"] = dr7.value(i, "f_id");
            err = dt.select(fDb, "select * from m_register where f_id=:f_id", b) == false;
            if (err) {
                goto ERR;
            }
            if (dt.rowCount() == 0) {
                ui->ptLog->appendPlainText(QString("No voucher for %1").arg(dr7.value(i, "f_id").toString()));
                ui->ptLog->appendPlainText("Continue..");
                continue;
            }
            dt.getBindValues(0, sb);
            b[":f_id"] = sb[":f_id"];
            err = sDb.select("delete from m_register where f_id=:f_id", b, o) == -1;
            if (err) {
                goto ERR;
            }
            err = sDb.insertWithoutId("m_register", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr5.rowCount()));
        }
        ui->chOnlineRestaurant->setChecked(true);
        //END ONLINE RESTAURANT

    }

    // MENU
    {
        DatabaseResult dr9;
        ui->ptLog->appendPlainText("Menu names");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from r_menu_names", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr9.select(fDb, "select * from r_menu_names", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr9.rowCount(); i++) {
            dr9.getBindValues(i, sb);
            err = sDb.insertWithoutId("r_menu_names", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr9.rowCount()));
        }
        ui->ptLog->appendPlainText("Dish part");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from r_dish_part", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr9.select(fDb, "select * from r_dish_part", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr9.rowCount(); i++) {
            dr9.getBindValues(i, sb);
            err = sDb.insertWithoutId("r_dish_part", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr9.rowCount()));
        }
        ui->ptLog->appendPlainText("Dish types");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from r_dish_type", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr9.select(fDb, "select * from r_dish_type", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr9.rowCount(); i++) {
            dr9.getBindValues(i, sb);
            err = sDb.insertWithoutId("r_dish_type", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr9.rowCount()));
        }
        ui->ptLog->appendPlainText("Dishes");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from r_dish", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr9.select(fDb, "select * from r_dish", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr9.rowCount(); i++) {
            dr9.getBindValues(i, sb);
            err = sDb.insertWithoutId("r_dish", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr9.rowCount()));
        }
        ui->ptLog->appendPlainText("And menu");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from r_menu", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr9.select(fDb, "select * from r_menu", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr9.rowCount(); i++) {
            dr9.getBindValues(i, sb);
            err = sDb.insertWithoutId("r_menu", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr9.rowCount()));
        }
        if (!processTable(sDb, "r_dish_complex", 0)) {
            return;
        }
        if (!processTable(sDb, "r_dish_complex_list", 0)) {
            return;
        }
        if (!processTable(sDb, "r_dish_mod", 0)) {
            return;
        }
        if (!processTable(sDb, "r_dish_mod_required", 0)) {
            return;
        }
        ui->chMenu->setChecked(true);
    }
    {
        DatabaseResult dr10;
        ui->ptLog->appendPlainText("Proceed cardex");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from f_cardex", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr10.select(fDb, "select * from f_cardex", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr10.rowCount(); i++) {
            dr10.getBindValues(i, sb);
            err = sDb.insertWithoutId("f_cardex", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr10.rowCount()));
        }
        err = dr10.select(fDb, "select * from f_cardex where length(f_extra1) > 0", b) == false;
        if (err) {
            goto ERR;
        }

        ui->ptLog->appendPlainText("Proceed cardex extra names");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        for (int i = 0; i < dr10.rowCount(); i++) {
            dr10.getBindValues(i, sb);
            b[":f_name"] = sb[":f_extra1"];
            b[":f_addr1"] = sb[":f_fextra2"];
            b[":f_addr2"] = "";
            b[":f_tel"] = "";
            b[":f_fax"] = "";
            b[":f_contact"] = "";
            b[":f_contactPos"] = "";
            b[":f_email"] = "";
            err = sDb.update("f_cardex", b, where_id(sb[":f_id"].toInt())) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr10.rowCount()));
        }
        sb.clear();
        ui->chCardex->setChecked(true);

        ui->ptLog->appendPlainText("Proceed city ledger");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from f_city_ledger", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr10.select(fDb, "select * from f_city_ledger", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr10.rowCount(); i++) {
            dr10.getBindValues(i, sb);
            err = sDb.insertWithoutId("f_city_ledger", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr10.rowCount()));
        }

        ui->ptLog->appendPlainText("Proceed city ledger extra names");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        dr10.select(fDb, "select * from f_city_ledger where length(f_extra1)>0", b);
        for (int i = 0; i < dr10.rowCount(); i++) {
            dr10.getBindValues(i, sb);
            b[":f_name"] = sb[":f_extra1"];
            b[":f_address"] = sb[":f_extra2"];
            b[":f_phone"] = "";
            b[":f_email"] = "";
            err = sDb.update("f_city_ledger", b, where_id(sb[":f_id"].toInt())) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr10.rowCount()));
        }
        sb.clear();
        ui->chCL->setChecked(true);

        ui->ptLog->appendPlainText("Proceed cardex counters");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        err = sDb.select("delete from f_cardex_group", b, o) == -1;
        if (err) {
            goto ERR;
        }
        err = dr10.select(fDb, "select * from f_cardex_group", b) == false;
        if (err) {
            goto ERR;
        }
        for (int i = 0; i < dr10.rowCount(); i++) {
            dr10.getBindValues(i, sb);
            err = sDb.insertWithoutId("f_cardex_group", sb) == false;
            if (err) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr10.rowCount()));
        }
        ui->chCardexCounter->setChecked(true);
    }
    if (!processTable(sDb, "f_nationality", ui->chNationality)) {
        return;
    }

    {
        ui->ptLog->appendPlainText("Proceed settings");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult drr;
        drr.select(fDb, "select * from f_global_settings", b);
        if (sDb.select("delete from f_global_settings", b, o) == -1) {
            goto ERR;
        }
        for (int i = 0; i < drr.rowCount(); i++) {
            drr.getBindValues(i, sb);
            if (sb[":f_key"].toString() == "AHC" || sb[":f_key"].toString() == "HC") {
                continue;
            }
            sb.remove(":f_id");
            if (!sDb.insert("f_global_settings", sb)) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(drr.rowCount()));
        }
        ui->chSettings->setChecked(true);
    }

    {
        ui->ptLog->appendPlainText("Proceed out of order and drafts");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        DatabaseResult drr;
        b[":f_state1"] = RESERVE_OUTOFINVENTORY;
        b[":f_state2"] = RESERVE_OUTOFROOM;
        b[":f_state3"] = RESERVE_SERVICE;
        b[":f_endDate"] = WORKING_DATE;
        if (!drr.select(fDb, "select * from f_reservation where (f_state=:f_state1 or f_state=:f_state2 or f_state=:f_state3) and f_endDate>=:f_endDate", b)) {
            goto ERR;
        }
        for (int i = 0; i < drr.rowCount(); i++) {
            drr.getBindValues(i, sb);
            b[":f_id"] = sb[":f_id"];
            ids << sb[":f_id"].toString();
            if (sDb.select("delete from f_reservation where f_id=:f_id", b, o) == -1) {
                goto ERR;
            }
            if (!sDb.insertWithoutId("f_reservation", sb)) {
                goto ERR;
            }
            replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(drr.rowCount()));
        }
        ui->chOutOfOrder->setChecked(true);
    }

    FVauchers::correctCL(sDb);

    {
        ui->ptLog->appendPlainText("Proceed final");
        ui->ptLog->appendPlainText("Go...");
        qApp->processEvents();
        int scount = 1;
        foreach (QString s, ids) {
            b[":f_id"]= s;
            fDb.insertWithoutId("id_back", b);
            replacePtLastLine(QString("%1 of %2 entries").arg(scount++).arg(ids.count()));
        }
        fAirDb.open();
        fAirDb.select("update f_id set f_id=f_id*-1, f_comp=null,f_date=null, f_time=null where f_id>0", b, o);
        fAirDb.close();
        ui->chFinal->setChecked(true);
    }

    ERR:
    if (err) {
        sDb.fDb.rollback();
        ui->ptLog->appendPlainText(sDb.fLastError);
        ui->ptLog->appendPlainText("Cannot continue, check for errors");
        return;
    }

    sDb.fDb.commit();
    ui->ptLog->appendPlainText("Done.");

}

void DlgExport::replacePtLastLine(const QString &str)
{
    QTextBlock tb = ui->ptLog->document()->findBlockByLineNumber(ui->ptLog->document()->lineCount() - 1);
    if (tb.isValid()) {
        QTextCursor cursor(tb);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    }
    ui->ptLog->appendPlainText(str);
    qApp->processEvents();
}

bool DlgExport::processTable(Database &db, const QString &table, QCheckBox *ch)
{
    ui->ptLog->appendPlainText(QString("Proceed %1").arg(table));
    ui->ptLog->appendPlainText("Go...");
    qApp->processEvents();
    DatabaseResult dr;
    QMap<QString, QVariant> b, sb;
    QList<QList<QVariant> > o;
    bool err = db.select(QString("delete from %1").arg(table), b, o) == -1;
    if (err) {
        goto ERR;
    }
    err = dr.select(fDb, QString("select * from %1").arg(table), b) == false;
    if (err) {
        goto ERR;
    }
    for (int i = 0; i < dr.rowCount(); i++) {
        dr.getBindValues(i, sb);
        err = db.insertWithoutId(table, sb) == false;
        if (err) {
            goto ERR;
        }
        replacePtLastLine(QString("%1 of %2 entries").arg(i + 1).arg(dr.rowCount()));
    }
    if (ch) {
        ch->setChecked(true);
    }

    ERR:
    if (err) {
        db.fDb.rollback();
        ui->ptLog->appendPlainText(db.fLastError);
        ui->ptLog->appendPlainText("Cannot continue, check for errors");
        return false;
    }

    return true;
}

