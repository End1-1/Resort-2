#include "dayitemdelegate.h"
#include "chardefaults.h"
#include <QPainter>
#include <QDate>
#include <QApplication>

static QMap<int, QString> DAYS_OF_WEEK;

DayItemDelegate::DayItemDelegate()
{
    if (DAYS_OF_WEEK.count() == 0) {
        DAYS_OF_WEEK[1] = "Mo";
        DAYS_OF_WEEK[2] = "Tu";
        DAYS_OF_WEEK[3] = "We";
        DAYS_OF_WEEK[4] = "Th";
        DAYS_OF_WEEK[5] = "Fr";
        DAYS_OF_WEEK[6] = "Sa";
        DAYS_OF_WEEK[7] = "Su";
    }
}

void DayItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected) {
        QRect selRect = option.rect;
        painter->fillRect(selRect, VSELECTION_COLOR);
    } else {
        painter->fillRect(option.rect, Qt::white);
    }
    if (!index.isValid()) {
        return;
    }
    painter->setPen(LIGHT_GRAY);
    painter->drawRect(option.rect);
    QDate date = index.data(Qt::UserRole).toDate();
    if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) {
        painter->setPen(Qt::red);
    } else {
        painter->setPen(LIGHT_GRAY);
    }
    QTextOption to;
    to.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont f(qApp->font().family(), 8);
    f.setBold(true);
    painter->setFont(f);
    QRect dayRect = option.rect;
    dayRect.adjust(0, 0, 0, (option.rect.height() / 2) * -1);
    QString day = QString::number(date.day());
    painter->drawText(dayRect, day, to);

    QRect dayOfWeekRect = option.rect;
    dayOfWeekRect.adjust(0, option.rect.height() / 2, 0, 0);
    QString dayOfWeek = DAYS_OF_WEEK[date.dayOfWeek()];
    painter->drawText(dayOfWeekRect, dayOfWeek, to);
}
