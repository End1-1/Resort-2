#ifndef DLGGROUPRESERVATIONFUCK_H
#define DLGGROUPRESERVATIONFUCK_H

#include "basewidget.h"
#include "dwselectorcardex.h"
#include "dwselectorguest.h"
#include "dwselectorcityledger.h"
#include "dwselectorcreditcard.h"
#include "dlgsimpleselectroom.h"

namespace Ui {
class DlgGroupReservationFuck;
}

class DlgGroupReservationFuck : public BaseWidget
{
    Q_OBJECT

public:
    explicit DlgGroupReservationFuck(QWidget *parent = 0);
    ~DlgGroupReservationFuck();
    void loadGroup(int id);
    virtual void setup();
private slots:
    void preCount(const QString &str);
    void cityLedger(CI_CityLedger *ci);
    void card(CI_CreditCard *ci);
    void removeRow();
    void editReserve();
    void editInvoice();
    void editAInvoice();
    void roomTextChanged(const QString &arg1);
    void openRoomSelectDialog(bool v);
    void singleGuestLineEdit(bool v);
    void singleGuest(CI_Guest *ci);
    void singleGuestFocusOut();
    void createMultiRoom(int tag);
    void roomPriceChanged(const QString &arg1);
    void on_deArrival_textChanged(const QString &arg1);

    void on_deDeparture_textChanged(const QString &arg1);

    void on_tblRoom_clicked(const QModelIndex &index);

    void on_btnAddGuest_clicked();

    void on_leGuest_returnPressed();

    void on_teIndRemarks_textChanged();

    void on_btnCreateGroup_clicked();

    void on_btnSave_clicked();

    void on_btnRemarksToAll_clicked();

    void on_btnRemarksToAll_2_clicked();

    void on_btnArrangeToAll_clicked();

    void on_btnPriceToAll_clicked();

    void on_btnClear_clicked();

    void on_btnDateAll_clicked();

    void on_btnArrangeToAll_2_clicked();

    void on_btnAllPayments_clicked();

    void on_cbModeOfPayment_currentIndexChanged(int index);

    void on_btnAllCL_clicked();

    void on_btnAllCard_clicked();

    void on_tblCat_cellDoubleClicked(int row, int column);

    void on_btnClearFilter_clicked();

    void on_btnPrint_clicked();

    void on_btnCancelGroup_clicked();

private:
    Ui::DlgGroupReservationFuck *ui;
    DWSelectorCardex *fDockCardex;
    DWSelectorGuest *fDockGuest;
    DWSelectorGuest *fDockSingleGuest;
    DWSelectorCityLedger *fDockCL;
    DWSelectorCreditCard *fDockCard;
    int fCurrRow;
    DlgSimpleSelectRoom *fDockRoom;
    void save();
    void countReserve();
    void countTotalReservation();
    void makeTrackControl(int row);
    void createRooms(const QString &cat, const QString &bed, int count, double price);
};

#endif // DLGGROUPRESERVATIONFUCK_H
