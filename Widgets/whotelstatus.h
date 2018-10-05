#ifndef WHOTELSTATUS_H
#define WHOTELSTATUS_H

#include "basewidget.h"

namespace Ui {
class WHotelStatus;
}

class WHotelStatus : public BaseWidget
{
    Q_OBJECT

public:
    explicit WHotelStatus(QWidget *parent = 0);
    ~WHotelStatus();
    virtual void setup();
private:
    Ui::WHotelStatus *ui;
};

#endif // WHOTELSTATUS_H
