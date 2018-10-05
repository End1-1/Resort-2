#include "eqtextedit.h"

EQTextEdit::EQTextEdit(QWidget *parent) :
    QTextEdit(parent)
{

}

QString EQTextEdit::getField()
{
    return fField;
}

void EQTextEdit::setField(const QString &field)
{
    fField = field;
}

void EQTextEdit::setBgColor(const QColor &color)
{
    QListIterator<QObject*> it(children());
    while (it.hasNext()) {
        QObject *o = it.next();
        if (!strcmp(o->metaObject()->className(), "QWidget")) {
            QWidget *w = static_cast<QWidget*>(o);
            QPalette p = w->palette();
            p.setColor(QPalette::Base, color);
            w->setPalette(p);
        }
    }
}

void EQTextEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTextEdit::mouseDoubleClickEvent(e);
    emit doubleClicked();
}
