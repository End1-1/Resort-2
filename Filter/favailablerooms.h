#ifndef FAVAILABLEROOMS_H
#define FAVAILABLEROOMS_H

#include "wfilterbase.h"

namespace Ui {
class FAvailableRooms;
}

class FAvailableRooms : public WFilterBase
{
    Q_OBJECT

public:
    explicit FAvailableRooms(QWidget *parent = 0);
    ~FAvailableRooms();
    virtual QString reportTitle();
    virtual QWidget *firstElement();
    virtual void apply(WReportGrid *rg);
private:
    Ui::FAvailableRooms *ui;
};

#endif // FAVAILABLEROOMS_H
