#include "pimage.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

PImage::PImage(const QString &fileName, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    fImage = new QImage(fileName);
}

PImage::~PImage()
{
    delete fImage;
}

void PImage::setRect(const QRectF &rect)
{
    fRect = rect;
    update();
}

QRectF PImage::boundingRect() const
{
    return fRect;
}

void PImage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    QRect r = option->rect;
    painter->drawImage(r, *fImage);
}
