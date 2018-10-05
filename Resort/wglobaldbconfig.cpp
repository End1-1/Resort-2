#include "wglobaldbconfig.h"
#include "ui_wglobaldbconfig.h"
#include "message.h"
#include "defrest.h"
#include "dlgeditservtax.h"

void WGlobalDbConfig::getCompSettings()
{
    ui->lwHost->clear();
    fDb.select("select f_comp, f_hall from s_tax_print", fDbBind, fDbRows);
    foreach_rows {
        QListWidgetItem *item = new QListWidgetItem(ui->lwHost);
        item->setText(it->at(0).toString());
        item->setData(Qt::UserRole, it->at(1));
        ui->lwHost->addItem(item);
    }
    ui->leHost->clear();
    ui->leHallCode->clear();
}

void WGlobalDbConfig::getAccess()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    DatabaseResult drdb;
    drdb.select(db, "select * from f_db", fDbBind);
    ui->tblAccess->clearContents();
    ui->tblAccess->setRowCount(0);
    ui->tblAccess->setColumnCount(1 + drdb.rowCount());
    QStringList colCaption;
    colCaption << tr("Station/Username");
    QMap<QString, int> dbAddr;
    QMap<int, int> dbColAddr;
    for (int i = 0; i < drdb.rowCount(); i++) {
        colCaption << drdb.value(i, "f_name").toString();
        dbAddr[drdb.value(i, "f_id").toString()] = i + 1;
        dbColAddr[i + 1] = drdb.value(i, "f_id").toInt();
    }
    ui->tblAccess->setHorizontalHeaderLabels(colCaption);
    DatabaseResult draccess;
    draccess.select(db, "select * from f_users", fDbBind);
    ui->tblAccess->setRowCount(draccess.rowCount());
    QMap<QString, int> userAddr;
    for (int i = 0; i < draccess.rowCount(); i++) {
        userAddr[draccess.value(i, "f_name").toString()] = i;
        ui->tblAccess->setItemWithValue(i, 0, draccess.value(i, "f_name"));
        for (int j = 1; j < ui->tblAccess->columnCount(); j++) {
            ui->tblAccess->addCheckBox(i, j);
            ui->tblAccess->setItemWithValue(i, j, "", dbColAddr[j]);
        }
    }

    draccess.select(db, "select * from f_access", fDbBind);
    for (int i = 0; i < draccess.rowCount(); i++) {
        int row = userAddr[draccess.value(i, "f_user").toString()];
        int col = dbAddr[draccess.value(i, "f_db").toString()];
        ui->tblAccess->checkBox(row, col)->setChecked(draccess.value(i, "f_right").toString() == "1");
    }
    ui->tblAccess->horizontalHeader()->resizeContentsPrecision();
}

void WGlobalDbConfig::getDatabases()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    DatabaseResult dr;
    dr.select(db, "select * from f_db", fDbBind);
    Utils::tableSetColumnWidths(ui->tblDatabases, ui->tblDatabases->columnCount(),
                                0, 120, 120, 200, 120, 120);
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = ui->tblDatabases->addRow();
        ui->tblDatabases->addLineEdit(row, 0, false)->setText(dr.value(i, "f_id").toString());
        ui->tblDatabases->addLineEdit(row, 1, false)->setText(dr.value(i, "f_name").toString());
        ui->tblDatabases->addLineEdit(row, 2, false)->setText(dr.value(i, "f_host").toString());
        ui->tblDatabases->addLineEdit(row, 3, false)->setText(dr.value(i, "f_database").toString());
        ui->tblDatabases->addLineEdit(row, 4, false)->setText(dr.value(i, "f_username").toString());
        ui->tblDatabases->addLineEdit(row, 5, false)->setText(dr.value(i, "f_password").toString());
    }
}

