#ifndef DLGRESERVEQUICKINFO_H
#define DLGRESERVEQUICKINFO_H

#include "baseextendeddialog.h"
#include "cachereservation.h"

namespace Ui {
class DlgReserveQuickInfo;
}

class DlgReserveQuickInfo : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReserveQuickInfo(CI_Reservation *reservation, QWidget *parent = 0);
    ~DlgReserveQuickInfo();
protected:
    virtual void escapePressed();
private slots:
    void on_btnOpenInvoice_clicked();
    void on_btnOpenReserve_clicked();

    void on_btnGroup_clicked();

private:
    Ui::DlgReserveQuickInfo *ui;
    CI_Reservation *fReservation;

};

#endif // DLGRESERVEQUICKINFO_H
