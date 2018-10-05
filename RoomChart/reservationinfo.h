#ifndef RESERVATIONINFO_H
#define RESERVATIONINFO_H

#include "cachereservation.h"
#include "chardefaults.h"
#include "preferences.h"
#include <QGraphicsItem>
#include <QTimer>

class TimerBlya;

class ReservationInfo : public QGraphicsItem
{
public:
    friend class TimerBlya;
    ReservationInfo(CI_Reservation *r, QGraphicsItem *parent = 0);
    ~ReservationInfo();
    QRectF boundingRect() const;
    CI_Reservation *fReservation;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    QRectF fRect;
    QRect fBtnRect;
    QPixmap fPixmap;
    TimerBlya *fTimer;
    void singleClick();
    void doubleClick();
    static Preferences fPref;
};

#endif // RESERVATIONINFO_H
