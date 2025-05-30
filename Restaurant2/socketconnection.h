#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include "sslsocket.h"
#include <QTimer>

class SocketConnection : public QObject
{
    Q_OBJECT
public:
    explicit SocketConnection(QObject *parent = nullptr);
    ~SocketConnection();
    quint32 getTcpPacketNumber();
    quint32 getMessageId();
    static void initInstance();
    static void startConnection(const QString &ip, int port, const QString &username, const QString &password);
    static void setConnectionParams(const QString &ip, int port, const QString &username, const QString &password);
    static SocketConnection *instance();

public slots:
    void run();
    void sendData(const QByteArray &d);

private:
    static SocketConnection *fInstance;
    static quint32 fTcpPacketNumber;
    static quint32 fMessageId;
    QString fHostIP;
    int fHostPort;
    QString fHostUsername;
    QString fHostPassword;
    QByteArray fData;
    quint32 fMessageNumber;
    qint32 fMessageSize;
    quint16 fMessageCommand;
    SslSocket *fSocket;
    QTimer *fTimer;

private slots:
    void timeout();
    void encrypted();
    void disconnected();
    void readyRead();
    void errorOccurred(QAbstractSocket::SocketError error);

signals:
    void stopped();
    void connected();
    void connectionLost();
    void dataReady(QByteArray &);
    void externalDataReady(quint16, quint32, QByteArray);
};

#endif // SOCKETCONNECTION_H
