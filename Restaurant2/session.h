#ifndef SESSION_H
#define SESSION_H

#include <QDate>
#include <QString>

class Session
{
public:
    static const int STATUS_OPEN = 1;
    static const int STATUS_CLOSED = 2;

    static int findOpen(int branch);
    static int open(int branch);
    static bool close(int sessionId);
    static int openTableCount(int branch);
    static int currentId();
    static void setCurrentId(int id);
    static QDate openWorkingDate(int sessionId);
    static bool isValidForWorkingDate(QString &error);

private:
    static int fCurrentId;
};

#endif // SESSION_H
