#ifndef DLGONEBREAKFASAT_H
#define DLGONEBREAKFASAT_H

#include "baseextendeddialog.h"
#include "user.h"

namespace Ui {
class DlgOneBreakfasat;
}

class DlgOneBreakfasat : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgOneBreakfasat(User *u, QWidget *parent = 0);
    ~DlgOneBreakfasat();

private slots:
    void on_btnPrint_clicked();

    void on_lePax_textChanged(const QString &arg1);

    void on_lePrice_textChanged(const QString &arg1);

    void on_btnExit_clicked();

    void on_btnPax_clicked();

    void on_btnPrice_clicked();

    void on_btnPayment_clicked();

    void on_btnPrintTax_clicked();

    void on_btnPrePrint_clicked();

    void on_btnOpenDraft_clicked();

    void on_btnRoom_clicked();

private:
    Ui::DlgOneBreakfasat *ui;
    User *fUser;
    QString fBreakfastId;
    void hideAllMode();
    void setModeCard();
    void setModeCityLedger();
    void setModeRoom();
};

#endif // DLGONEBREAKFASAT_H
