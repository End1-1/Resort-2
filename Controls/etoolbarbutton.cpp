#include "etoolbarbutton.h"

EToolbarButton::EToolbarButton(QWidget *parent) :
    QToolButton(parent)
{

}

QString EToolbarButton::getTag()
{
    return fTag;
}

void EToolbarButton::setTag(const QString &tag)
{
    fTag = tag;
}
