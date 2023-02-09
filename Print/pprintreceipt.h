#ifndef PPRINTRECEIPT_H
#define PPRINTRECEIPT_H

#include "base.h"

class PPrintReceipt : public Base
{
public:
    PPrintReceipt(const QString &printerName, int number, int user);
    static void printOrder(const QString &printerName, int number, int user);
};

#endif // PPRINTRECEIPT_H
