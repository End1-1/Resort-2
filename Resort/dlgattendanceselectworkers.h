#ifndef DLGATTENDANCESELECTWORKERS_H
#define DLGATTENDANCESELECTWORKERS_H

#include "basedialog.h"

namespace Ui {
class DlgAttendanceSelectWorkers;
}

class DlgAttendanceSelectWorkers : public BaseDialog
{
    Q_OBJECT

public:
    explicit DlgAttendanceSelectWorkers(QWidget *parent = nullptr);
    ~DlgAttendanceSelectWorkers();
    void setListOfChecked(const QList<int> &idlist);

private slots:
    void on_btnSave_clicked();

    void on_chHideUnchecked_clicked(bool checked);

    void on_btnRefresh_clicked();

private:
    Ui::DlgAttendanceSelectWorkers *ui;
    QList<int> fListOfId;
    void getList();
};

#endif // DLGATTENDANCESELECTWORKERS_H
