#include "eqtimeedit.h"
#include "trackcontrol.h"
#include "defines.h"
#include <QLineEdit>

EQTimeEdit::EQTimeEdit(QWidget *parent) :
    QTimeEdit(parent)
{
    setDisplayFormat("HH:mm");
    setTime(QTime::fromString("00:00:00", "HH:mm:ss"));
}

QString EQTimeEdit::getField()
{
    return fField;
}

void EQTimeEdit::setField(const QString &field)
{
    fField = field;
}

void EQTimeEdit::setBgColor(const QColor &color)
{
    QListIterator<QObject*> it(children());
    QLineEdit *l;
    while (it.hasNext()) {
        l = static_cast<QLineEdit*>(it.next());
        if (isLineEdit(l)) {
            QPalette p = l->palette();
            p.setColor(QPalette::Base, color);
            l->setPalette(p);
            return;
        }
    }
}

QString EQTimeEdit::toString()
{
    return time().toString(def_time_format);
}
