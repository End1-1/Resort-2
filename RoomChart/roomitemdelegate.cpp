#include "roomitemdelegate.h"
#include "cacheroom.h"
#include <QPainter>

RoomItemDelegate::RoomItemDelegate()
{

}

void RoomItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) {
        QItemDelegate::paint(painter, option, index);
        return;
    }
    CI_Room *r = index.data(Qt::UserRole).value<CI_Room*>();
    /*
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, HSELECTION_COLOR);

    } else {
    */
    QLinearGradient bgFill(0, 0, 0, option.rect.height());
    bgFill.setColorAt(0, ROOM_RECT_BACKGROUND[r->fState][0]);
    bgFill.setColorAt(0.5, ROOM_RECT_BACKGROUND[r->fState][1]);
    bgFill.setColorAt(1, ROOM_RECT_BACKGROUND[r->fState][0]);
    QBrush brush(bgFill);
    painter->fillRect(option.rect, brush);

    QTextOption o;
    o.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    painter->setPen(LIGHT_GRAY);
    painter->drawRect(option.rect);

    painter->setPen(Qt::black);
    painter->drawText(option.rect, r->fName, o);
    painter->setPen(LIGHT_GRAY);
    QRect rect = option.rect;
    rect.adjust(-1, -1, -1, -1);
   // painter->drawText(rect, r->fName, o);
}
