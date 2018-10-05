#ifndef WRESERVATIONROOMTAB_H
#define WRESERVATIONROOMTAB_H

#include "basewidget.h"
#include "dwreservationitemsize.h"
#include "dwselectorcardex.h"
#include "cacheroom.h"
#include "dwselectorcityledger.h"
#include "dwselectorreservestatus.h"
#include "cacheguest.h"
#include "dwselectorguest.h"

#define cache_reserv_state 2
#define cache_room 4
#define cache_reminder_state 6
#define cache_sex 7
#define cache_nationality 8
#define cache_room_class 9
#define cache_room_bed 10
#define cache_payment_type 11
#define cache_vat_mode 12
#define cache_room_view 13
#define cache_room_arrangement 14
#define cache_reserv_status 15
#define cache_city_ledger 16

class WReservation;

namespace Ui {
class WReservationRoomTab;
}

class WReservationRoomTab : public BaseWidget
{
    Q_OBJECT
public:
    explicit WReservationRoomTab(QWidget *parent = 0);
    ~WReservationRoomTab();
    static bool check(Database &db, int room, const QDate &start, const QDate &end, const QString &rid, QString &errorMsg);
    void setBaseData(const QDate &date1, const QDate &date2, CI_Room *room);
    void setTimes(const QTime &arrival, const QTime &departure);
    void setCardex(CI_Cardex *c);
    void setArrangement(int id);
    void setRoomPrice(const QString &text);
    void setRemarks(const QString &remarks);
    bool save();
    void print();
    QString roomName();
    QString roomCode();
    QDate date1();
    QDate date2();
    QString docId();
    int firstGuestId();
    QString mainGuest();
    bool checkDoc(QStringList &errors);
    void setRoom(CI_Room *r);
    void disconnectSignals();
    void connectSignals();
    void loadReservation(const QString &id);
    void setGuest(int id, bool removeFirst = false);
    bool canRecheckin();
    void reCheckin();
    bool canCheckIn(QString &why);
    bool checkIn(QString &errorString);
    bool canCancel();
    bool cancelReservation(bool confirm = true);
    bool canCopyLast();
    bool canRevive();
    bool revive();
    void copyLast(const QString &lastId = "");
    QString author();
    QString lastModify();
    int reserveState();
    QString reserveId();
    void startTrackControl();
    TrackControl *trackControl();
    WReservation *fReservation;
    EQComboBox *cbRoomArrangement();
    void addRemoveFromGroup();
    void sendConfirmation();
protected:
    virtual void setupTab();
    virtual bool event(QEvent *event);
private slots:
    void roomCacheUpdated(const QString &id);
    void roomSearch(bool v);
    void tblGuestChange(int tag);
    void tblGuestRemove(int row);
    void tblGuestChangeInfo(int tag);
    void guest(CI_Guest *c);
    void room(CI_Room *c);
    void cardex(CI_Cardex *c);
    void cityLedger(CI_CityLedger *c);
    void reserveStatus(CI_ReserveStatus *c);
    void on_leRoomCode_returnPressed();
    void on_leMealPrice_textChanged(const QString &arg1);
    void on_leRooming_textChanged(const QString &arg1);
    void on_sbNights_valueChanged(int arg1);
    void on_deDeparture_dateChanged(const QDate &date);
    void on_deEntry_dateChanged(const QDate &date);
    void on_cbVAT_currentIndexChanged(int index);
    void on_leExtraBedAmount_textChanged(const QString &arg1);
    void on_leEarlyCheckInFee_textChanged(const QString &arg1);
    void on_chEarlyCheckIn_stateChanged(int arg1);
    void on_chExtraBed_stateChanged(int arg1);
    void on_sbWoman_valueChanged(int arg1);
    void on_sbChild_valueChanged(int arg1);
    void on_sbMan_valueChanged(int arg1);
    void on_chEarlyCheckIn_clicked(bool checked);
    void on_chExtraBed_clicked(bool checked);
    void on_btnNewGuest_clicked();
    void on_cbPaymentType_currentIndexChanged(int index);
    void on_btnItemSide_clicked();
    void on_leSearchGuest_returnPressed();
    void on_chMealIncluded_clicked(bool checked);
    void on_btnAppendAdvance_clicked();
    void on_chLateCheckout_clicked(bool checked);
    void on_sbMealQty_valueChanged(int arg1);
    void on_leMealPrice_textEdited(const QString &arg1);
    void on_deEntry_textEdited(const QString &arg1);
    void on_deDeparture_textEdited(const QString &arg1);
    void on_cbArrangment_currentIndexChanged(int index);

    void on_btnAllNation_clicked();

private:
    Ui::WReservationRoomTab *ui;
    int fReservRoomId;
    bool fStartDateOk;
    bool fCardexOk;
    bool fEndDateOk;
    bool fCityLedgerOk;
    QString fAuthor;
    QString fLastModify;
    DWSelectorReserveStatus *fDockReserveStatus;
    DWReservationItemSize *fDockItemSide;
    DWSelectorCardex *fDockCardex;
    DWSelectorCityLedger *fDockCityLedger;
    DWSelectorGuest *fDockGuest;
    QList<int> fItemSide;
    void addGuest(CI_Guest *g, bool log);
    void countTotal();
    void setEarlyCheckIn(bool v);
    void checkDatesCross();
    void setGroupBoxesEnabled(bool v);
    void saveVaucher();
    void getAdvance();
signals:
    void roomChanged(const QString &roomName, int tabIndex);
    void commonChanges();
};

#endif // WRESERVATIONROOMTAB_H
