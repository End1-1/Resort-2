#ifndef RFACE_H
#define RFACE_H

#include <QTimer>
#include "baseextendeddialog.h"
#include "command.h"
#include "hall.h"
#include "user.h"

namespace Ui
{
class RFace;
}

class RFace : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RFace(QWidget *parent = 0);
    ~RFace();
    bool setup();
    bool fIsConfigured;

private slots:
    void socketReadyRead();
    void socketDisconnected();
    void on_tableWidget_clicked(const QModelIndex &index);
    void on_btnExit_clicked();

private:
    Ui::RFace* ui;
    Hall fHall;
    int fCurrentHall;
    int fCurrenTableState;
    int fTimerCounter;
    Command fCommand;
    User* login();
};

#endif // RFACE_H
