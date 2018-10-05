#ifndef DLGRECEIPTVAUCHER_H
#define DLGRECEIPTVAUCHER_H

#include "baseextendeddialog.h"
#include "dwselectoractiveroom.h"
#include "dwselectorcityledger.h"
#include "dwselectorinvoiceitem.h"
#include "dwselectorcreditcard.h"
#include "dwselectorpaymentmode.h"

namespace Ui {
class DlgReceiptVaucher;
}

class DlgReceiptVaucher : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReceiptVaucher(QWidget *parent = 0);
    ~DlgReceiptVaucher();
    void setVaucher(const QString &id);

private slots:
    void roomChanged(CI_ActiveRoom *c);
    void cityLedger(CI_CityLedger *c);
    void paymentMode(CI_PaymentMode *c);
    void creditCard(CI_CreditCard *c);
    void on_btnSave_clicked();
    void on_rbGuest_clicked(bool checked);
    void on_rbCityLedger_clicked(bool checked);
    void on_btnCancel_clicked();
    void on_btnPrint_clicked();
    void on_leAmountAMD_textEdited(const QString &arg1);
    void on_btnNew_clicked();

private:
    Ui::DlgReceiptVaucher *ui;
    DWSelectorActiveRoom *fDockRoom;
    DWSelectorCityLedger *fDockCL;
    DWSelectorPaymentMode *fDockPM;
    DWSelectorCreditCard *fDockCard;
    void saveRoom();
    void saveCityLedger();
    void cardVisible(bool v);
};

#endif // DLGRECEIPTVAUCHER_H
