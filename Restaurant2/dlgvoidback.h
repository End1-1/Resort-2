#ifndef DLGVOIDBACK_H
#define DLGVOIDBACK_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgVoidBack;
}

class DlgVoidBack : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgVoidBack(QWidget *parent = 0);
    ~DlgVoidBack();
    static void recover();
private slots:
    void on_btnCancel_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnRecover_clicked();

    void on_tblData_clicked(const QModelIndex &index);

private:
    Ui::DlgVoidBack *ui;
};

#endif // DLGVOIDBACK_H
