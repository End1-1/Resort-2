#ifndef DLGRESERVATIONREMARKS_H
#define DLGRESERVATIONREMARKS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgReservationRemarks;
}

class DlgReservationRemarks : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReservationRemarks(QWidget *parent = 0);
    ~DlgReservationRemarks();
    static bool Remarks(QString &remark);
private slots:
    void on_tblData_clicked(const QModelIndex &index);

    void on_btnRemoveSelected_clicked();

    void on_btnSaveAs_clicked();

    void on_btnSaveCurrent_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_tblData_doubleClicked(const QModelIndex &index);

private:
    Ui::DlgReservationRemarks *ui;
};

#endif // DLGRESERVATIONREMARKS_H
