#ifndef DLGPAYMENT_H
#define DLGPAYMENT_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgPayment;
}

class DlgPayment : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPayment(QWidget *parent = 0);
    ~DlgPayment();
    static bool payment(const QString &order);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_btnCard_clicked();

    void on_btnDept_clicked();

    void on_leDept_textChanged(const QString &arg1);

    void on_btnDeptHolder_clicked();

    void on_btnCouponNumber_clicked();

    void on_btnCouponSerail_clicked();

    void on_btnCash_clicked();

    void on_btnPrintTax_clicked();

    void on_leDiscount_returnPressed();

private:
    Ui::DlgPayment *ui;
    QString fOrder;
    bool fUpdateHeader;
    void calcCash();
    int fCountId;
};

#endif // DLGPAYMENT_H
