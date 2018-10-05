#include "monthitemdelegate.h"
#include <QPainter>
#include <QDate>
#include <QApplication>

MonthItemDelegate::MonthItemDelegate()
{

}

void MonthItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->fillRect(option.rect, Qt::white);
    painter->drawRect(option.rect);
    if (!index.isValid()) {
        return;
    }
    QDate date = index.data(Qt::UserRole).toDate();
    QString dateStr = date.toString("MMMM, yyyy");
    QFont f(qApp->font().family(), 12);
    painter->setFont(f);
    QTextOption to;
    to.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    painter->drawText(option.rect, dateStr, to);
}
