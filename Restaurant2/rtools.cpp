#include "rtools.h"
#include "ui_rtools.h"
#include "rdesk.h"
#include "dlgselecttaxcashmode.h"

RTools::RTools(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::RTools)
{
    ui->setupUi(this);
    fDesk = static_cast<RDesk*>(parent);
    //ui->btnComplex->setEnabled(!fDesk->fDishComplex && fDesk->countTotal() < 0.1);
}

void RTools::setNoTable()
{
    ui->btnCompimentary->setEnabled(false);
    ui->btnMoveTable->setEnabled(false);
    ui->btnRemoveOrder->setEnabled(false);
    ui->btnOrderComment->setEnabled(false);
    ui->btnShowHideRemovedItems->setEnabled(false);
    ui->btnComplex->setEnabled(false);
}

RTools::~RTools()
{
    delete ui;
}

void RTools::on_btnExit_clicked()
{
    reject();
}

void RTools::on_btnShowHideRemovedItems_clicked()
{
    accept();
    fDesk->showHideRemovedItems();
}

void RTools::on_btnOrderComment_clicked()
{
    accept();
    fDesk->setOrderComment();
}

void RTools::on_btnMoveTable_clicked()
{
    accept();
    fDesk->moveTable();
}

void RTools::on_btnRemoveOrder_clicked()
{
    accept();
    fDesk->removeOrder();
}

void RTools::on_btnShowTableOrders_clicked()
{
    accept();
    fDesk->showTableOrders();
}


void RTools::on_btnShowNyTotal_clicked()
{
    accept();
    fDesk->showMyTotal();
}

void RTools::on_btnInitCash_clicked()
{
    accept();
    fDesk->initialCash();
}

void RTools::on_btnComplex_clicked()
{
    accept();
    fDesk->setComplexMode();
}

void RTools::on_btnTotalReport_clicked()
{
    accept();
    fDesk->printTotalToday();
}

void RTools::on_btnTotalYesterday_clicked()
{
    accept();
    fDesk->printTotalYesterday();
}

void RTools::on_btnCompimentary_clicked()
{
    accept();
    fDesk->complimentary();
}

void RTools::on_btnTax_clicked()
{
    accept();
    fDesk->printTaxDialog();
}

void RTools::on_btnChangePassword_clicked()
{
    accept();
    fDesk->changePassword();
}

void RTools::on_btnRecover_clicked()
{
    accept();
    fDesk->recover();
}

void RTools::on_btnTotalReportAnyDay_clicked()
{
    accept();
    fDesk->printTotalAnyDay();
}

void RTools::on_btnPrintAnyReport_clicked()
{
    accept();
    fDesk->printReceiptByNumber();
}

void RTools::on_btnVoidBack_clicked()
{
    accept();
    fDesk->voidBack();
}

void RTools::on_btnPrintVoid_clicked()
{
    accept();
    fDesk->printVoidReport();
}


void RTools::on_btnCloseDay_clicked()
{
    accept();
    fDesk->closeDay();
}

void RTools::on_btnSalary_clicked()
{
    accept();
    fDesk->salary();
}

void RTools::on_btnTotalShort_clicked()
{
    accept();
    fDesk->printTotalShort();
}

void RTools::on_btnCardStat_clicked()
{
    accept();
    fDesk->cardStat();
}

void RTools::on_btnVisitStat_clicked()
{
    accept();
    fDesk->visitStat();
}

void RTools::on_btnCheckCard_clicked()
{
    accept();
    fDesk->checkCardAmount();
}

void RTools::on_btnPrintDailySales_clicked()
{
    accept();
    fDesk->saledItem();
}
