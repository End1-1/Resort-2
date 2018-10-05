#ifndef DLGPAYMENTSDETAILS_H
#define DLGPAYMENTSDETAILS_H

#include "baseextendeddialog.h"
#include "dwselectorcityledger.h"
#include "dwselectorcreditcard.h"
#include "dwselectoractiveroom.h"

namespace Ui {
class DlgPaymentsDetails;
}

class DlgPaymentsDetails : public BaseExtendedDialog
{
    Q_OBJECT
public:
    explicit DlgPaymentsDetails(QWidget *parent = 0);
    ~DlgPaymentsDetails();
    void setInvoice(const QString &id);
    bool isZeroBalance();
    void setCheckoutFlag();
private slots:
    void refundLineFocusIn();
    void refundRemove(int tag);
    void refundTextChanged(const QString &text);
    void room(CI_ActiveRoom *c);
    void cityLedgerFocusIn();
    void cardFocusIn();
    void cityLedger(CI_CityLedger* c);
    void cityRefund(CI_CityLedger *c);
    void creditCard(CI_CreditCard *c);
    void removeRowClicked(int tag);
    void amountLineEditEdited(const QString &text);
    void on_btnCash_clicked();
    void on_btnCard_clicked();
    void on_btnCityLedger_clicked();
    void on_btnBank_clicked();
    void on_btnSave_clicked();
    void on_leRoomCode_returnPressed();

private:
    int fCheckoutFlag;
    Ui::DlgPaymentsDetails *ui;
    DWSelectorActiveRoom *fDockRoom;
    DWSelectorCreditCard *fDockCard;
    DWSelectorCityLedger *fDockCityLedger;
    DWSelectorCityLedger *fDockCityRefund;
    QDate fArrival;
    QDate fDeparture;
    void countTotal();
    void printPayment(EQTableWidget *t, QList<int> &rows);
    double countTotal(QTableWidget *t, int curr);
    void countRefund();
    void newPaidRow(int mode);
    void newRefundRow(int mode);
    void setRowValues(QTableWidget *t, int row, const QList<QVariant> &values);
    EQLineEdit *createLineEdit(QTableWidget *t, int row, int column);
    EQLineEdit *lineEdit(QTableWidget *t, int row, int column);
    bool savePayment(QTableWidget *t, int side, QList<int> &printRows);
};

#endif // DLGPAYMENTSDETAILS_H
