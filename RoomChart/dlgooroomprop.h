#ifndef DLGOOROOMPROP_H
#define DLGOOROOMPROP_H

#include "baseextendeddialog.h"
#include "cachereservation.h"

namespace Ui {
class DlgOORoomProp;
}

class DlgOORoomProp : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgOORoomProp(CI_Reservation *reserve, QWidget *parent = 0);
    ~DlgOORoomProp();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_deEnd_textChanged(const QString &arg1);

    void on_deStart_textChanged(const QString &arg1);

    void on_btmRemove_clicked();

private:
    CI_Reservation *fReservation;
    Ui::DlgOORoomProp *ui;
    void checkOO();
};

#endif // DLGOOROOMPROP_H
