#ifndef COMMAND_H
#define COMMAND_H

#include <QTcpSocket>

class Command : public QObject {
    Q_OBJECT
public:
    Command(QTcpSocket *socket);
    QString fDatabase;
    QString fUsername;
    QTcpSocket *fSocket;
    bool identify(const QString &database);
    bool isIdentified();
    void setSocket(QTcpSocket *socket);
    void readBytes(const QByteArray &data);
private:
    bool fIdentified;
    int fSize;
    QString fCommand;
signals:
    void parseCommand(const QString &command);
};

#endif // COMMAND_H
