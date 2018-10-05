#ifndef DLGNOSHOW_H
#define DLGNOSHOW_H

#include "basedialog.h"
#include "dwselectorpaymentmode.h"

namespace Ui {
class DlgNoShow;
}

class DlgNoShow : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgNoShow(QWidget *parent = 0);
    ~DlgNoShow();
    void setReservation(const QString &reserv);
    void load(const QString &id);
private slots:
    void timeout();
    void on_btnClose_clicked();
    void on_btnPrint_clicked();
    void on_btnSave_clicked();
    void on_btnPrintTax_clicked();

private:
    Ui::DlgNoShow *ui;
    void getBalance();
    DWSelectorPaymentMode *fDockPay;
};

#endif // DLGNOSHOW_H
