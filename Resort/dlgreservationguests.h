#ifndef DLGRESERVATIONGUESTS_H
#define DLGRESERVATIONGUESTS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgReservationGuests;
}

class DlgReservationGuests : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgReservationGuests(QWidget *parent = 0);
    ~DlgReservationGuests();
    static void viewGuests(const QString &id);
private:
    Ui::DlgReservationGuests *ui;
    void load(const QString &id);
};

#endif // DLGRESERVATIONGUESTS_H
