#include "wwelcomerest.h"
#include "ui_wwelcomerest.h"
#include "mainwindow.h"
#include "recarclient.h"
#include "ftstorereport.h"
#include "wreportgrid.h"
#include "fbalanceoncard.h"
#include "wsalary.h"

wwelcomerest::wwelcomerest(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::wwelcomerest)
{
    ui->setupUi(this);
    fMainWindow->hideMenu();
    fMainWindow->showMaximized();
}

wwelcomerest::~wwelcomerest()
{
    delete ui;
}

void wwelcomerest::setup()
{
    setupTabTextAndIcon(tr("Home"), ":/images/home.png");
}

void wwelcomerest::on_bntnMaterialsGroups_clicked()
{
    fMainWindow->on_actionType_of_dishes_triggered();
}

void wwelcomerest::on_btnMaterials_clicked()
{
    fMainWindow->on_actionDishes_triggered();
}

void wwelcomerest::on_btnHall_clicked()
{
    fMainWindow->on_actionHakk_triggered();
}

void wwelcomerest::on_btnTables_clicked()
{
    fMainWindow->on_actionTables_triggered();
}

void wwelcomerest::on_btnStorages_clicked()
{
    fMainWindow->on_actionStorages_triggered();
}

void wwelcomerest::on_bntnMaterialsGroups_2_clicked()
{
    fMainWindow->on_actionNew_store_document_triggered();
}

void wwelcomerest::on_btnDocuments_clicked()
{
    fMainWindow->on_actionDocuments_list_triggered();
}

void wwelcomerest::on_btnDocuments_2_clicked()
{
    fMainWindow->on_actionStore_movement_triggered();
}

void wwelcomerest::on_btnMovementInstore_clicked()
{
    fMainWindow->on_actionStore_movement_triggered();
}

void wwelcomerest::on_btnCaclOutput_clicked()
{
    fMainWindow->on_actionCalculate_output_of_restaurant_triggered();
}

void wwelcomerest::on_btnStore_clicked()
{
    fMainWindow->on_actionMaterials_in_the_store_triggered();
}

void wwelcomerest::on_btn_clicked()
{

}

void wwelcomerest::on_btnNewStoreDoc_clicked()
{
    fMainWindow->on_actionNew_store_document_triggered();
}

void wwelcomerest::on_btnMoveInStore_clicked()
{
    fMainWindow->on_actionStore_movement_triggered();
}

void wwelcomerest::on_btnOnLineReport_clicked()
{
    fMainWindow->on_actionRestaurant_online_triggered();
}

void wwelcomerest::on_btnRestCommon_clicked()
{
    fMainWindow->on_actionRestaurant_triggered();
}

void wwelcomerest::on_btnUsersGroups_clicked()
{
    fMainWindow->on_actionUsers_groups_triggered();
}


void wwelcomerest::on_btnUsers_clicked()
{
    fMainWindow->on_actionUsers_triggered();
}

void wwelcomerest::on_btnMenuReview_clicked()
{
    fMainWindow->on_actionMenu_review_triggered();
}

void wwelcomerest::on_btnCoupons_clicked()
{
    fMainWindow->on_actionCoupons_triggered();
}

void wwelcomerest::on_btnReportByCash_clicked()
{
    fMainWindow->on_actionSales_by_storages_triggered();
}

void wwelcomerest::on_btnHallOptions_clicked()
{
    fMainWindow->on_actionGlobal_config_triggered();
}

void wwelcomerest::on_btnExit_clicked()
{
    fMainWindow->close();
}

void wwelcomerest::on_btnCarsModel_clicked()
{
    fMainWindow->on_actionModels_of_cars_triggered();
}

void wwelcomerest::on_btnCustomers_clicked()
{
    fMainWindow->on_actionCostumers_cars_triggered();
}

void wwelcomerest::on_btnPackages_clicked()
{
    fMainWindow->on_actionComplex_dish_triggered();
}

void wwelcomerest::on_btnReportByPayment_clicked()
{
    fMainWindow->on_actionReport_by_payment_triggered();
}

void wwelcomerest::on_btnPartnerDebt_clicked()
{
    fMainWindow->on_actionPartners_debts_triggered();
}

void wwelcomerest::on_btnSaleCoupons_clicked()
{
    fMainWindow->on_actionCoupons_sales_triggered();
}

void wwelcomerest::on_btnPartners_clicked()
{
    fMainWindow->on_actionPartners_2_triggered();
}

void wwelcomerest::on_btnCouponSeria_clicked()
{
    fMainWindow->on_actionCoupons_seria_triggered();
}

void wwelcomerest::on_btnCash_clicked()
{
    fMainWindow->on_actionAccounts_triggered();
}

void wwelcomerest::on_btnCashNames_clicked()
{
    fMainWindow->on_actionAccounts_2_triggered();
}

void wwelcomerest::on_btnStoreEntries_clicked()
{
    fMainWindow->on_actionStore_entries_triggered();
}

void wwelcomerest::on_btnNewStoreCheckPoint_clicked()
{
    fMainWindow->on_actionNew_store_checkpoint_triggered();
}

void wwelcomerest::on_btnCashOps_clicked()
{
    fMainWindow->on_actionInvoice_items_triggered();
}

void wwelcomerest::on_btnDiscountCards_clicked()
{
    fMainWindow->on_actionCoupons_statistics_triggered();
}

void wwelcomerest::on_btnDebts_clicked()
{
    fMainWindow->on_actionDebts_triggered();
}

void wwelcomerest::on_btnDiscountReport_clicked()
{
    fMainWindow->on_actionDiscount_report_triggered();
}

void wwelcomerest::on_btnReportByCar_clicked()
{
    fMainWindow->on_actionSales_report_by_cars_triggered();
}

void wwelcomerest::on_btnDiscountTotal_clicked()
{
    fMainWindow->on_actionDiscount_total_triggered();
}

void wwelcomerest::on_btnCostumers_clicked()
{
    fMainWindow->on_actionCostumers_triggered();
}

void wwelcomerest::on_btnCustomers_2_clicked()
{
    RECarClient::openReport2();
}

void wwelcomerest::on_btnCustomers_3_clicked()
{
    RECarClient::openReport3();
}

void wwelcomerest::on_btnTStore_clicked()
{
    FTStoreReport::openFilterReport<FTStoreReport, WReportGrid>();
}

void wwelcomerest::on_btnCardBalances_clicked()
{
    FBalanceOnCard::openFilterReport<FBalanceOnCard, WReportGrid>();
}

void wwelcomerest::on_btnSalary_clicked()
{
    WSalary::openFilterReport<WSalary, WReportGrid>();
}
