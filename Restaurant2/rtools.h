#ifndef RTOOLS_H
#define RTOOLS_H

#include "baseextendeddialog.h"

class RDesk;

namespace Ui {
class RTools;
}

class RTools : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RTools(QWidget *parent = 0);
    void setNoTable();
    ~RTools();
private slots:
    void on_btnExit_clicked();
    void on_btnShowHideRemovedItems_clicked();
    void on_btnOrderComment_clicked();

    void on_btnMoveTable_clicked();

    void on_btnRemoveOrder_clicked();

    void on_btnShowTableOrders_clicked();

    void on_btnShowNyTotal_clicked();

    void on_btnInitCash_clicked();

    void on_btnComplex_clicked();

    void on_btnTotalReport_clicked();

    void on_btnTotalYesterday_clicked();

    void on_btnCompimentary_clicked();

    void on_btnTax_clicked();

    void on_btnChangePassword_clicked();

    void on_btnRecover_clicked();

    void on_btnTotalReportAnyDay_clicked();

    void on_btnPrintAnyReport_clicked();

    void on_btnVoidBack_clicked();

    void on_btnPrintVoid_clicked();

    void on_btnCloseDay_clicked();

    void on_btnSalary_clicked();

    void on_btnTotalShort_clicked();

    void on_btnCardStat_clicked();

    void on_btnVisitStat_clicked();

    void on_btnCheckCard_clicked();

    void on_btnPrintDailySales_clicked();

private:
    Ui::RTools *ui;
    RDesk *fDesk;
};

#endif // RTOOLS_H
