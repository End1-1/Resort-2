#ifndef DLGPAYDEBT_H
#define DLGPAYDEBT_H

#include "baseextendeddialog.h"
#include "debtpay.h"

namespace Ui
{
class DlgPayDebt;
}

class DlgPayDebt : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPayDebt(QWidget *parent = nullptr);
    ~DlgPayDebt();
    void prepareToShow();
    static void showDialog(QWidget *parent = nullptr);

private slots:
    void on_btnSearch_clicked();
    void on_btnShowUnpaid_clicked();
    void on_btnCash_clicked();
    void on_btnCard_clicked();
    void on_btnPayTalon_clicked();
    void on_btnCancel_clicked();

private:
    void searchDebts();
    void showUnpaidDebts();
    void fillDebts(const QList<OpenDebtRow> &rows);
    void refreshDebts();
    bool paySelected(int paymentMode);
    bool selectedDebt(int &orderId, QString &govNumber, double &balance);

    Ui::DlgPayDebt *ui;
    bool fShowAllUnpaid = false;
};

#endif // DLGPAYDEBT_H
