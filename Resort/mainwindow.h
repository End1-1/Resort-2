#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "base.h"
#include "command.h"
#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>
#include <QLabel>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class BaseWidget;

class MainWindow : public QMainWindow, public Base
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void login();
    void logout();
    void addTabWidget(BaseWidget *widget);
    void refreshReservationList();
    void setCurrentWidget(QWidget *w);
    void configureLabels();
    QTabWidget *fTab;
    void hideMenu();
public slots:
    void on_actionExpected_arrivals_triggered();
    void on_actionReservatios_triggered();
    void on_actionIn_house_guest_triggered();
    void on_actionHistory_of_calls_triggered();
    void on_actionCity_Ledger_detailed_balance_triggered();
    void on_actionInvoices_triggered();
    void on_actionDaily_movement_triggered();
    void on_actionMonthly_Report_triggered();
    void on_actionCash_repoort_detailed_triggered();
    void on_actionCash_report_total_triggered();
    void on_actionCardex_analysis_triggered();
    void on_actionVauchers_triggered();
    void on_actionExport_invoices_triggered();
    void on_actionExport_active_reservations_triggered();
    void on_actionSynchronization_triggered();
    void on_actionRestaurant_triggered();
    void on_actionHotel_status_triggered();
    void on_actionRemarks_triggered();
    void on_actionReports_set_triggered();
    void on_actionStatistics_triggered();
    void on_actionHakk_triggered();
    void on_actionTables_triggered();
    void on_actionNames_of_menu_triggered();
    void on_actionMain_partitions_of_menu_triggered();
    void on_actionType_of_dishes_triggered();
    void on_actionDishes_triggered();
    void on_actionModifiers_triggered();
    void on_actionStorages_triggered();
    void on_actionMenu_review_triggered();
    void on_actionPrinters_triggered();
    void on_actionTrack_changes_triggered();
    void on_actionRoomChart_triggered();
    void on_actionNew_reservation_triggered();

    void on_actionCanceled_reservations_triggered();

    void on_actionWeb_triggered();

    void on_actionReservations_by_date_created_triggered();

    void on_actionNew_store_document_triggered();

    void on_actionPartners_2_triggered();

    void on_actionDocuments_list_triggered();

    void on_actionMaterials_in_the_store_triggered();

    void on_actionStore_movement_triggered();

    void on_actionCoupons_triggered();

    void on_actionCalculate_output_of_restaurant_triggered();
    void on_actionLogout_triggered();
    void on_actionLock_triggered();
    void on_actionDatabases_triggered();
    void on_actionRoom_list_triggered();
    void on_actionAbout_triggered();
    void on_actionGuest_file_triggered();
    void on_actionContacts_triggered();
    void on_actionUsers_triggered();
    void on_actionUsers_groups_triggered();
    void on_actionRoom_view_triggered();
    void on_actionPartners_triggered();

    void on_actionCredit_card_triggered();

    void on_actionCity_Ledger_balance_triggered();

    void on_actionChange_password_triggered();

    void on_actionNew_advance_entry_triggered();

    void on_actionGlobal_config_triggered();

    void on_actionComplex_dish_triggered();

    void on_actionCategories_triggered();

    void on_actionType_of_bed_triggered();

    void on_actionCategory_to_sell_triggered();

    void on_actionCity_Ledger_triggered();

    void on_actionCurrencies_triggered();

    void on_actionForecast_Occupancy_Category_triggered();

    void on_actionSetup_Tax_Printer_triggered();

    void on_actionInvoice_items_triggered();

    void on_actionPosting_charge_triggered();

    void on_actionDiscount_triggered();

    void on_actionRe_checkin_triggered();

    void on_actionTransfer_amount_triggered();

    void on_actionExpected_arrivals_simple_triggered();

    void on_actionUpdate_program_triggered();

    void on_actionReceipt_vaucher_triggered();

    void on_actionNotes_triggered();

    void on_actionEvent_triggered();

    void on_actionDaily_financial_report_triggered();

    void on_actionBanquet_comment_triggered();

    void on_actionCity_ledger_balance_2_triggered();

    void on_actionComplimentary_comment_triggered();

    void on_actionReport_buillder_triggered();

    void on_actionList_of_source_reservation_triggered();

    void on_actionYearly_financial_report_triggered();

    void on_actionUtils_triggered();

    void on_actionExport_data_triggered();

    void on_actionAdvance_report_triggered();

    void on_actionRestaurant_online_triggered();
    void on_actionPartners_group_triggered();

    void on_actionGuest_titles_triggered();

    void on_actionNationality_file_triggered();

    void on_actionCheckout_invoices_free_rooming_triggered();

    void on_actionNew_group_reservation_triggered();

    void on_actionChanges_of_states_of_room_triggered();

    void on_actionExport_back_triggered();

    void on_actionOpen_breakfast_triggered();

    void on_actionMonthly_occupancy_percentages_triggered();

    void on_actionNaitonality_report_by_period_triggered();

    void on_actionCancelation_No_show_fees_triggered();

    void on_actionCancelation_No_show_fee_triggered();

    void on_actionSales_by_storages_triggered();
    void on_actionReservation_groups_triggered();

    void on_actionConfigure_Welcome_Page_triggered();

    void on_actionGuest_by_nationality_triggered();

    void on_actionModels_of_cars_triggered();
    void on_actionCostumers_cars_triggered();
    void on_actionReport_by_payment_triggered();
    void on_actionPartners_debts_triggered();
    void on_actionCoupons_sales_triggered();
    void on_actionCoupons_seria_triggered();
    void on_actionAccounts_triggered();
    void on_actionAccounts_2_triggered();
    void on_actionStore_entries_triggered();
    void on_actionNew_store_checkpoint_triggered();
    void on_actionCoupons_statistics_triggered();
    void on_actionExit_triggered();
    void on_actionLogin_triggered();
    void on_actionPartner_payments_triggered();
    void on_actionDebts_triggered();
    void on_actionDiscount_report_triggered();
    void on_actionSales_report_by_cars_triggered();
    void on_actionDiscount_total_triggered();
    void on_actionCostumers_triggered();

protected:
    void closeEvent(QCloseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void shortcutFullScreen();
    void shortcutSlot();
    void customReport();
    void tabCloseRequested(int index);
    void timeout();
    void socketReadyRead();
    void parseSocketCommand(const QString &command);
    void socketError(QAbstractSocket::SocketError f_cityLedger);
    void socketDisconnected();
    void on_actionBreakfast_report_triggered();

    void on_actionRefund_voucher_triggered();

    void on_actionRoom_inventory_triggered();

    void on_actionTransfer_CL_amount_triggered();

    void on_actionAvailable_amounts_triggered();

    void on_actionGuest_Tray_Ledger_triggered();

    void on_actionIn_house_detailed_balance_triggered();

    void on_actionAvaiable_rooms_triggered();

    void on_actionRoom_inventory_2_triggered();

    void on_actionRoom_inventory_states_triggered();

    void on_actionList_of_checkin_guests_triggered();

    void on_actionLenght_of_stay_triggered();

    void on_actionExport_reservations_triggered();

private:
    Ui::MainWindow *ui;
    QTimer fTimer;
    Command fCommand;
    QTcpSocket fSocket;
    QUdpSocket fUdpSocket;
    QTcpSocket fSocketDraft;
    QString actionTitle(QObject *a);
    QLabel *fStatusLabelLeft;
    QLabel *fStatusLabelRight;
    void lock();
    void enableMainMenu(bool value);
    void disableMainMenu();
    QMap<QAction*, int> fCustomReports;
};

#endif // MAINWINDOW_H
