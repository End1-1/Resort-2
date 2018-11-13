#ifndef WWELCOMEREST_H
#define WWELCOMEREST_H

#include "basewidget.h"

namespace Ui {
class wwelcomerest;
}

class wwelcomerest : public BaseWidget
{
    Q_OBJECT

public:
    explicit wwelcomerest(QWidget *parent = 0);
    ~wwelcomerest();
    virtual void setup();
private slots:
    void on_bntnMaterialsGroups_clicked();

    void on_btnMaterials_clicked();

    void on_btnHall_clicked();

    void on_btnTables_clicked();

    void on_btnStorages_clicked();

    void on_bntnMaterialsGroups_2_clicked();

    void on_btnDocuments_clicked();

    void on_btnDocuments_2_clicked();

    void on_btnStore_clicked();

    void on_btnMovementInstore_clicked();

    void on_btnCaclOutput_clicked();

    void on_btn_clicked();

    void on_btnNewStoreDoc_clicked();

    void on_btnMoveInStore_clicked();

    void on_btnOnLineReport_clicked();

    void on_btnRestCommon_clicked();

    void on_btnUsersGroups_clicked();

    void on_btnUsers_clicked();

    void on_btnMenuReview_clicked();

    void on_btnCoupons_clicked();

    void on_btnReportByCash_clicked();

    void on_btnHallOptions_clicked();

    void on_btnExit_clicked();

    void on_btnCarsModel_clicked();

    void on_btnCustomers_clicked();

    void on_btnPackages_clicked();

    void on_btnReportByPayment_clicked();

    void on_btnPartnerDebt_clicked();

    void on_btnSaleCoupons_clicked();

    void on_btnPartners_clicked();

    void on_btnCouponSeria_clicked();

    void on_btnCash_clicked();

    void on_btnCashNames_clicked();

    void on_btnStoreEntries_clicked();

    void on_btnNewStoreCheckPoint_clicked();

    void on_btnCashOps_clicked();

    void on_btnDiscountCards_clicked();

    void on_btnDebts_clicked();

    void on_btnDiscountReport_clicked();

    void on_btnReportByCar_clicked();

    void on_btnDiscountTotal_clicked();

    void on_btnCostumers_clicked();

    void on_btnCustomers_2_clicked();

    void on_btnCustomers_3_clicked();

    void on_btnTStore_clicked();

    void on_btnCardBalances_clicked();

private:
    Ui::wwelcomerest *ui;
};

#endif // WWELCOMEREST_H