void WGlobalDbConfig::getMonthly()
{
    Utils::tableSetColumnWidths(ui->tblMonthly, ui->tblMonthly->columnCount(),
                                50, 120, 100, 300);
    DatabaseResult dr;
    dr.select(fDb, "select * from serv_monthly order by f_id", fDbBind);
    ui->tblMonthly->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->tblMonthly->setItemWithValue(i, 0, dr.value(i, "f_id"));
        ui->tblMonthly->addLineEdit(i, 1, false)->setText(dr.value(i, "f_title").toString());
        ui->tblMonthly->addLineEdit(i, 2, false)->setText(dr.value(i, "f_width").toString());
        ui->tblMonthly->addLineEdit(i, 3, false)->setText(dr.value(i, "f_items").toString());
    }

    Utils::tableSetColumnWidths(ui->tblCardexAnalysis, ui->tblCardexAnalysis->columnCount(),
                                50, 120, 100, 300);
    dr.select(fDb, "select * from serv_cardex_analysis order by f_id", fDbBind);
    ui->tblCardexAnalysis->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->tblCardexAnalysis->setItemWithValue(i, 0, dr.value(i, "f_id"));
        ui->tblCardexAnalysis->addLineEdit(i, 1, false)->setText(dr.value(i, "f_title").toString());
        ui->tblCardexAnalysis->addLineEdit(i, 2, false)->setText(dr.value(i, "f_width").toString());
        ui->tblCardexAnalysis->addLineEdit(i, 3, false)->setText(dr.value(i, "f_items").toString());
    }
    ui->teDailyMovement->setPlainText(fPreferences.getDb(def_daily_movement_items).toString());
    ui->rbDailySubtotalDown->setChecked(fPreferences.getDb(def_daily_movement_total_side).toInt());
}

void WGlobalDbConfig::getTax()
{
    Utils::tableSetColumnWidths(ui->tblTax, ui->tblTax->columnCount(),
                                     0, 150, 40, 150, 150, 150);
    ui->tblTax->clearContents();
    DatabaseResult dr;
    dr.select(fDb, "select f_id, f_comp, f_active, f_host, f_port, f_password from serv_tax order by f_comp", fDbBind);
    ui->tblTax->setRowCount(dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        ui->tblTax->setItem(i, 0, new QTableWidgetItem(dr.value(i, "f_id").toString()));
        ui->tblTax->setItem(i, 1, new QTableWidgetItem(dr.value(i, "f_comp").toString()));
        ui->tblTax->setItem(i, 2, new QTableWidgetItem(dr.value(i, "f_active").toString()));
        ui->tblTax->setItem(i, 3, new QTableWidgetItem(dr.value(i, "f_host").toString()));
        ui->tblTax->setItem(i, 4, new QTableWidgetItem(dr.value(i, "f_port").toString()));
        ui->tblTax->setItem(i, 5, new QTableWidgetItem(dr.value(i, "f_password").toString()));
    }
}

