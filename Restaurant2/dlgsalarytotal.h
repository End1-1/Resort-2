#ifndef DLGSALARYTOTAL_H
#define DLGSALARYTOTAL_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgSalaryTotal;
}

class DlgSalaryTotal : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgSalaryTotal(QWidget *parent = nullptr);
    ~DlgSalaryTotal();

private slots:
    void on_btnSave_clicked();

    void on_btnPrint_clicked();

    void on_btnClose_clicked();

private:
    Ui::DlgSalaryTotal *ui;
};

#endif // DLGSALARYTOTAL_H
