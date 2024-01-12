#ifndef RDESK_H
#define RDESK_H

#include "baseextendeddialog.h"
#include "dishestable.h"
#include "hall.h"
#include "user.h"
#include "defrest.h"
#include "baseorder.h"
#include "trackcontrol.h"
#include <QPainter>

namespace Ui {
class RDesk;
}

class RTools;
class DishItemDelegate;

class RDesk : public BaseExtendedDialog
{
    Q_OBJECT
    friend class RTools;
    friend class DishItemDelegate;

public:
    explicit RDesk(QWidget *parent = 0);
    ~RDesk();
    void prepareToShow();
    bool setup(TableStruct *t);
    void setStaff(User *user);
    TableStruct *loadHall(int hall);
    void showHideRemovedItems();
    void setOrderComment();
    void removeOrder();
    void removeOrderByNumber();
    void showTableOrders();
    void showMyTotal();
    void initialCash();
    void setComplexMode();
    void closeOrder(int state = ORDER_STATE_CLOSED);
    void printTotalShort();
    void printTotalToday();
    void printTotalYesterday();
    void printTotalAnyDay();
    void printReceiptByNumber();
    void printVoidReport();
    void complimentary();
    void openTools();
    void closeDay();
    void salary();
    void visitStat();
    void checkCardAmount();
    void cardStat();
    void saledItem();
    void checkCardRegistration();
    void employesOfDay();

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    void printCanceledOrder(int id);
    void startService();

private slots:
    void timeout();
    virtual void externalDataReceived(quint16 cmd, quint32 messageId, const QByteArray &data);
    void socketConnected();
    virtual void connectionLost();
    void onBtnQtyClicked();
    void on_btnExit_clicked();
    void on_btnLanguage_clicked();
    void on_btnMenu_clicked();
    void on_tblPart_clicked(const QModelIndex &index);
    void on_tblType_clicked(const QModelIndex &index);
    void on_tblDish_clicked(const QModelIndex &index);
    void on_btnTrash_clicked();
    void on_btnPayment_clicked();
    void on_btnPrint_clicked();
    void on_btnComment_clicked();
    void on_btnTools_clicked();
    void on_btnCheckout_clicked();
    void on_btnTypeUp_clicked();
    void on_btnTypeDown_clicked();
    void on_btnDishUp_clicked();
    void on_btnDishDown_clicked();
    void on_btnOrdDown_clicked();
    void on_btnOrdUp_clicked();
    void on_btnComplex_clicked();
    void on_tblTables_itemClicked(QTableWidgetItem *item);
    void on_btnPayment_2_clicked();
    void on_btnSetCar_clicked();
    void on_btnDiscount_clicked();
    void on_btnPackage_clicked();
    void on_btnHallWash_clicked();
    void on_btnHallCafe_clicked();
    void on_btnExit_2_clicked();
    void on_btnDiss50_clicked();
    void on_btnHallVIP_clicked();
    void on_btnShop_clicked();
    void on_btnSetRecoverFrom_clicked();

    void on_btnPrintMultipleFiscal_clicked();

    void on_pushButton_clicked();

private:
    Ui::RDesk *ui;
    static QMap<int, DishStruct*> fQuickDish;
    TrackControl *fTrackControl;
    TableStruct *fTable;
    HallStruct *fHall;
    int fTimerCounter;
    User *fStaff;
    bool fCanClose;
    bool fShowRemoved;
    int fCurrentHall;
    int fMenu;
    DishesTable fDishTable;
    int fCarId;
    QString fCarModel;
    QString fCarGovNum;
    int fCostumerId;
    QList<int> fMessages;
    void setBtnMenuText();
    void setupType(int partId);
    void setupDish(int typeId);
    void addDishToOrder(DishStruct* d, bool counttotal);
    void addDishToTable(OrderDishStruct *od, bool counttotal, bool checkservice);
    void updateDish(OrderDishStruct *od);
    double countTotal();
    void countDish(OrderDishStruct *d);
    bool setTable(TableStruct *t, bool nosmile);
    void checkOrderHeader(TableStruct *t);
    void clearOrder();
    void loadOrder(bool showwarning);
    void setOrderRowHidden(int row, OrderDishStruct *od);
    void printServiceCheck(const QString &prn, int side);
    void printRemovedDish(OrderDishStruct *od, double removed, int user);
    void printReceipt(bool printModePayment);
    void changeBtnState();
    void checkEmpty();
    void resetPrintQty();
    void updateDishQtyHistory(OrderDishStruct *od);
    void updateTableInfo();
    void manualdisc(double val, int costumer);
    void repaintTables();
    void setRecoverFrom(int from);

signals:
    void dataReady(const QByteArray &);
};

#endif // RDESK_H
