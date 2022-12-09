#ifndef DLGCONFIGATTENDANCE_H
#define DLGCONFIGATTENDANCE_H

#include <QDialog>

namespace Ui {
class DlgConfigAttendance;
}

class DlgConfigAttendance : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfigAttendance(QWidget *parent = nullptr);
    ~DlgConfigAttendance();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgConfigAttendance *ui;
};

#endif // DLGCONFIGATTENDANCE_H
