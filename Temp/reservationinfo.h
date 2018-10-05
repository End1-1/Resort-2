#ifndef RESERVATIONINFO_H
#define RESERVATIONINFO_H

#include "cachereservation.h"
#include "chardefaults.h"
#include "basewidget.h"
#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class ReservationInfo;
}

class ReservationInfo : public BaseWidget
{
    Q_OBJECT
public:
    explicit ReservationInfo(CI_Reservation *r, QWidget *parent = 0);
    ~ReservationInfo();
    CI_Reservation *fReservation;
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
private:
    Ui::ReservationInfo *ui;
private slots:
    void on_btnView_clicked();
};

#endif // RESERVATIONINFO_H
