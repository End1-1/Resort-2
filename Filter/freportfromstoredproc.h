#ifndef FREPORTFROMSTOREDPROC_H
#define FREPORTFROMSTOREDPROC_H

#include "basewidget.h"
#include "database2.h"

namespace Ui {
class FReportFromStoredProc;
}

class FReportFromStoredProc : public BaseWidget
{
    Q_OBJECT

public:
    explicit FReportFromStoredProc(QWidget *parent = nullptr);
    ~FReportFromStoredProc();
    QString fTitle;
    EQTableWidget *tbl(Database2 &db2);

private:
    Ui::FReportFromStoredProc *ui;
    virtual void setupTab() override;
};

#endif // FREPORTFROMSTOREDPROC_H
