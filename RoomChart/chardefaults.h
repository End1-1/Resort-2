#ifndef CHARDEFAULTS_H
#define CHARDEFAULTS_H

#include <QColor>
#include <QDate>

#define COLUMN_WIDTH 25
#define ROW_HEIGHT 22


static const QColor ROOM_RECT_BACKGROUND[][2] = {
    {Qt::white, Qt::white},
    {QColor::fromRgb(70, 170, 255), QColor::fromRgb(120, 190, 255)},
    {QColor::fromRgb(255, 255, 0), QColor::fromRgb(255, 255, 150)},
    {QColor::fromRgb(88, 88, 88), QColor::fromRgb(170, 170, 170)}, // o/o
    {Qt::white, Qt::white},
    {Qt::white, Qt::white},
    {Qt::white, Qt::white}, // o/i
    {Qt::white, Qt::white},
    {Qt::white, Qt::white},
    {Qt::white, Qt::white}
};


static const QColor LIGHT_GRAY = QColor::fromRgb(72, 72, 72);
static const QColor VSELECTION_COLOR = QColor::fromRgb(0, 255, 0, 100);
static const QColor HSELECTION_COLOR = QColor::fromRgb(255, 0, 0, 200);
extern QDate ChartStartDate;

QDate dateFromX(int x);
int xFromDate(const QDate &date);
int rowFromY(int y);

#endif // CHARDEFAULTS_H
