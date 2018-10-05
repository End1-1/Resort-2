#ifndef RDESK_H
#define RDESK_H

#include "baseextendeddialog.h"
#include "dishestable.h"
#include "hall.h"
#include "user.h"
#include "trackcontrol.h"
#include <QPainter>
#include <QTimer>

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
    void showHideRemovedItems();
    void setOrderComment();
    void moveTable();
    void removeOrder();
    void showTableOrders();
    void showMyTotal();
    void recover();
    void initialCash();
    void setComplexMode();
    void closeOrder(int state = ORDER_STATE_CLOSED);
    void printTotalToday();
    void printTotalYesterday();
    void printTotalAnyDay();
    int printTax(int cashMode);
    void printTaxDialog();
    void printReceiptByNumber();
    void voidBack();
    void printVoidReport();
    void complimentary();
    void changePassword();
    void openTools();
protected:
    virtual void closeEvent(QCloseEvent *e);
private slots:
    void onBtnQtyClicked();
    void timeout();
    void on_btnExit_clicked();
    void on_btnLanguage_clicked();
    void on_btnMenu_clicked();
    void on_tblPart_clicked(const QModelIndex &index);
    void on_tblType_clicked(const QModelIndex &index);
    void on_tblDish_clicked(const QModelIndex &index);
    void on_btnTrash_clicked();
    void on_btnPayment_clicked();
    void on_btnPrint_clicked();
    void on_btnTable_clicked();
    void on_btnComment_clicked();
    void on_btnTools_clicked();
    void on_btnCheckout_clicked();
    void on_btnTransfer_clicked();

    void on_btnTypeUp_clicked();

    void on_btnTypeDown_clicked();

    void on_btnDishUp_clicked();

    void on_btnDishDown_clicked();

    void on_btnOrdDown_clicked();

    void on_btnOrdUp_clicked();

    void on_btnSetRoom_clicked();

    void on_btnComplex_clicked();

    void on_pushButton_clicked();

    void on_btnGuests_clicked();

private:
    Ui::RDesk *ui;
    static QMap<int, DishStruct*> fQuickDish;
    TrackControl *fTrackControl;
    TableStruct *fTable;
    HallStruct *fHall;
    User *fStaff;
    bool fCanClose;
    bool fShowRemoved;
    int fMenu;
    int fCloseTimeout;
    DishesTable fDishTable;
    QTimer fTimer;
    int right(int right, int &trackUser);
    void setBtnMenuText();
    void setupType(int partId);
    void setupDish(int typeId);
    void addDishToOrder(DishStruct* d, bool dontCheckTable = false);
    void addDishToTable(OrderDishStruct *od);
    void updateDish(OrderDishStruct *od);
    double countTotal();
    void countDish(OrderDishStruct *d);
    bool setTable(TableStruct *t);
    void checkOrderHeader(TableStruct *t, DishStruct *od, bool skipService);
    void clearOrder();
    void loadOrder();
    void setOrderRowHidden(int row, OrderDishStruct *od);
    void printServiceCheck(const QString &prn, int side);
    void printRemovedDish(OrderDishStruct *od, double removed, int user);
    void printReceipt(bool printModePayment);
    void changeBtnState();
    void checkEmpty();
    void resetPrintQty();
    void updateDishQtyHistory(OrderDishStruct *od);
};

#endif // RDESK_H
