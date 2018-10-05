#ifndef FRESERVAIONREMARKS_H
#define FRESERVAIONREMARKS_H

#include "wfilterbase.h"

namespace Ui {
class FReservaionRemarks;
}

class FReservaionRemarks : public WFilterBase
{
    Q_OBJECT

public:
    explicit FReservaionRemarks(QWidget *parent = 0);
    ~FReservaionRemarks();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    static void openReport();
private slots:
    void on_rbReserve_clicked();

    void on_rbCheckin_clicked();

private:
    Ui::FReservaionRemarks *ui;
};

#endif // FRESERVAIONREMARKS_H
