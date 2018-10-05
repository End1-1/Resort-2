#include "dlgmain.h"
#include "ui_dlgmain.h"
#include "defines.h"
#include <QCloseEvent>
#include <QHostAddress>
#include <QTcpSocket>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSettings>
#include <QNetworkProxy>

DlgMain::DlgMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMain)
{
    ui->setupUi(this);

    QSettings s("SmartHotel", "SmartHotel");
    ui->leServerPort->setText(s.value("server_port").toString());
    ui->leHost->setText(s.value("host").toString());
    ui->leDatabase->setText(s.value("database").toString());
    ui->leUsername->setText(s.value("username").toString());
    ui->lePassword->setText(s.value("password").toString());

    ui->leLogHost->setText(s.value("loghost").toString());
    ui->leLogDb->setText(s.value("logdb").toString());
    ui->leLogUsername->setText(s.value("logusername").toString());
    ui->leLogPassword->setText(s.value("logpassword").toString());

    ui->rgGenRangom->setChecked(s.value("idgen").toBool());

    fTrayMenu.addAction(tr("Quit"), this, SLOT(appTerminate()));
    fCanClose = false;
    fTrayIcon.setIcon(QIcon(":/app.png"));
    fTrayIcon.show();
    fTrayIcon.setContextMenu(&fTrayMenu);
    connect(&fTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconClicked(QSystemTrayIcon::ActivationReason)));
    fTcpServer.setProxy(QNetworkProxy::NoProxy);
    if (!fTcpServer.listen(QHostAddress::Any, ui->leServerPort->text().toInt())) {
        logActivity("Cannot listen.");
    }
    connect(&fTcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if (!fUdpSocket.bind(DATAGRAM_PORT, QUdpSocket::ShareAddress)) {
        logActivity("Cannot bind udp server.");
    }
    fTcpServer.
    connect(&fUdpSocket, SIGNAL(readyRead()), this, SLOT(datagramRead()));

}

DlgMain::~DlgMain()
{
    for (QMap<QTcpSocket*, Command*>::const_iterator it = fTcpSockets.begin(); it != fTcpSockets.end(); it++) {
        it.key()->close();
    }
    qDeleteAll(fTcpSockets);
    fTcpSockets.clear();
    delete ui;
}

void DlgMain::closeEvent(QCloseEvent *e)
{
    if (fCanClose) {
        QDialog::closeEvent(e);
        return;
    } else {
        hide();
        e->ignore();
        return;
    }
}

void DlgMain::logActivity(const QString &action)
{
    QDateTime d = QDateTime::currentDateTime();
    QString log = QString("%1: %2")
            .arg(d.toString(def_date_time_format))
            .arg(action);
    QListWidgetItem *item = new QListWidgetItem(ui->lwActivity);
    item->setText(log);
    ui->lwActivity->addItem(item);
}

void DlgMain::appTerminate()
{
    fCanClose = true;
    qApp->quit();
}

void DlgMain::iconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        setVisible(true);
        break;
    default:
        break;
    }
}

