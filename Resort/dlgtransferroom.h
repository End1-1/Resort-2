#ifndef DLGTRANSFERROOM_H
#define DLGTRANSFERROOM_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgTransferRoom;
}

class DlgTransferRoom : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTransferRoom(QWidget *parent = 0);
    ~DlgTransferRoom();

private slots:
    void on_leRoomCode1_returnPressed();

    void on_leRoomCode2_returnPressed();

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgTransferRoom *ui;
};

#endif // DLGTRANSFERROOM_H
