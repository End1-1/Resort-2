#ifndef DLGCREATEGROUPRESERVATION_H
#define DLGCREATEGROUPRESERVATION_H

#include "baseextendeddialog.h"
#include "dwselectorguest.h"
#include "dwselectorcardex.h"

namespace Ui {
class DlgCreateGroupReservation;
}

class WReservationRoomTab;

class DlgCreateGroupReservation : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgCreateGroupReservation(QWidget *parent = 0);
    ~DlgCreateGroupReservation();
    void loadRooms();
    void setSingleMode(bool mode);
    void setReservationTab(WReservationRoomTab *t);
private slots:
    void groupPriceChanged(const QString &arg1);
    void singleHandle(bool v);
    void guest(CI_Guest *c);
    void cardex(CI_Cardex *c);
    void on_btnCreate_clicked();
    void on_btnAddGuest_clicked();
    void on_btnClearRoomFilter_clicked();
    void on_tblCategory_clicked(const QModelIndex &index);
    void on_tblBed_clicked(const QModelIndex &index);
    void on_tblFloor_clicked(const QModelIndex &index);
    void on_tblSmoke_clicked(const QModelIndex &index);

    void on_deArrival_textEdited(const QString &arg1);

    void on_deDeparture_textEdited(const QString &arg1);

    void on_leGuest_returnPressed();

private:
    Ui::DlgCreateGroupReservation *ui;
    bool fSingleMode;
    WReservationRoomTab *fTab;
    void makeRooms();
    DWSelectorGuest *fDockGuest;
    DWSelectorCardex *fDockCardex;
    QMap<QString, QString> fPrice;
    void countSelected();
};

#endif // DLGCREATEGROUPRESERVATION_H
