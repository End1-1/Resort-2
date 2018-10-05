#ifndef MONTHITEMDELEGATE_H
#define MONTHITEMDELEGATE_H

#include "chardefaults.h"
#include <QItemDelegate>

class MonthItemDelegate : public QItemDelegate
{
public:
    MonthItemDelegate();
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // MONTHITEMDELEGATE_H
