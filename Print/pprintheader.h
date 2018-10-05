#ifndef PPRINTHEADER_H
#define PPRINTHEADER_H

#include "base.h"

class PPrintScene;

class PPrintHeader : public Base
{
public:
    PPrintHeader(PPrintScene *ps, const QString &headerText, int number);
};

#endif // PPRINTHEADER_H
