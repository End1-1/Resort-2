#ifndef WREMARKS_H
#define WREMARKS_H

#include "wreportgrid.h"

class WRemarks : public WReportGrid
{
    Q_OBJECT
public:
    WRemarks(QWidget *parent = 0);
    virtual void setup();
};

#endif // WREMARKS_H
