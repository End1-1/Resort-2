#ifndef DLGCHECKADVANCEBEFORECANCEL_H
#define DLGCHECKADVANCEBEFORECANCEL_H

#include "basedialog.h"

#define CR_CANCEL 1
#define CR_NOCANCEL 2

namespace Ui {
class DlgCheckAdvanceBeforeCancel;
}

class DlgCheckAdvanceBeforeCancel : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgCheckAdvanceBeforeCancel(const QString &inv, QWidget *parent = 0);
    ~DlgCheckAdvanceBeforeCancel();
    static int checkAdvance(const QString &inv);
private slots:

    void on_btnCancel_clicked();

    void on_btnNoShow_clicked();

    void on_btnRefund_clicked();

    void on_btnCancelreservation_clicked();

    void on_btnTransferToCL_clicked();

private:
    Ui::DlgCheckAdvanceBeforeCancel *ui;
    QString fInvoice;
    bool getBalance();
};

#endif // DLGCHECKADVANCEBEFORECANCEL_H
