#ifndef DLGMAIN_H
#define DLGMAIN_H

#include "command.h"
#include "database.h"
#include "databaseresult.h"
#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTcpServer>
#include <QUdpSocket>

namespace Ui {
class DlgMain;
}

struct Lock {
    QString lock;
    QString user;
};

class DlgMain : public QDialog
{
    Q_OBJECT
public:
    explicit DlgMain(QWidget *parent = 0);
    ~DlgMain();
protected:
    virtual void closeEvent(QCloseEvent *e);
private:
    Ui::DlgMain *ui;
    bool fCanClose;
    QSystemTrayIcon fTrayIcon;
    QMenu fTrayMenu;
    QTcpServer fTcpServer;
    QUdpSocket fUdpSocket;
    QMap<QTcpSocket*, Command*> fAllSockets;
    QMap<QTcpSocket*, Command*> fTcpSockets;
    QMap<QTcpSocket*, Command*> fTcpSocketsDraft;
    QMap<Command*, QList<Lock> > fLockInvoice;
    QMap<Command*, QList<Lock> > fLockReserve;
    void logActivity(const QString &action);
private slots:
    void appTerminate();
    void iconClicked(QSystemTrayIcon::ActivationReason reason);
    void newConnection();
    void clientDisconnected();
    void clientReadyRead();
    void datagramRead();
    void parseCommand(const QString &command);
    void on_btnSave_clicked();
};

#endif // DLGMAIN_H
