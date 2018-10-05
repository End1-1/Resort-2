#ifndef DLGBANKET_H
#define DLGBANKET_H

#include "baseextendeddialog.h"
#include "user.h"

namespace Ui {
class DlgBanket;
}

class DlgBanket : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgBanket(User *user, QWidget *parent = 0);
    ~DlgBanket();

private slots:
    void on_btnExit_clicked();

    void on_btnGuestQty_clicked();

    void on_btnPrice_clicked();

    void on_btnPayment_clicked();

    void on_btnSelectService_clicked();

    void on_btnPrintReceipt_clicked();

    void on_btnSave_clicked();

    void on_btnHall_clicked();

    void on_btnTaxPrint_clicked();

    void on_btnComment_clicked();

    void on_btnDraft_clicked();

    void on_btnOpen_clicked();

    void on_btnNew_clicked();

    void on_btnPrepayment_clicked();

private:
    Ui::DlgBanket *ui;
    void printTax();
    int fFiscal;
    QString fDoc;
    User *fUser;
    bool isCorrect();
};

#endif // DLGBANKET_H
