#ifndef WMAINDESK_H
#define WMAINDESK_H

#include "basewidget.h"
#include "roomscene.h"
#include "dwmaindeskhint.h"
#include "cacheroom.h"
#include "cachereservation.h"
#include "dwselectorreservationcardex.h"
#include <QItemDelegate>
#include <QPainter>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QTimer>

namespace Ui {
class WMainDesk;
}

class WMainDesk : public BaseWidget
{
    Q_OBJECT

public:
    explicit WMainDesk(QWidget *parent = 0);
    ~WMainDesk();
    void loadReservationList();
    void filterRoom();
    virtual void setup();
    virtual void setupTab();
    virtual void handleBroadcast(const QMap<QString, QVariant> &data);
    virtual void selector(int selectorNumber, const QVariant &value);
protected:
    virtual bool event(QEvent *e);
private slots:
    void timeout();
    void scDateHScroll(int value);
    void scMainHScroll(int value);
    void scRoomVScroll(int value);
    void scMainVScroll(int value);
    void roomSelectionChanged(const QModelIndex &m1, const QModelIndex &m2);
    void editReserveRoom(CI_Reservation *r);
    void daySelectionChanged(const QModelIndex &m1, const QModelIndex &m2);
    void reservationCacheUpdated(const QString &id);
    void roomCacheUpdated(const QString &id);
    void btnDockHintGoToClicked(int tag);
    void dockHintVisibilityChanged(bool v);
    void changeDate();
    void on_tblClasses_clicked(const QModelIndex &index);
    void on_tblBed_clicked(const QModelIndex &index);
    void on_btnClearFilter_clicked();
    void on_btnScrollRight_clicked();
    void on_btnScrollLeft_clicked();
    void on_btnCheckIn_clicked();
    void on_btnClearSelection_clicked();
    void on_btnGroupReservation_clicked();
    void on_btnScrollDown_clicked();
    void on_btnScrollUp_clicked();
    void on_btnEndOfDay_clicked();
    void on_btnCheckin_clicked();
    void on_btnShowDockHint_clicked();
    void on_tblSmoke_clicked(const QModelIndex &index);
    void on_btnJumpToDate_clicked();
    void on_tblRoom_clicked(const QModelIndex &index);
    void on_tblDay_clicked(const QModelIndex &index);
    void on_btnCheckoutFilter_clicked();
    void on_leJumpToRoom_editingFinished();

    void on_tblRoom_doubleClicked(const QModelIndex &index);

    void on_btnInvoice_clicked();

    void on_btnPostCharge_clicked();

    void on_btnReceipt_clicked();

    void on_btnTransferRoom_clicked();

    void on_btnFilterCardex_clicked();

    void on_btnAirReserve_clicked();

    void on_leFilterGuest_textEdited(const QString &arg1);

    void on_btnStatus_clicked();

private:
    Ui::WMainDesk *ui;
    QTimer fTimer;
    QDate fDateStart;
    QDate fDateEnd;
    DWMainDeskHint *fDockHint;
    DWSelectorReservationCardex *fDockCardex;
    RoomScene *fScene;
    int fReserveStatus;
    QString fFilterCardex;
    QList<CI_Room*> fRoomList;
    QSet<int> fRoomIdList;
    QList<QList<QVariant> > fReservationHint;
    void dockHint(const QString &filter);
    int xFromDate(const QDate &date);
    void roomsRowSelectionChanged();
    void daysColumnSelectionChanged();
};

#endif // WMAINDESK_H