WGlobalDbConfig::WGlobalDbConfig(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::WGlobalDbConfig)
{
    ui->setupUi(this);
    ui->deWorkingDate->setDate(WORKING_DATE);
    ui->teSlogan->setPlainText(fPreferences.getDb(def_slogan).toString());
    ui->leExtraBed->setText(fPreferences.getDb(def_default_extra_bed).toString());
    ui->leDefaultCardex->setText(fPreferences.getDb(def_default_cardex).toString());
    ui->leRoomChargeId->setText(fPreferences.getDb(def_room_charge_id).toString());
    ui->teEarlyCheckin->setTime(QTime::fromString(fPreferences.getDb(def_earyly_checkin).toString(), "HH:mm"));
    ui->teLateCheckout->setTime(QTime::fromString(fPreferences.getDb(def_late_checkout).toString(), "HH:mm"));
    ui->leEarlyCheckinID->setText(fPreferences.getDb(def_earyly_checkin_id).toString());
    ui->leLateCheckoutID->setText(fPreferences.getDb(def_late_checkout_id).toString());
    ui->leAirportPickupId->setText(fPreferences.getDb(def_airport_pickup_id).toString());
    ui->leDayUse->setText(fPreferences.getDb(def_day_use_id).toString());
    ui->leVaucherDigits->setText(fPreferences.getDb(def_vouchers_digits).toString());
    ui->teVoucherRightText->setPlainText(fPreferences.getDb(def_vouchers_right_header).toString());
    ui->teVoucherInvoiceFooter->setPlainText(fPreferences.getDb(def_vouchers_invoice_footer).toString());
    ui->leBrekfastChargeId->setText(fPreferences.getDb(def_auto_breakfast_id).toString());
    ui->leDiscountId->setText(fPreferences.getDb(def_invoice_default_discount_id).toString());
    ui->lePosTransferId->setText(fPreferences.getDb(def_invoice_default_positive_transfer_id).toString());
    ui->leNegTransferId->setText(fPreferences.getDb(def_invoice_default_negative_transfer_id).toString());
    ui->leRefundId->setText(fPreferences.getDb(def_invoice_default_refund_id).toString());
    ui->chNoTrackControl->setChecked(fPreferences.getDb(def_no_tracking_changes).toBool());
    ui->leManualTax->setText(fPreferences.getDb(def_filter_manual_tax).toString());
    ui->leRoomingList->setText(fPreferences.getDb(def_rooming_list).toString());
    ui->chRestMode->setChecked(fPreferences.getDb(def_welcome_rest_mode).toInt());
    ui->leCancelCode->setText(fPreferences.getDb(def_cancelfee_code).toString());
    ui->leNoshowCode->setText(fPreferences.getDb(def_noshowfee_code).toString());
    ui->leRestHallForReception->setText(fPreferences.getDb(def_rest_hall_for_reception).toString());
    ui->leRoomArrangement->setText(fPreferences.getDb(def_room_arrangement).toString());
    ui->leReceiptVaucherId->setText(fPreferences.getDb(def_receip_vaucher_id).toString());
    ui->leReservationVoucherId->setText(fPreferences.getDb(def_reservation_voucher_id).toString());
    ui->leRoomRateChangeId->setText(fPreferences.getDb(def_room_rate_change_id).toString());
    ui->leAdvanceVoucherId->setText(fPreferences.getDb(def_advance_voucher_id).toString());
    ui->leBreakfastHallId->setText(fPreferences.getDb(def_breakfast_default_hall).toString());
    ui->leBreakfastTableId->setText(fPreferences.getDb(def_breakfast_default_table).toString());
    ui->leBreakfastDishId->setText(fPreferences.getDb(def_breakfast_default_dish).toString());
    ui->leMinibarHallId->setText(fPreferences.getDb(def_minibar_default_hall).toString());
    ui->leMinibarTableId->setText(fPreferences.getDb(def_minibar_default_table).toString());
    ui->leMinibarDishId->setText(fPreferences.getDb(def_minibar_default_dish).toString());
    ui->chPasswordRequired->setChecked(fPreferences.getDb(def_passport_required).toInt());
    ui->chShowLogs->setChecked(fPreferences.getDb(def_show_logs).toBool());

    fTrackControl =  new TrackControl(TRACK_GLOBAL_CONFIG);
    fTrackControl->addWidget(ui->deWorkingDate, "Working date")
            .addWidget(ui->teSlogan, "Slogan")
            .addWidget(ui->leExtraBed, "Extra bed rate")
            .addWidget(ui->leDefaultCardex, "Default cardex code")
            .addWidget(ui->leRoomChargeId, "Room charge id")
            .addWidget(ui->leEarlyCheckinID, "Early checkin id")
            .addWidget(ui->leLateCheckoutID, "Late checkout id")
            .addWidget(ui->teEarlyCheckin, "Early checkin time")
            .addWidget(ui->teLateCheckout, "Late checkout time")
            .addWidget(ui->leAirportPickupId, "Airport pickup id")
            .addWidget(ui->leDayUse, "Day use id")
            .addWidget(ui->leVaucherDigits, "Voucher number format, digits count")
            .addWidget(ui->leBrekfastChargeId, "Breakfast charge id")
            .addWidget(ui->leDiscountId, "Default discount id")
            .addWidget(ui->lePosTransferId, "Positive balance default id")
            .addWidget(ui->leNegTransferId, "Negative balance default id")
            .addWidget(ui->chNoTrackControl, "No tracking changes")
            .addWidget(ui->leRefundId, "Refund default id")
            .addWidget(ui->leManualTax, "Manual tax filter")
            .addWidget(ui->leCancelCode, "Cancelation fee code")
            .addWidget(ui->leNoshowCode, "No show fee code")
            .addWidget(ui->leRestHallForReception, "Restaurant hall for reception")
            .addWidget(ui->leRoomArrangement, "Default room arrangement")
            .addWidget(ui->leReceiptVaucherId, "Receipt vaucher id")
            .addWidget(ui->leReservationVoucherId, "Reservation voucher id")
            .addWidget(ui->leAdvanceVoucherId, "Advance voucher id")
            .addWidget(ui->leRoomRateChangeId, "Room rate change voucher id")
            .addWidget(ui->leBreakfastHallId, "Breakfast hall id")
            .addWidget(ui->leBreakfastTableId, "Breakfast table id")
            .addWidget(ui->leBreakfastDishId, "Breakfast dish id")
            .addWidget(ui->leMinibarHallId, "Minibar hall id")
            .addWidget(ui->leMinibarTableId, "MInibar table id")
            .addWidget(ui->leMinibarDishId, "Minibar dish id")
            .addWidget(ui->chPasswordRequired, "Passport required")
            .addWidget(ui->chShowLogs, "Show logs")
            ;

    getCompSettings();
    getAccess();
    getDatabases();
    getMonthly();
    getTax();

    fDockHall = new DWSelectorHall(this);
    fDockHall->configure();
    fDockHall->setSelector(ui->leHallCode);
    connect(fDockHall, SIGNAL(hall(CI_RestHall*)), this, SLOT(hall(CI_RestHall*)));
}

