#ifndef PPRINTVAUCHER_H
#define PPRINTVAUCHER_H

#include "base.h"

class PPrintVaucher : public Base
{
public:
    PPrintVaucher();
    static void printVaucher(const QString &id);
};

#endif // PPRINTVAUCHER_H
