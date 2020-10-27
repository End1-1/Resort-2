#ifndef STOREOUTPUT_H
#define STOREOUTPUT_H

#include "database.h"

class StoreOutput
{
public:
    StoreOutput(Database &db, int doc);
    void rollbackSale(Database &db, int order);
    void rollback(Database &db, int doc);
    void output(QMap<int, double> &priceList);
    
private:
    int fDoc;
    Database &fDb;
};

#endif // STOREOUTPUT_H
