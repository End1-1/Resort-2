#ifndef DLGCONNECTTOSERVER_H
#define DLGCONNECTTOSERVER_H

#include "baseextendeddialog.h"
#include <QUdpSocket>
#include <QTimer>

namespace Ui {
class DlgConnectToServer;
}

class DlgConnectToServer : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgConnectToServer(QWidget *parent = nullptr);
    ~DlgConnectToServer();

private:
    Ui::DlgConnectToServer *ui;
    QTimer fTimer;
    QUdpSocket fUdpSocket;

private slots:
    void timeout();
    void readDatagram();
};

#endif // DLGCONNECTTOSERVER_H
