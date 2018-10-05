#include "eqcheckbox.h"
#include <QKeyEvent>

EQCheckBox::EQCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    fRequireLang = false;
}

QString EQCheckBox::getField()
{
    return fField;
}

void EQCheckBox::setField(const QString &name)
{
    fField = name;
}

int EQCheckBox::getTag()
{
    return fTag;
}

void EQCheckBox::setTag(int tag)
{
    fTag = tag;
}

void EQCheckBox::setStyle(const QString &style)
{
    setStyleSheet(style);
}

bool EQCheckBox::getRequireLang()
{
    return fRequireLang;
}

void EQCheckBox::setRequireLang(bool value)
{
    fRequireLang = value;
}
