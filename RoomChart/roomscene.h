#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include "reserverect.h"
#include "chardefaults.h"
#include "reservewidget.h"
#include "cacheroom.h"
#include "reservationinfo.h"
#include "cachereservation.h"
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QSet>

class RoomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    RoomScene(QObject *parent = 0);
    ~RoomScene();
    void initBackgroung(int columnCount, const QList<CI_Room *> &rooms);
    void rowsSelect(const QSet<int> &rows);
    void columnsSelect(const QSet<int> &columns);
    void clearReserveWidgets();
    void getReserveWidgets(QList<ReserveWidget *> &w);
    void removeInvalidReserveWidget(CI_Reservation *r);
    ReservationInfo *addReserveInfo(CI_Reservation *r);
    ReserveWidget *addReserveWidget(CI_Reservation *r);
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
private:
    bool fStartReserveSelect;
    ReserveRect *fReserveRect;
    QGraphicsItemGroup fGridGroup;
    QGraphicsItemGroup fHSelGroup;
    QGraphicsItemGroup fVSelGroup;
    QGraphicsItemGroup fReserveGroup;
    QGraphicsItemGroup fReserveInfoGroup;
    QMap<int, QGraphicsRectItem*> fSelectedRooms;
    QMap<int, QGraphicsRectItem*> fSelectedColumns;
    QList<CI_Room *> fRooms;
    int yFromRoomCode(const QString &room);
    QPointF nearestPoint(const QPointF &p);
    void changeSelection(QGraphicsItemGroup &gr, QMap<int, QGraphicsRectItem*> &sel, const QSet<int> &values);
private slots:
    void editReserveRoom(CI_Reservation *r);
signals:
    void editReserve(CI_Reservation *r);
};

#endif // ROOMSCENE_H
