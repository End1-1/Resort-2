#ifndef WRESERVATIONS_H
#define WRESERVATIONS_H

#include "wreportgrid.h"

class WReservations : public WReportGrid
{
    Q_OBJECT
public:
    WReservations(QWidget *parent = 0);
    virtual void processValues(int row, bool isNew);
protected:
    virtual void setupTab();
};

#endif // WRESERVATIONS_H
