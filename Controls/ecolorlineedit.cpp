#include "ecolorlineedit.h"
#include <QIntValidator>

EColorLineEdit::EColorLineEdit(QWidget *parent) :
    EQLineEdit(parent)
{
    setReadOnly(true);
    setValidator(new QIntValidator());
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(colorChanged(QString)));
    setText("-1");
}

QString EColorLineEdit::getField()
{
    return fField;
}

void EColorLineEdit::setField(const QString &name)
{
    fField = name;
}

QColor EColorLineEdit::color()
{
    return QColor::fromRgb(text().toInt());
}

void EColorLineEdit::colorChanged(const QString &arg)
{
    if (arg.length() == 0) {
        setText("-1");
        return;
    }
    QPalette palette;
    QColor color = QColor::fromRgb(arg.toInt());
    palette.setColor(QPalette::Base, color);
    palette.setColor(QPalette::Text, color);
    setPalette(palette);
}
