#ifndef DLGINVOICEPRINTOPTION_H
#define DLGINVOICEPRINTOPTION_H

#include "baseextendeddialog.h"

#define pio_none 0
#define pio_guest 1
#define pio_comp 2
#define pio_guestcomp_ser 3
#define pio_guestcomp_tog 4

namespace Ui {
class DlgInvoicePrintOption;
}

class DlgInvoicePrintOption : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgInvoicePrintOption(QWidget *parent = 0);
    ~DlgInvoicePrintOption();
    static int getOption();
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_rbGuest_clicked();

    void on_rbCompany_clicked();

    void on_rbGuestCompanySeparate_clicked();

    void on_rbGuestCompanyTogether_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::DlgInvoicePrintOption *ui;
    int fResult;
};

#endif // DLGINVOICEPRINTOPTION_H
