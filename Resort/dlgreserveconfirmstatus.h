#ifndef DLGRESERVECONFIRMSTATUS_H
#define DLGRESERVECONFIRMSTATUS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgReserveConfirmStatus;
}

class DlgReserveConfirmStatus : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReserveConfirmStatus(QWidget *parent = 0);
    ~DlgReserveConfirmStatus();

private slots:
    void on_btnCancel_clicked();

    void on_btnConfirmed_clicked();

    void on_btnGuaranted_clicked();

    void on_btnTentative_clicked();

    void on_btnWaiting_clicked();

    void on_btnBlock_clicked();

private:
    Ui::DlgReserveConfirmStatus *ui;
};

#endif // DLGRESERVECONFIRMSTATUS_H
