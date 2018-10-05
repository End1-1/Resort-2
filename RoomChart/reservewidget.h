#ifndef RESERVEWIDGET_H
#define RESERVEWIDGET_H

#include "cacheroom.h"
#include "chardefaults.h"
#include "basewidget.h"
#include "cachereservation.h"
#include <QDate>

class ReserveRect;
class WMainDesk;

namespace Ui {
class ReserveWidget;
}

class ReserveWidget : public BaseWidget
{
    Q_OBJECT
public:
    friend class WMainDesk;
    explicit ReserveWidget(ReserveRect *rr, QWidget *parent = 0);
    explicit ReserveWidget(CI_Reservation *r, QWidget *parent = 0);
    ~ReserveWidget();
    void createService();
    void removeService();
    CI_Reservation *fReservation;
protected:
    void resizeEvent(QResizeEvent *e);
private slots:
    void on_btnCancel_clicked();

private:
    Ui::ReserveWidget *ui;
    CI_Room *fRoom;
    QDate fDateStart;
    QDate fDateEnd;
    QString fId;
    void setSize();
signals:
    void editReserv(CI_Reservation *r);
};

#endif // RESERVEWIDGET_H
