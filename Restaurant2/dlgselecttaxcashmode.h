#ifndef DLGSELECTTAXCASHMODE_H
#define DLGSELECTTAXCASHMODE_H

#include"baseextendeddialog.h"

#define tax_mode_cash 1
#define tax_mode_card 2

namespace Ui {
class DlgSelectTaxCashMode;
}

class DlgSelectTaxCashMode : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgSelectTaxCashMode(QWidget *parent = 0);
    ~DlgSelectTaxCashMode();
    static bool getCashMode(int &mode);
private slots:
    void on_btnCash_clicked();

    void on_btnCashLess_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgSelectTaxCashMode *ui;
    int fResult;
};

#endif // DLGSELECTTAXCASHMODE_H
