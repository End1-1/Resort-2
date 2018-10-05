#include "dlgmain.h"
#include "ui_dlgmain.h"
#include "preferences.h"
#include <QTcpSocket>
#include <QFile>
#include <QProcess>

bool autoStart = true;

DlgMain::DlgMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMain)
{
    ui->setupUi(this);
    fStarted = false;
}

DlgMain::~DlgMain()
{
    delete ui;
}

void DlgMain::on_btnUpdate_clicked()
{
    if (fStarted) {
        stop();
    } else {
        start();
    }
}

void DlgMain::start()
{
    fStarted = true;
    ui->btnUpdate->setText("Cancel update");
    Preferences p;
    p.initFromConfig();
    Db db = p.getFirstDatabase();

    QTcpSocket s;
    s.connectToHost(db.dc_main_host, 1250);
    if (!s.waitForConnected(5000)) {
        ui->lbState->setText("Failed to connect to the server");
        stop();
        return;
    }
    QString data = QString("{\"db\": {\"database\" : \"%1\", \"user\" : \"%2\", \"password\" : \"%3\"}, "
                           "\"command\":{\"command\":\"%4\" %5}} ")
            .arg("update")
            .arg("update")
            .arg("update")
            .arg("update")
            .arg(",\"file\":\"restaurant\"");
    int size = data.toUtf8().length();
    QByteArray ds;
    ds.append(reinterpret_cast<const char*>(&size), sizeof(size));
    ds.append(data.toUtf8(), data.toUtf8().length());
    s.write(ds, ds.length());
    s.flush();
    if (!s.waitForReadyRead(5000)) {
        stop();
        s.disconnectFromHost();
        ui->lbState->setText("Update failed. No files were found on the server.");
        return;
    }
    qint32 fileSize;
    s.read(reinterpret_cast<char *>(&fileSize), sizeof(quint32));
    if (fileSize < 0) {
        stop();
        s.disconnectFromHost();
        ui->lbState->setText("Update failed. No files were found on the server.");
        return;
    }
    ui->pb->setMaximum(fileSize + 400);
    qint32 total = 0;
    QFile f("./update.7z");
    f.open(QIODevice::WriteOnly);
    char buff[1024];
    s.waitForReadyRead(5000);
    while (total < fileSize) {
        int readed = s.read(buff, 1024);
        if (readed == 0) {
            if (!s.waitForReadyRead(5000)) {
                ui->lbState->setText("Update failed. Receive error.");
                s.close();
                stop();
                return;
            }
        }
        f.write(buff, readed);
        total += readed;
        ui->pb->setValue(total);
        qApp->processEvents();
    }
    f.close();
    s.disconnectFromHost();
    QStringList args;
    args << "e" << "update.7z" << "-y" << "-oupdate";
    QProcess *ps = new QProcess();
    ps->start("./7z.exe", args);
    ps->waitForFinished(-1);
    delete ps;
    ps = new QProcess();
    args.clear();
    ps->start("./update/start.bat", args);
    ps->waitForFinished(-1);
    ui->pb->setValue(ui->pb->maximum());
    stop();
    qApp->quit();
}

void DlgMain::stop()
{
    fStarted = false;
    ui->btnUpdate->setText("Update");
}
