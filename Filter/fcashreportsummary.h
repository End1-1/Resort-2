#ifndef FCASHREPORTSUMMARY_H
#define FCASHREPORTSUMMARY_H

#include "wfilterbase.h"

namespace Ui {
class FCashReportSummary;
}

class FCashReportSummary : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCashReportSummary(QWidget *parent = 0);
    ~FCashReportSummary();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
private slots:
    void on_btnBack_clicked();

    void on_btnForward_clicked();

private:
    Ui::FCashReportSummary *ui;
    QString fQuery;
};

#endif // FCASHREPORTSUMMARY_H
