#ifndef DLGTRANSFERINVOICEAMOUNT_H
#define DLGTRANSFERINVOICEAMOUNT_H

#include "baseextendeddialog.h"
#include "cacheactiveroom.h"
#include "dwselectoractiveroom.h"

namespace Ui {
class DlgTransferInvoiceAmount;
}

class DlgTransferInvoiceAmount : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTransferInvoiceAmount(QWidget *parent = 0);
    ~DlgTransferInvoiceAmount();
    void setRoomFrom(CI_ActiveRoom *r);
private slots:
    void roomFrom(CI_ActiveRoom *c);
    void roomTo(CI_ActiveRoom *c);
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_leAmountTo_textChanged(const QString &arg1);

    void on_btnPrint_clicked();

private:
    Ui::DlgTransferInvoiceAmount *ui;
    DWSelectorActiveRoom *fDockRoomFrom;
    DWSelectorActiveRoom *fDockRoomTo;
    void getAmount(const QString &invoice);
};

#endif // DLGTRANSFERINVOICEAMOUNT_H
