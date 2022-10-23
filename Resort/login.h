#ifndef LOGIN_H
#define LOGIN_H

#include "baseextendeddialog.h"
#include <QUdpSocket>
#include <QTimer>

namespace Ui {
class Login;
}

class Login : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void setLockUser(int id);
private slots:
    void shortcutSlot();
    void timeout();
    void readDatagram();
    void on_btnCancel_clicked();
    void on_btnAddDatabase_clicked();
    void on_btnLogin_clicked();
    void on_leUsername_textChanged(const QString &arg1);
    void on_btnDatabases_clicked();

private:
    Ui::Login *ui;
    int fLockUser;
    QUdpSocket fUdpSocket;
    QTimer fTimer;
    QMap<QString, QMap<QString, QString> > fDatabaseList;
    void getDatabases();
};

#endif // LOGIN_H
