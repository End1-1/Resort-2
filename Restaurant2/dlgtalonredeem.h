#ifndef DLGTALONREDEEM_H
#define DLGTALONREDEEM_H

#include "baseextendeddialog.h"
#include "talonservice.h"

namespace Ui {
class DlgTalonRedeem;
}

class DlgTalonRedeem : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTalonRedeem(QWidget *parent = nullptr);
    ~DlgTalonRedeem();

    static bool confirm(const TalonRedeemInfo &info, double orderAmount, QWidget *parent);

private slots:
    void on_btnApply_clicked();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgTalonRedeem *ui;
};

#endif // DLGTALONREDEEM_H
