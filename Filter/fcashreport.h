#ifndef FCASHREPORT_H
#define FCASHREPORT_H

#include "wfilterbase.h"

namespace Ui {
class FCashReport;
}

class FCashReport : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCashReport(QWidget *parent = 0);
    ~FCashReport();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    static void open();
    virtual QString reportTitle();
private slots:
    void on_toolButton_2_clicked();
    void on_toolButton_clicked();

private:
    Ui::FCashReport *ui;
    QString fHotelQuery;
    QString fRestaurantQuery;
};

#endif // FCASHREPORT_H
