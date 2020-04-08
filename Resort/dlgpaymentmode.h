#ifndef DLGPAYMENTMODE_H
#define DLGPAYMENTMODE_H

#include "basedialog.h"
#include "dwselectorpaymentmode.h"
#include "dwselectorcreditcard.h"

namespace Ui {
class DlgPaymentMode;
}

class DlgPaymentMode : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgPaymentMode(QWidget *parent = 0);
    ~DlgPaymentMode();
    static bool getPayment(int &paymentMode, QString &comment, int &cl);
private slots:
    void paymentMode(CI_PaymentMode *ci);
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgPaymentMode *ui;
    DWSelectorPaymentMode *fDockPayment;
    DWSelectorCreditCard *fDockCard;
};

#endif // DLGPAYMENTMODE_H
