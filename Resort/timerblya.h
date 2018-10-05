#ifndef TIMERBLYA_H
#define TIMERBLYA_H

#include "reservationinfo.h"
#include <QWidget>

class TimerBlya : public QWidget {
    Q_OBJECT
public:
    TimerBlya(ReservationInfo *r) : QWidget() {
        fReserve = r;
        fTimer.setInterval(400);
        fTimer.setSingleShot(true);
        connect(&fTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    }
    void start() {
        fDoubleClick = 0;
        fTimer.start();
    }
    void incDoubleClick() {
        if (fTimer.isActive()) {
            fDoubleClick ++;
        }
    }

private:
    ReservationInfo *fReserve;
    QTimer fTimer;
    int fDoubleClick;
private slots:
    void timeout() {
        if (fDoubleClick == 1) {
            fReserve->singleClick();
        } else if (fDoubleClick > 1) {
            fReserve->doubleClick();
        }
        fDoubleClick = 0;
    }
};

#endif // TIMERBLYA_H
