#ifndef BASEORDER_H
#define BASEORDER_H

#include "database.h"

class BaseOrder
{
public:
    BaseOrder(int id);
    void calculateOutput(Database &db);
    void calculateOutput(Database &db, int id);
private:
    int fId;
};

#endif // BASEORDER_H
