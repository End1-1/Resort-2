#ifndef DLGSALARY_H
#define DLGSALARY_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgSalary;
}

class DlgSalary : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgSalary(bool day, QWidget *parent = 0);
    ~DlgSalary();
    static void salary();
    static void salary2();
    void loadSalary();
    void loadSalary2();

private slots:
    void on_btnClose_clicked();
    void on_btnSave_clicked();
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_btnAmount_clicked();
    void on_deDate_dateChanged(const QDate &date);
    void on_btnCopyFromReal_clicked();

private:
    Ui::DlgSalary *ui;
    bool fDay;
    void countSalary();
};

#endif // DLGSALARY_H
