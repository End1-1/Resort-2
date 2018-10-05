#ifndef DLGTRANSFER_H
#define DLGTRANSFER_H

#include "baseextendeddialog.h"
#include "dwselectoractiveroom.h"

namespace Ui {
class DlgTransfer;
}

class DlgTransfer : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTransfer(QWidget *parent = 0);
    ~DlgTransfer();
    void setRoomByCode(const QString &code);
    void addRow(QList<QVariant> row);
private slots:
    void roomTo(CI_ActiveRoom *c);
    void on_chGuest_clicked(bool checked);
    void on_chCompany_clicked(bool checked);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgTransfer *ui;
    DWSelectorActiveRoom *fDockRoom;
};

#endif // DLGTRANSFER_H
