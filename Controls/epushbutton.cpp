#include "epushbutton.h"

EPushButton::EPushButton(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(clicked()));
    fTag = 0;
}

void EPushButton::setTag(int tag)
{
    fTag = tag;
}

void EPushButton::clicked()
{
    emit clickedWithTag(fTag);
    if (!fId.isEmpty()) {
        emit clickedWithId(fId);
    }
}
