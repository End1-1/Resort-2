#ifndef DLGMAIN_H
#define DLGMAIN_H

#include "preferences.h"
#include "database.h"
#include <QDialog>
#include <QSerialPort>
#include <QMenu>
#include <QSystemTrayIcon>

namespace Ui {
class DlgMain;
}

class DlgMain : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMain(QWidget *parent = 0);
    ~DlgMain();
    virtual void closeEvent(QCloseEvent *e);
private:
    Database fDb;
    Preferences fPref;
    Ui::DlgMain *ui;
    QSerialPort fPort;
    QString fBuffer;
    QMap<QString, int> fCallMap;
    QMap<QString, double> rates;
    QMap<QString, int> fCallType;
    QMenu fTrayMenu;
    QSystemTrayIcon fTrayIcon;
    bool fCanClose;
    void processLine(const QString &line);
    void processLine1(const QString &line);
    void callLog(const QString &txt);
    void writeToFile(const QString &line);
private slots:
    void readyRead();
    void portError(QSerialPort::SerialPortError serialPortError);
    void iconClicked(QSystemTrayIcon::ActivationReason r);
    void appTerminate();
    void on_leUserId_textChanged(const QString &arg1);
    void on_leHost_textChanged(const QString &arg1);
    void on_leDb_textChanged(const QString &arg1);
    void on_leUsername_textChanged(const QString &arg1);
    void on_lePassword_textChanged(const QString &arg1);
    void on_btnReconnect_clicked();
    void on_chFileLog_clicked(bool checked);
    void on_leATSPort_textChanged(const QString &arg1);
    void on_cbATS_currentIndexChanged(int index);
    void on_cbComData_currentIndexChanged(int index);
    void on_cbComParity_currentIndexChanged(int index);
    void on_leLine_textChanged(const QString &arg1);
    void on_leIncomingNumber_textChanged(const QString &arg1);
    void on_lePathToExecutable_textChanged(const QString &arg1);
    void on_leTime_textChanged(const QString &arg1);
    void on_leAirHost_textChanged(const QString &arg1);
    void on_leAirDb_textChanged(const QString &arg1);
    void on_leAirUser_textChanged(const QString &arg1);
    void on_leAirPass_textChanged(const QString &arg1);
};

#endif // DLGMAIN_H

