#ifndef DLGCLINITIALBALANCE_H
#define DLGCLINITIALBALANCE_H

#include "baseextendeddialog.h"
#include "cachecityledger.h"

namespace Ui {
class DlgCLInitialBalance;
}

class DlgCLInitialBalance : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgCLInitialBalance(int clCode, QWidget *parent = 0);
    ~DlgCLInitialBalance();
    static void openVaucher(const QString &id);
private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgCLInitialBalance *ui;
    CI_CityLedger *fCityLedger;
};

#endif // DLGCLINITIALBALANCE_H
