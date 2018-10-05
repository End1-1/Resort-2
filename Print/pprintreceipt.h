#ifndef PPRINTRECEIPT_H
#define PPRINTRECEIPT_H

#include "base.h"

class PPrintReceipt : public Base
{
public:
    PPrintReceipt(const QString &printerName, const QString &number, int user);
    static void printOrder(const QString &printerName, const QString &number, int user);
};

#endif // PPRINTRECEIPT_H
