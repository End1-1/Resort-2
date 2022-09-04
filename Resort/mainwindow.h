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
    void setCurrentWidget(QWidget *w);
    QTabWidget *fTab;
    void expandTab();

public slots:
    void on_actionCash_repoort_detailed_triggered();
    void on_actionCash_report_total_triggered();
    void on_actionRestaurant_triggered();
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
    void on_actionNew_store_document_triggered();
    void on_actionPartners_2_triggered();
    void on_actionDocuments_list_triggered();
    void on_actionMaterials_in_the_store_triggered();
    void on_actionStore_movement_triggered();
    void on_actionCoupons_triggered();
    void on_actionLogout_triggered();
    void on_actionLock_triggered();
    void on_actionDatabases_triggered();
    void on_actionAbout_triggered();
    void on_actionContacts_triggered();
    void on_actionUsers_triggered();
    void on_actionUsers_groups_triggered();
    void on_actionCredit_card_triggered();
    void on_actionChange_password_triggered();
    void on_actionGlobal_config_triggered();
    void on_actionComplex_dish_triggered();
    void on_actionEvent_triggered();
    void on_actionComplimentary_comment_triggered();
    void on_actionReport_buillder_triggered();
    void on_actionRestaurant_online_triggered();
    void on_actionOpen_breakfast_triggered();
    void on_actionSales_by_storages_triggered();
    void on_actionConfigure_Welcome_Page_triggered();
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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void shortcutFullScreen();
    void shortcutSlot();
    void customReport();
    void tabCloseRequested(int index);
    void socketReadyRead();
    void parseSocketCommand(const QString &command);
    void socketError(QAbstractSocket::SocketError f_cityLedger);
    void socketDisconnected();
    void on_actionBreakfast_report_triggered();
    void on_actionT_Report_triggered();
    void on_tbEarning_clicked();
    void on_tbStore_clicked();
    void on_tbStore_2_clicked();
    void on_tbStore_3_clicked();
    void on_tbStore_4_clicked();
    void on_actionCostumers_triggered();
    void on_actionCoupons_one_off_triggered();
    void on_actionCoupons_present_triggered();
    void on_actionBalance_of_the_cards_triggered();
    void on_tbExit_clicked();
    void on_actionEarnings_carwash_triggered();
    void on_actionEarnings_waitroom_triggered();
    void on_actionCarwash_index_triggered();
    void on_actionWaiting_room_index_triggered();
    void on_actionStorage_goods_index_triggered();
    void on_actionEarnings_shop_triggered();
    void on_actionShop_index_triggered();
    void on_actionSalary_by_employes_triggered();

private:
    Ui::MainWindow *ui;
    QTimer fTimer;
    Command fCommand;
    QTcpSocket fSocket;
    QUdpSocket fUdpSocket;
    QTcpSocket fSocketDraft;
    QString actionTitle(QObject *a);
    void lock();
    void enableMainMenu(bool value);
    void disableMainMenu();
    void buildMenu(QToolButton *btn, const QList<QAction*> &l);
    QMap<QAction*, int> fCustomReports;
    void dish(int t);
};

#endif // MAINWINDOW_H
