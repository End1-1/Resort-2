#ifndef BROADCASTTHREAD_H
#define BROADCASTTHREAD_H

#include "base.h"
#include <QThread>
#include <QTcpSocket>

#define cmd_end_of_day 1
#define cmd_update_cache 2
#define cmd_global_settings 3
#define cmd_update_program 4

class BroadcastThread : public QThread, public Base
{
    Q_OBJECT
public:
    BroadcastThread(QObject *parent = 0);
    ~BroadcastThread();
    static void cmdRefreshCache(int cache, const QString &item);
    static void cmdCommand(int command, const QMap<QString, QString> &params);
protected:
    virtual void run();
private:
    QTcpSocket *fSocket;
    QByteArray fData;
    static void sendData(const QString &data);
private slots:
    void socketError(QAbstractSocket::SocketError err);
};

#endif // BROADCASTTHREAD_H
