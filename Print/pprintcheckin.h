#ifndef PPRINTCHECKIN_H
#define PPRINTCHECKIN_H

#include "base.h"

class PPrintCheckin : public QObject, public Base
{
    Q_OBJECT
public:
    PPrintCheckin();
    static void print(const QString &id);
};

#endif // PPRINTCHECKIN_H
