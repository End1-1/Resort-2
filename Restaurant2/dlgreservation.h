#ifndef DLGRESERVATION_H
#define DLGRESERVATION_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgReservation;
}

class DlgReservation : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReservation(QWidget *parent = 0);
    ~DlgReservation();
    void loadCars();
    void loadEmployes();
    int fItemCode;
    QString fReservationId;
private:
    Ui::DlgReservation *ui;
    int fMode;
private slots:
    void kdbText(const QString &text);
    void kbdAccept();
    void on_leSearch_textChanged(const QString &arg1);
    void on_btnClear_clicked();
    void on_tblData_clicked(const QModelIndex &index);
    void on_tblItem_clicked(const QModelIndex &index);
};

#endif // DLGRESERVATION_H
