#ifndef ROOMITEMDELEGATE_H
#define ROOMITEMDELEGATE_H

#include "chardefaults.h"
#include <QItemDelegate>

class RoomItemDelegate : public QItemDelegate
{
public:
    RoomItemDelegate();
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ROOMITEMDELEGATE_H
