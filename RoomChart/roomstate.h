#ifndef ROOMSTATE_H
#define ROOMSTATE_H

#include "baseextendeddialog.h"
#include "cacheroom.h"
#include "dwselectorroomstate.h"

namespace Ui {
class RoomState;
}

class RoomState : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RoomState(QWidget *parent = 0);
    ~RoomState();
    void setRoom(CI_Room *r);
private slots:
    void roomState(CI_RoomState *c);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

    void on_deStart_textChanged(const QString &arg1);

    void on_deEnd_textChanged(const QString &arg1);

private:
    Ui::RoomState *ui;
    DWSelectorRoomState *fDockState;
    CI_Room *fRoom;
    void checkOO();
};

#endif // ROOMSTATE_H
