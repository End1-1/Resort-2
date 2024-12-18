#include "socketconnection.h"
#include "rawmessage.h"
#include "messagelist.h"
#include <QSslSocket>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QApplication>
#include <QMutex>
#include <QDebug>

SocketConnection *SocketConnection::fInstance = nullptr;
quint32 SocketConnection::fTcpPacketNumber = 0;
quint32 SocketConnection::fMessageId = 0;
QMutex fMutex;

SocketConnection::SocketConnection(QObject *parent) :
    QObject(parent)
{

}

SocketConnection::~SocketConnection()
{
    fTimer->deleteLater();
    fSocket->deleteLater();
}

void SocketConnection::startConnection(const QString &ip, int port, const QString &username, const QString &password)
{
    initInstance();
    fInstance->setConnectionParams(ip, port, username, password);
    auto *t = new QThread();
    connect(t, &QThread::finished, t, &QThread::deleteLater);
    connect(t, &QThread::started, fInstance, &SocketConnection::run);
    connect(fInstance, &SocketConnection::stopped, t, &QThread::quit);
    fInstance->moveToThread(t);
    t->start();
}

void SocketConnection::setConnectionParams(const QString &ip, int port, const QString &username, const QString &password)
{
    fInstance->fHostIP = ip;
    fInstance->fHostPort = port;
    fInstance->fHostUsername = username;
    fInstance->fHostPassword = password;
}

SocketConnection *SocketConnection::instance()
{
    if (fInstance == nullptr) {
        initInstance();
    }
    return fInstance;
}

void SocketConnection::run()
{
    QString certFileName = qApp->applicationDirPath() + "/" + "cert.pem";
    QFile file(certFileName);
    file.open(QIODevice::ReadOnly);
    QSslCertificate cert = QSslCertificate(file.readAll());
    fSocket = new SslSocket(this);
    fTimer = new QTimer(this);
    connect(fTimer, &QTimer::timeout, this, &SocketConnection::timeout);
    connect(fSocket, &QSslSocket::readyRead, this, &SocketConnection::readyRead);
    //connect(fSocket, &QSslSocket::errorOccurred, this, &SocketConnection::errorOccurred);
    connect(fSocket, &QSslSocket::encrypted, this, &SocketConnection::encrypted);
    connect(fSocket, &QSslSocket::disconnected, this, &SocketConnection::disconnected);
    connect(this, &SocketConnection::dataReady, this, &SocketConnection::sendData);
    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    fSocket->addCaCertificate(cert);
    fSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    fTimer->start(1000);
}

void SocketConnection::sendData(const QByteArray &d)
{
    fSocket->write(d);
    fSocket->flush();
}

quint32 SocketConnection::getTcpPacketNumber()
{
    QMutexLocker ml(&fMutex);
    return ++fTcpPacketNumber;
}

quint32 SocketConnection::getMessageId()
{
    QMutexLocker ml(&fMutex);
    return ++fMessageId;
}

void SocketConnection::initInstance()
{
    if (fInstance == nullptr) {
        fInstance = new SocketConnection();
    }
}

void SocketConnection::timeout()
{
    if (fSocket->state() == QSslSocket::ConnectedState) {
        fTimer->stop();
        return;
    } else {
        fSocket->connectToHostEncrypted(fHostIP, fHostPort);
    }
}

void SocketConnection::encrypted()
{
    fTimer->stop();
    fTcpPacketNumber = 0;
    RawMessage r(fSocket);
    r.setHeader(getTcpPacketNumber(), 0, MessageList::hello);
    r.putString(fSocket->fUuid);
    emit dataReady(r.data());
    r.clear();
    r.setHeader(getTcpPacketNumber(), 0, MessageList::silent_auth);
    r.putString(fHostUsername);
    r.putString(fHostPassword);
    emit dataReady(r.data());
    emit connected();
}

void SocketConnection::disconnected()
{
    emit connectionLost();
    fTimer->start(3000);
}

void SocketConnection::readyRead()
{
    QByteArray ba;
    ba.append(0x03);
    ba.append(0x04);
    ba.append(0x15);

    if (fData.isEmpty()) {
        fData = fSocket->read(3);
        if (memcmp(fData.data(), ba.data(), 3) != 0) {
            fSocket->disconnectFromHost();
        }
    }
    fData.append(fSocket->readAll());
    int headersize = 3 + sizeof(fMessageNumber) + sizeof(fMessageId) + sizeof(fMessageCommand) + sizeof(fMessageSize);
    while (fData.length() >= headersize) {
        int pos = 3;
        memcpy(&fMessageNumber, &fData.data()[pos], sizeof(fMessageNumber));
        pos += sizeof(fMessageNumber);
        memcpy(&fMessageId, &fData.data()[pos], sizeof(fMessageId));
        pos += sizeof(fMessageId);
        memcpy(&fMessageCommand, &fData.data()[pos], sizeof(fMessageCommand));
        pos += sizeof(fMessageCommand);
        memcpy(&fMessageSize, &fData.data()[pos], sizeof(fMessageSize));

        if (fData.length() - headersize >= fMessageSize) {
            emit externalDataReady(fMessageCommand, fMessageId, fData.mid(headersize, fMessageSize));
            fData.remove(0, headersize + fMessageSize);
        } else {
            break;
        }
    }
}

void SocketConnection::errorOccurred(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    fSocket->disconnectFromHost();
}
