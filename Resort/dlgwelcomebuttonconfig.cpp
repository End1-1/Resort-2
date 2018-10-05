#include "dlgwelcomebuttonconfig.h"
#include "ui_dlgwelcomebuttonconfig.h"
#include "cacherights.h"

DlgWelcomeButtonConfig::DlgWelcomeButtonConfig(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgWelcomeButtonConfig)
{
    ui->setupUi(this);
    QObjectList ol = ui->w1->children();
    foreach (QObject *o, ol) {
        EToolbarButton *t = dynamic_cast<EToolbarButton*>(o);
        if (t) {
            t->setChecked(fPreferences.getUser("welbtn_" + t->text()).toInt());
        }
    }
    ol = ui->w2->children();
    foreach (QObject *o, ol) {
        EToolbarButton *t = dynamic_cast<EToolbarButton*>(o);
        if (t) {
            t->setChecked(fPreferences.getUser("welbtn_" + t->text()).toInt());
        }
    }
    ol = ui->w3->children();
    foreach (QObject *o, ol) {
        EToolbarButton *t = dynamic_cast<EToolbarButton*>(o);
        if (t) {
            t->setChecked(fPreferences.getUser("welbtn_" + t->text()).toInt());
        }
    }
    ui->w1->setVisible(false);
    adjustSize();

    ui->btnRoomChart->setVisible(r__(cr__room_chart));
    ui->btnNewReservation->setVisible(r__(cr__edit_reservation));
    ui->btnNewGroupResevation->setVisible(r__(cr__edit_reservation));
    ui->btnReservations->setVisible(r__(cr__reservations));
    ui->btnInhouseGuest->setVisible(r__(cr__report_guest));
    ui->btnArrivalDeparture->setVisible(r__(cr__report_guest));
    ui->btnCallHistory->setVisible(r__(cr__calls_history));
    ui->btnNotes->setVisible(r__(cr__notes));
    ui->btnContacts->setVisible(r__(cr__contacts));
    ui->btnAdvance->setVisible(r__(cr__advance_vaucher));
    ui->btnPostCharge->setVisible(r__(cr__postchage_vaucher));
    ui->btnReceipt->setVisible(r__(cr__receipt_vaucher));
    ui->btnTransfer->setVisible(r__(cr__transfer_vaucher));
    ui->btnDiscount->setVisible(r__(cr__discount_vaucher));
    ui->btnCityLedgerDetailedBalance->setVisible(r__(cr__cityledger_balance));
    ui->btnCheckoutInvoices->setVisible(r__(cr__report_checkout_invoices));
    ui->btnDailyMovement->setVisible(r__(cr__report_daily_movement));
    ui->btnMonthlyMovement->setVisible(r__(cr__report_monthly));
    ui->btnCashReportDetailed->setVisible(r__(cr__report_cash));
    ui->btnCashReportSummary->setVisible(r__(cr__report_cash));
    ui->btnCardexAnalysis->setVisible(r__(cr__cardex_analysis));
    ui->btnVauchers->setVisible(r__(cr__report_vauchers));
    ui->btnExportInvoices->setVisible(r__(cr__bookkeeper_sync));
    ui->btnExportActiveReservation->setVisible(r__(cr__export_active_reservations));
    ui->btnExportRestaurant->setVisible(r__(cr__export_event_etc));
    ui->btnCallHistory->setVisible(r__(cr__call_in) || r__(cr__call_out) || r__(cr__call_int) || r__(cr__call_tin) || r__(cr__call_tout));
    ui->btnRestaurant->setVisible(r__(cr__report_restaurant));
    ui->btnRemarks->setVisible(r__(cr__remarks));
    ui->btnHotelStatus->setVisible(r__(cr__hotel_status));
    ui->btnReportSet->setVisible(r__(cr__reports_set));
    ui->btnOnlineInvoice->setVisible(r__(cr__edit_reservation));
    ui->btnStatistics->setVisible(r__(cr__reports_set));

    ui->btnExportActiveReservation->setVisible(fPreferences.getDb("HC").toBool() && RIGHT(WORKING_USERGROUP, cr__export_event_etc) && ui->btnExportActiveReservation->isVisible());
    ui->btnExportRestaurant->setVisible(fPreferences.getDb("HC").toBool() && RIGHT(WORKING_USERGROUP, cr__export_event_etc) && ui->btnExportRestaurant->isVisible());
    ui->btnExportInvoices->setVisible(fPreferences.getDb("HC").toBool() && RIGHT(WORKING_USERGROUP, cr__export_event_etc) && ui->btnExportInvoices->isVisible());
}

DlgWelcomeButtonConfig::~DlgWelcomeButtonConfig()
{
    delete ui;
}

void DlgWelcomeButtonConfig::on_btnCancel_clicked()
{
    reject();
}

void DlgWelcomeButtonConfig::on_btnOk_clicked()
{
    ui->btnExportActiveReservation->setChecked(fPreferences.getDb("HC").toBool() && RIGHT(WORKING_USERGROUP, cr__export_event_etc) && ui->btnExportActiveReservation->isChecked());
    ui->btnExportInvoices->setChecked(fPreferences.getDb("HC").toBool() && RIGHT(WORKING_USERGROUP, cr__export_event_etc) && ui->btnExportInvoices->isChecked());
    ui->btnExportRestaurant->setChecked(fPreferences.getDb("HC").toBool() && RIGHT(WORKING_USERGROUP, cr__export_event_etc) && ui->btnExportRestaurant->isChecked());
    QObjectList ol = ui->w1->children();

    foreach (QObject *o, ol) {
        EToolbarButton *t = dynamic_cast<EToolbarButton*>(o);
        if (t) {
            fPreferences.setUser(fDbName, "welbtn_" + t->text(), t->isChecked() ? "1" : "0");
        }
    }
    ol = ui->w2->children();
    foreach (QObject *o, ol) {
        EToolbarButton *t = dynamic_cast<EToolbarButton*>(o);
        if (t) {
            fPreferences.setUser(fDbName, "welbtn_" + t->text(), t->isChecked() ? "1" : "0");
        }
    }
    ol = ui->w3->children();
    foreach (QObject *o, ol) {
        EToolbarButton *t = dynamic_cast<EToolbarButton*>(o);
        if (t) {
            fPreferences.setUser(fDbName, "welbtn_" + t->text(), t->isChecked() ? "1" : "0");
        }
    }
    accept();
    message_info_tr("Settings will apply in next time logon");
}
