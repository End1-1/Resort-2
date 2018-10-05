#include "eqdateedit.h"
#include "defines.h"
#include <QLineEdit>

EQDateEdit::EQDateEdit(QWidget *parent) :
    QDateEdit(parent)
{
    setDisplayFormat(def_date_format);
    setCalendarPopup(true);
    //setDate(def_date);
    setDate(QDate::currentDate());
}

QString EQDateEdit::getField()
{
    return fField;
}

void EQDateEdit::setField(const QString &field)
{
    fField = field;
}

void EQDateEdit::setBgColor(const QColor &color)
{
    QListIterator<QObject*> it(children());
    while (it.hasNext()) {
        QObject *o = it.next();
        QLineEdit *l = dynamic_cast<QLineEdit*>(o);
        if (l) {
            QPalette p = l->palette();
            p.setColor(QPalette::Base, color);
            l->setPalette(p);
        }
    }
}

QString EQDateEdit::toString()
{
    return date().toString(def_date_format);
}
