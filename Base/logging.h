#ifndef LOGGING_H
#define LOGGING_H

#define writelog(x) logging::writeLog(x)

#include <QString>

class logging
{
public:
    logging();
    static void writeLog(const QString &text);
};

extern bool logEnabled;

#endif // LOGGING_H
