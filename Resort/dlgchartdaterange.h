#ifndef DLGCHARTDATERANGE_H
#define DLGCHARTDATERANGE_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgChartDateRange;
}

class DlgChartDateRange : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgChartDateRange(QWidget *parent = 0);
    ~DlgChartDateRange();
    void getResult(QDate &date);
    static bool getDate(QDate &date);
private:
    Ui::DlgChartDateRange *ui;
};

#endif // DLGCHARTDATERANGE_H
