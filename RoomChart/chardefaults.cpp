#include "chardefaults.h"

QDate ChartStartDate;

QDate dateFromX(int x)
{
    x -= (x % COLUMN_WIDTH);
    x /= COLUMN_WIDTH;
    return ChartStartDate.addDays(x);
}

int rowFromY(int y)
{
    y -= (y % ROW_HEIGHT);
    y /= ROW_HEIGHT;
    return y;
}

int xFromDate(const QDate &date)
{
    int days = ChartStartDate.daysTo(date);
    return days * COLUMN_WIDTH;
}
