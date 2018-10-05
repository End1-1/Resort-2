#ifndef REPORTPRINT_H
#define REPORTPRINT_H

#include "base.h"

class ReportPrint : public QObject, public Base
{
    Q_OBJECT
public:
    ReportPrint();
    static void printTotal(const QDate &date, const QString &printedBy, const QString &prn);
};

#endif // REPORTPRINT_H
