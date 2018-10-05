#include "dlgconnecttoserver.h"
#include "ui_dlgconnecttoserver.h"
#include "rmessage.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkInterface>

DlgConnectToServer::DlgConnectToServer(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgConnectToServer)
{
    ui->setupUi(this);
    connect(&fUdpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
    timeout();
    connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    fTimer.start(2000);
}

DlgConnectToServer::~DlgConnectToServer()
{
    delete ui;
}

void DlgConnectToServer::timeout()
{
    QJsonObject jObj;
    jObj["server"] = "who";
    jObj["client"] = HOSTNAME + "." + fPreferences.hostUsername() + ".restaurant";
    QJsonDocument jDoc(jObj);
    QByteArray datagram = jDoc.toJson();
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface i, nets) {
        QList<QNetworkAddressEntry> addrs = i.addressEntries();
        foreach (QNetworkAddressEntry a, addrs) {
            if (a.ip().protocol() != QAbstractSocket::IPv4Protocol) {
                continue;
            }
            if (a.broadcast().toString().isEmpty()) {
                continue;
            }
            fUdpSocket.writeDatagram(datagram, a.broadcast(), DATAGRAM_PORT);
        }
    }
}

void DlgConnectToServer::readDatagram()
{
    fTimer.stop();
    while (fUdpSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(fUdpSocket.pendingDatagramSize());
        fUdpSocket.readDatagram(datagram.data(), datagram.size());
        QJsonDocument jDoc = QJsonDocument::fromJson(datagram);
        QJsonObject jObj = jDoc.object();
        if (jObj.contains("server")) {
            QString rep = jObj.value("server").toString();
            if (rep == "me") {
                Database db;
                db.setConnectionParams(jObj.value("host").toString(),
                                       jObj.value("database").toString(),
                                       jObj.value("username").toString(),
                                       jObj.value("password").toString());
                if (!db.open()) {
                    message_error(QString("Cannot connect to main database") + QString("<br>") + db.fLastError);
                    reject();
                    return;
                }
                DatabaseResult dr;
                fDbBind[":f_user"] = HOSTNAME + "." + fPreferences.hostUsername() + ".restaurant";
                dr.select(db, "select * from f_db where f_id in "
                          "(select f_db from f_access where lower(f_user)=lower(:f_user) and f_right=1)", fDbBind);
                for (int i = 0; i < dr.rowCount(); i++) {
                    QMap<QString, QString> conn;
                    conn["f_host"] = dr.value(i, "f_host").toString();
                    conn["f_database"] = dr.value(i, "f_database").toString();
                    conn["f_username"] = dr.value(i, "f_username").toString();
                    conn["f_password"] = dr.value(i, "f_password").toString();
                    fPreferences.appendDatabase("Main", dr.value(i, "f_host").toString(),
                                                dr.value(i, "f_database").toString(),
                                                dr.value(i, "f_username").toString(),
                                                dr.value(i, "f_password").toString(),
                                                "", "", "", "");
                    accept();
                    return;
                }
            }
        }
    }
    fTimer.start(2000);
}
