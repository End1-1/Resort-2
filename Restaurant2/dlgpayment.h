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
    static bool payment(int order, int hallid);
    virtual void accept();
    virtual void reject();

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
    void on_leCard_textChanged(const QString &arg1);
    void on_btnTaxPayerId_clicked();
    void on_btnCouponService_clicked(bool checked);

private:
    Ui::DlgPayment *ui;
    int fOrder;
    int fHall;
    QString fDefaultFiscalMachine;
    bool fUpdateHeader;
    void calcCash();
    void calcCard();
    int fCountId;
    bool fCanReject;
    QMap<QString, QMap<QString, QVariant> > fFiscalMachines;
    void readFiscalMachines();
};

#endif // DLGPAYMENT_H