WGlobalDbConfig::~WGlobalDbConfig()
{
    delete ui;
}

void WGlobalDbConfig::setupTab()
{
    setupTabTextAndIcon(tr("Global config"), ":/images/settings.png");
}

void WGlobalDbConfig::hall(CI_RestHall *c)
{
    dockResponse<CI_RestHall, CacheRestHall>(ui->leHallCode, ui->leHallName, c);
}

void WGlobalDbConfig::on_btnSave_clicked()
{
    QMap<QString, QString> values;
    values.insert(def_working_day, ui->deWorkingDate->date().toString(def_date_format));
    values.insert(def_slogan, ui->teSlogan->toPlainText());
    values.insert(def_default_extra_bed, ui->leExtraBed->text());
    values.insert(def_default_cardex, ui->leDefaultCardex->text());
    values.insert(def_room_charge_id, ui->leRoomChargeId->text());
    values.insert(def_earyly_checkin, ui->teEarlyCheckin->time().toString("HH:mm"));
    values.insert(def_late_checkout, ui->teLateCheckout->time().toString("HH:mm"));
    values.insert(def_earyly_checkin_id, ui->leEarlyCheckinID->text());
    values.insert(def_airport_pickup_id, ui->leAirportPickupId->text());
    values.insert(def_day_use_id, ui->leDayUse->text());
    values.insert(def_vouchers_digits, ui->leVaucherDigits->text());
    values.insert(def_vouchers_invoice_footer, ui->teVoucherInvoiceFooter->toPlainText());
    values.insert(def_vouchers_right_header, ui->teVoucherRightText->toPlainText());
    values.insert(def_late_checkout_id, ui->leLateCheckoutID->text());
    values.insert(def_auto_breakfast_id, ui->leBrekfastChargeId->text());
    values.insert(def_invoice_default_discount_id, ui->leDiscountId->text());
    values.insert(def_invoice_default_positive_transfer_id, ui->lePosTransferId->text());
    values.insert(def_invoice_default_negative_transfer_id, ui->leNegTransferId->text());
    values.insert(def_invoice_default_refund_id, ui->leRefundId->text());
    values.insert(def_no_tracking_changes, QString::number((int)ui->chNoTrackControl->isChecked()));
    values.insert(def_filter_manual_tax, ui->leManualTax->text());
    values.insert(def_cancelfee_code, ui->leCancelCode->text());
    values.insert(def_noshowfee_code, ui->leNoshowCode->text());
    values.insert(def_rooming_list, ui->leRoomingList->text());
    values.insert(def_welcome_rest_mode, QString("%1").arg((int)ui->chRestMode->isChecked()));
    values.insert(def_rest_hall_for_reception, ui->leRestHallForReception->text());
    values.insert(def_room_arrangement, ui->leRoomArrangement->text());
    values.insert(def_receip_vaucher_id, ui->leReceiptVaucherId->text());
    values.insert(def_reservation_voucher_id, ui->leReservationVoucherId->text());
    values.insert(def_room_rate_change_id, ui->leRoomRateChangeId->text());
    values.insert(def_advance_voucher_id, ui->leAdvanceVoucherId->text());
    values.insert(def_breakfast_default_hall, ui->leBreakfastHallId->text());
    values.insert(def_breakfast_default_table, ui->leBreakfastTableId->text());
    values.insert(def_breakfast_default_dish, ui->leBreakfastDishId->text());
    values.insert(def_minibar_default_hall, ui->leMinibarHallId->text());
    values.insert(def_minibar_default_table, ui->leMinibarTableId->text());
    values.insert(def_minibar_default_dish, ui->leMinibarDishId->text());
    values.insert(def_passport_required, QString::number((int)ui->chPasswordRequired->isChecked()));
    values.insert(def_show_logs, QString::number(ui->chShowLogs->isChecked()));
    QString query = "insert into f_global_settings (f_settings, f_key, f_value) values ";
    bool first = true;
    QMapIterator<QString, QString> it(values);
    while (it.hasNext()) {
        it.next();
        if (first) {
            first = false;
        } else {
            query += ",";
        }
        query += QString("(1, '%1', '%2')")
                .arg(it.key())
                .arg(it.value());
    }
    fDb.fDb.transaction();
    fDb.select("delete from f_global_settings where f_settings=1 and f_key not in ('HC', 'AHC')");
    bool result = fDb.queryDirect(query);
    fTrackControl->saveChanges();
    if (result) {
        fDb.fDb.commit();
        on_btnSaveReports_clicked();
        BroadcastThread::cmdCommand(cmd_global_settings, QMap<QString, QString>());
    } else {
        fDb.fDb.rollback();
        message_error(tr("New values not saved"));
    }
}

