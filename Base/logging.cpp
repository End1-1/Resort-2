#include "logging.h"

#include <QFile>
#include <QDateTime>
#include <QDir>

bool logEnabled = false;

logging::logging()
{

}

void logging::writeLog(const QString &text)
{
    if (!logEnabled) {
        return;
    }
    QDir dir;
    QString logFile = QString("%1/%2/log.txt").arg(dir.homePath()).arg("HotelResort");
    QFile f(logFile);
    if (f.open(QIODevice::Append)) {
        QString fullText = QString("%1: %2\r\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")).arg(text);
        f.write(fullText.toUtf8());
        f.close();
    }
}