void DlgMain::newConnection()
{
    QTcpSocket *s = fTcpServer.nextPendingConnection();
    logActivity(QString("New connection. Client: %1:%2")
                .arg(QHostAddress(s->peerAddress().toIPv4Address()).toString())
                .arg(s->peerPort()));
    connect(s, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(s, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
    Command *cmd = new Command(s);
    fAllSockets.insert(s, cmd);
    connect(cmd, SIGNAL(parseCommand(QString)), this, SLOT(parseCommand(QString)));
}

void DlgMain::clientDisconnected()
{
    QTcpSocket *s = static_cast<QTcpSocket*>(sender());
    Command *c = 0;
    if (fTcpSockets.contains(s)) {
        c = fTcpSockets[s];
        fTcpSockets.remove(s);
    } else if (fTcpSocketsDraft.contains(s)){
        c = fTcpSocketsDraft[s];
        fTcpSocketsDraft.remove(s);
        fLockInvoice.remove(c);
        fLockReserve.remove(c);
    } else {
        c = fAllSockets[s];
    }
    fAllSockets.remove(s);
    s->deleteLater();
    if (c) {
        logActivity(QString("Connection closed. Client: %1:%2, %3")
                    .arg(QHostAddress(s->peerAddress().toIPv4Address()).toString())
                    .arg(s->peerPort())
                    .arg(c->fDatabase));
        c->deleteLater();
    }
}

void DlgMain::clientReadyRead()
{
    QTcpSocket *s = static_cast<QTcpSocket*>(sender());
    Command *cmd = fAllSockets[s];
    cmd->readBytes(s->readAll());
}

void DlgMain::datagramRead()
{
    Database db;
    db.setConnectionParams(ui->leHost->text(), ui->leDatabase->text(), ui->leUsername->text(), ui->lePassword->text());
    if (!db.open()) {
        logActivity("Cannot open db in DlgMain::datagramRead," + db.fLastError + QString("%1,%2,%3,%4")
                    .arg(ui->leHost->text())
                    .arg(ui->leDatabase->text())
                    .arg(ui->leUsername->text())
                    .arg(ui->lePassword->text()));
        return;
    }
    while (fUdpSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(fUdpSocket.pendingDatagramSize());
        QHostAddress remoteAddress;
        quint16 remotePort;
        fUdpSocket.readDatagram(datagram.data(), datagram.size(), &remoteAddress, &remotePort);
        logActivity(datagram);
        QJsonDocument jDoc = QJsonDocument::fromJson(datagram);
        QJsonObject jObj = jDoc.object();
        if (jObj.contains("server")) {
            QString req = jObj.value("server").toString();
            QString client = jObj.value("client").toString();
            DatabaseResult dr;
            QMap<QString, QVariant> dbBind;
            dbBind[":f_name"] = client;
            dr.select(db, "select f_id from f_users where lower(f_name)=lower(:f_name)", dbBind);
            if (dr.rowCount() == 0) {
                logActivity("Client not found, append new " + client);
                logActivity(db.fLastError);
                dbBind[":f_name"] = client;
                db.insert("f_users", dbBind);
                dr.select(db, "select f_id from f_db", dbBind);
                for (int i = 0; i < dr.rowCount(); i++) {
                    dbBind[":f_user"] = client;
                    dbBind[":f_db"] = dr.value(i, "f_id");
                    dbBind[":f_right"] = 0;
                    db.insert("f_access", dbBind);
                }
            }
            if (req == "who") {
                QJsonObject jObjReply;
                jObjReply["server"] = "me";
                jObjReply["server_port"] = ui->leServerPort->text();
                jObjReply["host"] = ui->leHost->text();
                jObjReply["database"] = ui->leDatabase->text();
                jObjReply["username"] = ui->leUsername->text();
                jObjReply["password"] = ui->lePassword->text();
                jObjReply["loghost"] = ui->leLogHost->text();
                jObjReply["logdb"] = ui->leLogDb->text();
                jObjReply["loguser"] = ui->leLogUsername->text();
                jObjReply["logpass"] = ui->leLogPassword->text();
                jObjReply["idgen"] = (int) ui->rgGenRangom->isChecked();
                QJsonDocument jDocReply(jObjReply);
                datagram = jDocReply.toJson();
                fUdpSocket.writeDatagram(datagram, remoteAddress, remotePort);
                logActivity(datagram);
            }
        } else if (jObj.contains("first_setup")) {
            QString password = jObj["first_setup"].toString();
            Database dbTest;
            dbTest.copyConnectionParamsFrom(db);
            dbTest.fDb.setPassword(password);
            QJsonObject jObjReply;
            jObjReply["first_setup"] = dbTest.open() ? "ok" : "false";
            dbTest.close();
            DatabaseResult dr;
            QMap<QString, QVariant> dbBind;
            if (jObjReply["first_setup"].toString() == "ok") {
                dbBind[":f_user"] = jObj["client"].toString();
                QList<QList<QVariant> > dbRows;
                db.select("delete from f_access where f_user=:f_user", dbBind, dbRows);
                dr.select(db, "select f_id from f_db", dbBind);
                for (int i = 0; i < dr.rowCount(); i++) {
                    dbBind[":f_user"] = jObj["client"].toString();
                    dbBind[":f_db"] = dr.value(i, "f_id");
                    dbBind[":f_right"] = 1;
                    db.insert("f_access", dbBind);
                }
            }
            QJsonDocument jDocReply(jObjReply);
            datagram = jDocReply.toJson();
            fUdpSocket.writeDatagram(datagram, remoteAddress, remotePort);
        }
    }
}

void DlgMain::parseCommand(const QString &command)
{
    Command *cmd = static_cast<Command*>(sender());
    logActivity("Command: " + command);
    QJsonDocument jDoc = QJsonDocument::fromJson(command.toUtf8());
    QJsonObject jObj = jDoc.object();
    QJsonObject jObjDb = jObj.value("db").toObject();
    QJsonObject jObjCmd = jObj.value("command").toObject();
    QString db = jObjDb.value("database").toString();
    cmd->fDatabase = db;
    QJsonValue jValCmd = jObjCmd.value("command");
    if (jValCmd.toString() == "identify") {
        if (cmd->identify(db)) {
            cmd->fUsername = jObjDb.value("user").toString();
            fTcpSockets.insert(cmd->fSocket, cmd);
        }
        return;
    } else if (jValCmd.toString() == "lock_invoice") {
        QString invoice = jObjCmd.value("invoice").toString();
        for (QMap<QTcpSocket*, Command*>::iterator it = fTcpSocketsDraft.begin(); it != fTcpSocketsDraft.end(); it++) {
            if (it.value()->fDatabase != cmd->fDatabase) {
                continue;
            }
            if (it.value() == cmd) {
                continue;
            }
            for (QList<Lock>::const_iterator ii = fLockInvoice[it.value()].begin(); ii != fLockInvoice[it.value()].end(); ii++) {
                if (invoice == ii->lock) {
                    logActivity(invoice + " locked");
                    return;
                }
            }
            logActivity(invoice + " not locked");
            return;
        }
    } else if (jValCmd.toString() == "draft") {
        fTcpSocketsDraft.insert(cmd->fSocket, cmd);
        fLockInvoice.insert(cmd, QList<Lock>());
        fLockReserve.insert(cmd, QList<Lock>());
        return;
    } else if (jValCmd.toString() == "update") {
        QMap<QString, QVariant> p = jObjCmd.toVariantMap();
        QString fileName = "update.7z";
        logActivity(QString("Request update"));
        QFile f(fileName);
        qint32 fileSize = -1;
        QTcpSocket *s = cmd->fSocket;
        if (f.open(QIODevice::ReadOnly)) {
            fileSize = f.size();
            s->write(reinterpret_cast<const char*>(&fileSize), sizeof(quint32));
            char buff[1024];
            while (f.pos() < f.size()) {
                int readed = f.read(buff, 1024);
                s->write(buff, readed);
            }
        } else {
            s->write(reinterpret_cast<const char*>(&fileSize), sizeof(quint32));
        }
        s->flush();
        return;
    }

    QJsonDocument jDocBroadcast(jObjCmd);
    QString newCmd = jDocBroadcast.toJson(QJsonDocument::Compact);
    int size = newCmd.toUtf8().length();
    QByteArray dataToSend;
    dataToSend.append(reinterpret_cast<const char*>(&size), sizeof(size));
    dataToSend.append(newCmd.toUtf8());
    for (QMap<QTcpSocket*, Command*>::const_iterator it = fTcpSockets.begin(); it != fTcpSockets.end(); it++) {
        if (it.value()->fDatabase != cmd->fDatabase) {
            continue;
        }
        if (!it.value()->isIdentified()) {
            continue;
        }
        it.key()->write(dataToSend, dataToSend.length());
        it.key()->flush();
    }
}

void DlgMain::on_btnSave_clicked()
{
    QSettings s("SmartHotel", "SmartHotel");
    s.setValue("server_port", ui->leServerPort->text());
    s.setValue("host", ui->leHost->text());
    s.setValue("database", ui->leDatabase->text());
    s.setValue("username", ui->leUsername->text());
    s.setValue("password", ui->lePassword->text());
    s.setValue("loghost", ui->leLogHost->text());
    s.setValue("logdb", ui->leLogDb->text());
    s.setValue("logusername", ui->leLogUsername->text());
    s.setValue("logpassword", ui->leLogPassword->text());
    s.setValue("idgen", ui->rgGenRangom->isChecked());
    Database db;
    db.setConnectionParams(ui->leHost->text(), ui->leDatabase->text(), ui->leUsername->text(), ui->lePassword->text());
    if (!db.open()) {
        logActivity(db.fLastError);
        return;
    }
}
