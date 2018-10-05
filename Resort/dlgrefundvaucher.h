#ifndef DLGREFUNDVAUCHER_H
#define DLGREFUNDVAUCHER_H

#include "basedialog.h"
#include "dwselectorpaymentmode.h"
#include "dwselectorcityledger.h"

namespace Ui {
class DlgRefundVaucher;
}

class DlgRefundVaucher : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgRefundVaucher(QWidget *parent = 0);
    ~DlgRefundVaucher();
    static void refundVaucher(const QString &vaucher, const QString &reserv);
private slots:
    void on_btnSave_clicked();

    void on_btnPrint_clicked();

    void on_btnCancel_clicked();

    void on_rgGuest_clicked(bool checked);

    void on_rgCL_clicked(bool checked);

private:
    Ui::DlgRefundVaucher *ui;
    DWSelectorCityLedger *fDockCL;
    DWSelectorPaymentMode *fDockPay;
    void setReservation(const QString &reserv);
    void getBalance();
};

#endif // DLGREFUNDVAUCHER_H