void WGlobalDbConfig::on_lwHost_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    ui->leHost->setText(index.data(Qt::DisplayRole).toString());
    ui->leHallCode->setText(index.data(Qt::UserRole).toString());
    CI_RestHall *ch = CacheRestHall::instance()->get(ui->leHallCode->text());
    if (ch) {
        ui->leHallName->setText(ch->fName);
    }

    DatabaseResult dr;
    fDbBind[":f_comp"] = index.data(Qt::DisplayRole).toString();
    dr.select(fDb, "select f_key, f_value from r_config where upper(f_comp)=upper(:f_comp)", fDbBind);
    QMap<QString, QString> v;
    for (int i = 0; i < dr.rowCount(); i++) {
        v[dr.value(i, "f_key").toString()] = dr.value(i, "f_value").toString();
    }
    ui->chOpenTableAfterRun->setChecked(v[dr_open_table_after_run].toInt());
    ui->leFirstReceiptPrinter->setText(v[dr_first_receipt_printer]);
    ui->leSecondReceiptPrinter->setText(v[dr_second_receipt_printer]);
    ui->leDisc20->setText(v[dr_discount_20]);
    ui->leDisc50->setText(v[dr_discount_50]);
}

void WGlobalDbConfig::on_btnSaveRestaurant_clicked()
{
    if (ui->leHost->text().isEmpty()) {
        message_error_tr("Host not selected");
        return;
    }
    fDbBind[":f_hall"] = ui->leHallCode->text();
    fDb.update("s_tax_print", fDbBind, where_field("f_comp", "'" + ui->leHost->text() + "'"));
    fDbBind[":f_comp"] = ui->leHost->text();
    fDb.select("delete from r_config where f_comp=:f_comp", fDbBind, fDbRows);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_open_table_after_run;
    fDbBind[":f_value"] = QString("%1").arg((int)ui->chOpenTableAfterRun->isChecked());
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_first_receipt_printer;
    fDbBind[":f_value"] = ui->leFirstReceiptPrinter->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_second_receipt_printer;
    fDbBind[":f_value"] = ui->leSecondReceiptPrinter->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_discount_20;
    fDbBind[":f_value"] = ui->leDisc20->text();
    fDb.insert("r_config", fDbBind);
    fDbBind[":f_comp"] = ui->leHost->text();
    fDbBind[":f_key"] = dr_discount_50;
    fDbBind[":f_value"] = ui->leDisc50->text();
    fDb.insert("r_config", fDbBind);

    getCompSettings();
    message_info_tr("Saved");
}

