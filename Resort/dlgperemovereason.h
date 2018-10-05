#ifndef DLGPEREMOVEREASON_H
#define DLGPEREMOVEREASON_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgPERemoveReason;
}

class DlgPERemoveReason : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPERemoveReason(QWidget *parent = 0);
    ~DlgPERemoveReason();

private slots:
    void on_btnCancel_clicked();

    void on_btnVoid_clicked();

    void on_btnMistake_clicked();

private:
    Ui::DlgPERemoveReason *ui;
};

#endif // DLGPEREMOVEREASON_H
