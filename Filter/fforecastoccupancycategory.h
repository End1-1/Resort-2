#ifndef FFORECASTOCCUPANCYCATEGORY_H
#define FFORECASTOCCUPANCYCATEGORY_H

#include "wfilterbase.h"

namespace Ui {
class FForecastOccupancyCategory;
}

class FForecastOccupancyCategory : public WFilterBase
{
    Q_OBJECT

public:
    explicit FForecastOccupancyCategory(QWidget *parent = 0);
    ~FForecastOccupancyCategory();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
private slots:
    void on_deStart_dateChanged(const QDate &date);

    void on_deEnd_dateChanged(const QDate &date);

private:
    Ui::FForecastOccupancyCategory *ui;
    void setupTab();
};

#endif // FFORECASTOCCUPANCYCATEGORY_H
