#ifndef WROOMVIEW_H
#define WROOMVIEW_H

#include "wreportgrid.h"

class WRoomView : public WReportGrid
{
    Q_OBJECT
public:
    WRoomView(QWidget *parent = 0);
    virtual void setupTab();
};

#endif // WROOMVIEW_H
