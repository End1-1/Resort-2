#ifndef DLGPRINTRESERVATION_H
#define DLGPRINTRESERVATION_H

#include "baseextendeddialog.h"
#include "wreservationroomtab.h"

namespace Ui {
class DlgPrintReservation;
}

class DlgPrintReservation : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPrintReservation(WReservationRoomTab *r);
    ~DlgPrintReservation();

private slots:
    void on_btnClose_clicked();

    void on_btnPrintConfirmation_clicked();

    void on_btnPrintReservation_clicked();

    void on_btnPrintRegistrationCard_clicked();

private:
    Ui::DlgPrintReservation *ui;
    WReservationRoomTab *fSource;
};

#endif // DLGPRINTRESERVATION_H