void WGlobalDbConfig::on_btnRefresh_clicked()
{
    getAccess();
}

void WGlobalDbConfig::on_btnSaveAccess_clicked()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    db.select("delete from f_access");
    for (int i = 0; i < ui->tblAccess->rowCount(); i++) {
        for (int j = 1; j < ui->tblAccess->columnCount(); j++) {
            fDbBind[":f_user"] = ui->tblAccess->toString(i, 0);
            fDbBind[":f_db"] = ui->tblAccess->item(i, j)->data(Qt::UserRole);
            fDbBind[":f_right"] = (ui->tblAccess->checkBox(i, j)->isChecked() ? 1 : 0);
            db.insert("f_access", fDbBind);
        }
    }
   // BroadcastThread::cmdCommand(cmd_global_settings, QMap<QString, QString>());
    message_info_tr("Saved");
}

void WGlobalDbConfig::on_btnSaveDatabases_clicked()
{
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    for (int i = 0; i < ui->tblDatabases->rowCount(); i++) {
        fDbBind[":f_name"] = ui->tblDatabases->lineEdit(i, 1)->text();
        fDbBind[":f_host"] = ui->tblDatabases->lineEdit(i, 2)->text();
        fDbBind[":f_database"] = ui->tblDatabases->lineEdit(i, 3)->text();
        fDbBind[":f_username"] = ui->tblDatabases->lineEdit(i, 4)->text();
        fDbBind[":f_password"] = ui->tblDatabases->lineEdit(i, 5)->text();
        if (ui->tblDatabases->lineEdit(i, 0)->asInt() == 0) {
            ui->tblDatabases->lineEdit(i, 0)->setInt(db.insert("f_db", fDbBind));
        } else {
            db.update("f_db", fDbBind, where_id(ap(ui->tblDatabases->lineEdit(i, 0)->text())));
        }
    }
    BroadcastThread::cmdCommand(cmd_global_settings, QMap<QString, QString>());
    message_info_tr("Saved");
}

