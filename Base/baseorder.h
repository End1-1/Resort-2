#ifndef BASEORDER_H
#define BASEORDER_H

#include "base.h"

class BaseOrder : public Base
{
public:
    BaseOrder(const QString &id);
    void calculateOutput();
    void calculateOutput(const QString &id);
private:
    QString fId;
};

#endif // BASEORDER_H
