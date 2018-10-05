#include "reserverect.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ReserveRect::ReserveRect(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    fRect = QRectF(1, 1, COLUMN_WIDTH - 2, ROW_HEIGHT - 2);
}

ReserveRect::ReserveRect(const QPointF &p, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    fInitialPoint = p;
    fRect = QRectF(p.x() + 1, p.y() + 1, COLUMN_WIDTH - 2, ROW_HEIGHT - 2);
}

ReserveRect::~ReserveRect()
{

}

void ReserveRect::changeRect(const QPointF &p)
{
    if (p.x() < fInitialPoint.x()) {
        fRect.setX(p.x() + 1);
    } else if (p.x() > fInitialPoint.x()) {
        fRect.setWidth(p.x() - fInitialPoint.x() - 2 + COLUMN_WIDTH);
    } else {
        fRect.setWidth(COLUMN_WIDTH - 2);
    }
    fDateStart = dateFromX(fRect.left());
    fDateEnd = dateFromX(fRect.left() + fRect.width());
    update();
}

void ReserveRect::setRoom(CI_Room *room)
{
    fRoom = room;
    update();
}

int ReserveRect::width()
{
    return fRect.width();
}

QPointF ReserveRect::pos() const
{
    return QPointF(fRect.left(), fRect.top());
}

QRectF ReserveRect::boundingRect() const
{
    return fRect;
}

void ReserveRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    QColor color = QColor::fromRgb(204, 204, 255);
    painter->setPen(LIGHT_GRAY);
    painter->fillRect(option->rect, color);
    painter->drawRect(option->rect);
    QFont f("Times", 6);
    QString text = "#" + fRoom->fCode;
    if (option->rect.width() > COLUMN_WIDTH * 2) {

        text += QString("  %1 - %2")
                .arg(fDateStart.toString("dd,ddd"))
                .arg(fDateEnd.toString("dd,ddd"));
    }
    if (option->rect.width() > COLUMN_WIDTH * 4) {
        f.setPointSize(8);
    }
    painter->setFont(f);
    QTextOption to;
    to.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    painter->drawText(option->rect, text, to);
}
