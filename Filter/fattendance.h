#ifndef FATTENDANCE_H
#define FATTENDANCE_H

#include "wfilterbase.h"

namespace Ui {
class FAttendance;
}

class ReportQuery;

class FAttendance : public WFilterBase
{
    Q_OBJECT

public:
    explicit FAttendance(QWidget *parent = nullptr);
    ~FAttendance();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();

private:
    Ui::FAttendance *ui;
    ReportQuery *fReportQuery;

private slots:
    void branchDoubleClicked(bool v);
    void newWorker();
    void newWorkers();
    void removeWorker();
    void config();
};

#endif // FATTENDANCE_H
