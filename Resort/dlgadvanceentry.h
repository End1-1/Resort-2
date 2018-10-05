#ifndef DLGADVANCEENTRY_H
#define DLGADVANCEENTRY_H

#include "roweditordialog.h"
#include "dwselectorpaymentmode.h"
#include "dwselectorredreservation.h"
#include "dwselectorcreditcard.h"
#include "dwselectorcityledger.h"

namespace Ui {
class DlgAdvanceEntry;
}

class DlgAdvanceEntry : public RowEditorDialog
{
    Q_OBJECT

public:
    static void advance(CI_RedReservation *c = 0);
    ~DlgAdvanceEntry();

protected:
    virtual bool isDataCorrect();

private slots:
    void redReservation(CI_RedReservation *c);
    void paymentMode(CI_PaymentMode *c);
    void cityLedger(CI_CityLedger *c);
    void creditCard(CI_CreditCard *c);
    void on_btnCancel_clicked();
    void on_btnSave_clicked();

    void on_leAmount_textChanged(const QString &arg1);

    void on_btnPrint_clicked();

    void on_btnNew_clicked();

    void on_btnPrintTax_clicked();

private:
    explicit DlgAdvanceEntry(QList<QVariant> &values, QWidget *parent = 0);
    Ui::DlgAdvanceEntry *ui;
    DWSelectorCityLedger *fDockCL;
    DWSelectorCreditCard *fDockCard;
    DWSelectorPaymentMode *fDockPayment;
    DWSelectorRedReservation *fDockReserv;
    CI_RedReservation *fReserve;
};

#endif // DLGADVANCEENTRY_H
