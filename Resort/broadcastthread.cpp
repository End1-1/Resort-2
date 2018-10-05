#include "broadcastthread.h"
#include <QNetworkProxy>
#include "appconfig.h"
#include <QMessageBox>

BroadcastThread::BroadcastThread(QObject *parent) :
    QThread(parent),
    Base()
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

BroadcastThread::~BroadcastThread()
{
}

void BroadcastThread::cmdRefreshCache(int cache, const QString &item)
{
    Db db = fPreferences.getDatabase(fDbName);
    QString data = QString("{\"db\": {\"database\" : \"%1\", \"user\" : \"%2\", \"password\" : \"%3\"}, "
                           "\"command\" : {\"command\": \"update_cache\", \"cache\":%4, \"item\":\"%5\"}}")
            .arg(db.dc_main_path)
            .arg("userjan")
            .arg("passwordjan")
            .arg(cache)
            .arg(item);
    sendData(data);
}

void BroadcastThread::cmdCommand(int command, const QMap<QString, QString> &params)
{
    Db db = fPreferences.getDatabase(fDbName);
    QString buildData;
    QMapIterator<QString, QString> it(params);
    while (it.hasNext()) {
        it.next();
        buildData += QString(",\"%1\":\"%2\"")
                .arg(it.key())
                .arg(it.value());
    }
    QString data = QString("{\"db\": {\"database\" : \"%1\", \"user\" : \"%2\", \"password\" : \"%3\"}, "
                           "\"command\":{\"command\":\"%4\" %5}} ")
            .arg(db.dc_main_path)
            .arg("userjan")
            .arg("passwordjan")
            .arg(command)
            .arg(buildData);
    sendData(data);
}

void BroadcastThread::run()
{
    fSocket = new QTcpSocket();
    connect(fSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    fSocket->setProxy(QNetworkProxy::NoProxy);
    fSocket->connectToHost(AppConfig::fServerAddress, 1250);
    if (fSocket->waitForConnected()) {
        fSocket->write(fData, fData.length());
        fSocket->flush();
        fSocket->disconnectFromHost();
    } else {
        QMessageBox::critical(0, tr("Broadcast socket error"), fSocket->errorString());
    }
    fSocket->deleteLater();
    quit();
}

void BroadcastThread::sendData(const QString &data)
{
    int size = data.toUtf8().length();
    BroadcastThread *b = new BroadcastThread();
    b->fData.append(reinterpret_cast<const char*>(&size), sizeof(size));
    b->fData.append(data.toUtf8(), data.toUtf8().length());
    b->start();
}

void BroadcastThread::socketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err)
    QTcpSocket *s = static_cast<QTcpSocket*>(sender());
    QMessageBox::critical(0, tr("Broadcast socket error"), s->errorString());
}
