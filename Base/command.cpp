#include "command.h"

Command::Command(QTcpSocket *socket)
{
    fSocket = socket;
    fSize = 0;
    fCommand = "";
    fIdentified = false;
}

bool Command::identify(const QString &database)
{
    fDatabase = database;
    fIdentified = true;
    return fIdentified;
}

bool Command::isIdentified()
{
    return fIdentified;
}

void Command::setSocket(QTcpSocket *socket)
{
    fSocket = socket;
}

void Command::readBytes(const QByteArray &data)
{
    int pos = 0;
    int bytesLeft = data.length();
    do {
        int len;
        if (fSize == 0) {
            memcpy(&fSize, &data.data()[pos], sizeof(fSize));
            pos += sizeof(fSize);
            len = fSize;
            bytesLeft -= sizeof(fSize);
        } else {
            len = fSize - fCommand.toUtf8().length();
        }
        if (len > data.length() - pos) {
            len = data.length() - pos;
        }
        fCommand += data.mid(pos, len);
        pos += len;
        bytesLeft -= len;
        if (fCommand.length() == fSize) {
            emit parseCommand(fCommand);
            fCommand.clear();
            fSize = 0;
        }
    } while (bytesLeft > 0);
}
