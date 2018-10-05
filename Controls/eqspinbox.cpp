#include "eqspinbox.h"
#include "trackcontrol.h"
#include <QLineEdit>

EQSpinBox::EQSpinBox(QWidget *parent) :
    QSpinBox(parent)
{

}

void EQSpinBox::setBgColor(const QColor &color)
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

QString EQSpinBox::getField()
{
    return fField;
}

void EQSpinBox::setField(const QString &field)
{
    fField = field;
}
