#ifndef BASEUID_H
#define BASEUID_H

#include "database.h"

extern int _IDGENMODE_;

#define uuid(x, y) BaseUID::genID(x, y)

class BaseUID
{
public:
    BaseUID();
    static QString genID(const QString &prefix, Database &db);
    static QString ID(const QString &vaucher, Database &db);
    static QString INTID(const QString &prefix, Database &db);
    static int fUserId;
};

#endif // BASEUID_H
