#ifndef DAYITEMDELEGATE_H
#define DAYITEMDELEGATE_H

#include <QItemDelegate>

class DayItemDelegate : public QItemDelegate
{
public:
    DayItemDelegate();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DAYITEMDELEGATE_H
