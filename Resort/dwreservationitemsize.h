#ifndef DWRESERVATIONITEMSIZE_H
#define DWRESERVATIONITEMSIZE_H

#include "base.h"
#include <QDockWidget>

namespace Ui {
class DWReservationItemSize;
}

class DWReservationItemSize : public QDockWidget, public Base
{
    Q_OBJECT
public:
    explicit DWReservationItemSize(QList<int> &values, QWidget *parent = 0);
    ~DWReservationItemSize();
private:
    Ui::DWReservationItemSize *ui;
    QList<int> &fValues;
private slots:
    void thisVisibilityChanged(bool v);
};

#endif // DWRESERVATIONITEMSIZE_H
