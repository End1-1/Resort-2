#ifndef WREPORTROOM_H
#define WREPORTROOM_H

#include "wreportgrid.h"

class WReportRoom : public WReportGrid
{
    Q_OBJECT
public:
    explicit WReportRoom(QWidget *parent = 0);
    ~WReportRoom();
protected:
    virtual void setupTab();
private:
};

#endif // WREPORTROOM_H
