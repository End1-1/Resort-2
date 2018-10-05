#ifndef DLGADVANCE_H
#define DLGADVANCE_H

#include "baseextendeddialog.h"
#include "dwselectorpaymentmode.h"
#include "dwselectorcreditcard.h"

namespace Ui {
class DlgAdvance;
}

class DlgAdvance : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgAdvance(const QString &reserve, QWidget *parent = 0);
    ~DlgAdvance();
    void setVaucherId(const QString &id);
    static void openAdvance(const QString &vaucherId);
private slots:
    void paymentMode(CI_PaymentMode *c);
    void creditCard(CI_CreditCard *c);
    void on_btnClose_clicked();

    void on_btnPrint_clicked();

    void on_btnSave_clicked();

    void on_btnPrintTax_clicked();

private:
    Ui::DlgAdvance *ui;
    DWSelectorPaymentMode *fDockPay;
    DWSelectorCreditCard *fDockCard;
};

#endif // DLGADVANCE_H