void WGlobalDbConfig::on_btnSaveReports_clicked()
{
    for (int i = 0; i < ui->tblMonthly->rowCount(); i++) {
        fDbBind[":f_title"] = ui->tblMonthly->lineEdit(i, 1)->text();
        fDbBind[":f_width"] = ui->tblMonthly->lineEdit(i, 2)->text();
        fDbBind[":f_items"] = ui->tblMonthly->lineEdit(i, 3)->text();
        fDb.update("serv_monthly", fDbBind, where_id(ui->tblMonthly->toString(i, 0)));
    }
    for (int i = 0; i < ui->tblCardexAnalysis->rowCount(); i++) {
        fDbBind[":f_title"] = ui->tblCardexAnalysis->lineEdit(i, 1)->text();
        fDbBind[":f_width"] = ui->tblCardexAnalysis->lineEdit(i, 2)->text();
        fDbBind[":f_items"] = ui->tblCardexAnalysis->lineEdit(i, 3)->text();
        fDb.update("serv_cardex_analysis", fDbBind, where_id(ui->tblCardexAnalysis->toString(i, 0)));
    }
    fDbBind[":f_key"] = def_daily_movement_items;
    fDb.select("delete from f_global_settings where f_key=:f_key and f_settings=1", fDbBind, fDbRows);

    fDbBind[":f_key"] = def_daily_movement_total_side;
    fDb.select("delete from f_global_settings where f_key=:f_key and f_settings=1", fDbBind, fDbRows);

    fDbBind[":f_key"] = def_daily_movement_order;
    fDb.select("delete from f_global_settings where f_key=:f_key and f_settings=1", fDbBind, fDbRows);

    fDbBind[":f_settings"] = 1;
    fDbBind[":f_key"] = def_daily_movement_items;
    fDbBind[":f_value"] = ui->teDailyMovement->toPlainText();
    fDb.insert("f_global_settings", fDbBind);

    fDbBind[":f_settings"] = 1;
    fDbBind[":f_key"] = def_daily_movement_total_side;
    fDbBind[":f_value"] = (int) ui->rbDailySubtotalDown->isChecked();
    fDb.insert("f_global_settings", fDbBind);

    fPreferences.setDb(def_daily_movement_items, ui->teDailyMovement->toPlainText());
    fPreferences.setDb(def_daily_movement_total_side, (int) ui->rbDailySubtotalDown->isChecked());
    message_info(tr("Saved"));
}

void WGlobalDbConfig::on_btnRemoveStation_clicked()
{
    QModelIndexList sel = ui->tblAccess->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        message_error(tr("Nothing was selected"));
        return;
    }
    if (message_confirm(tr("Confirm to remove station")) != QDialog::Accepted) {
        return;
    }
    Database db;
    db.copyConnectionParamsFrom(fDb);
    db.fDb.setDatabaseName("airwick");
    if (!db.open()) {
        return;
    }
    fDbBind[":f_name"] = ui->tblAccess->toString(sel.at(0).row(), 0);
    db.select("delete from f_users where lower(f_name)=lower(:f_name)", fDbBind, fDbRows);
    fDbBind[":f_user"] = ui->tblAccess->toString(sel.at(0).row(), 0);
    db.select("delete from f_access where lower(f_user)=lower(:f_user)", fDbBind, fDbRows);
    db.close();
    getAccess();
}

void WGlobalDbConfig::on_btnRefreshReset_clicked()
{
    getCompSettings();
}

void WGlobalDbConfig::on_btnRemoveRest_clicked()
{
    QModelIndex index = ui->lwHost->currentIndex();
    if (!index.isValid()) {
        message_error_tr("Nothing was selected");
    }
    if (message_confirm_tr("Confirm to delete") != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_comp"] = index.data(Qt::DisplayRole).toString();
    fDb.select("delete from s_tax_print where f_comp=:f_comp", fDbBind, fDbRows);
    getCompSettings();
}

void WGlobalDbConfig::on_btnRefreshTax_clicked()
{
    getTax();
}

void WGlobalDbConfig::on_tblTax_cellDoubleClicked(int row, int column)
{
    if (row < 0 || column < 0) {
        return;
    }
    DlgEditServTax *d = new DlgEditServTax(this);
    d->setParams(ui->tblTax->toInt(row, 0),
                 ui->tblTax->toString(row, 1),
                 ui->tblTax->toInt(row, 2),
                 ui->tblTax->toString(row, 3),
                 ui->tblTax->toString(row, 4),
                 ui->tblTax->toString(row, 5));
    if (d->exec() == QDialog::Accepted) {
        getTax();
    }
    delete d;
}

void WGlobalDbConfig::on_btnSaveApplication_clicked()
{

}


void WGlobalDbConfig::on_chShowLogs_clicked(bool checked)
{
    fPreferences.setDb(def_show_logs, checked);
}
