#ifndef HALL_H
#define HALL_H

#include "base.h"
#include <QObject>
#include <QDateTime>

typedef struct {
    int fId;
    int fDefaultMenu;
    float fDefaultSvcValue;
    QString fName;
    int fItemIdForInvoice;
    QString fReceiptPrinter;
    QString fVatDept;
    QString fNoVatDept;
    int fServiceItem;
    QString fServiceName;
    double fServiceValue;
} HallStruct;
Q_DECLARE_METATYPE(HallStruct*)

typedef struct {
    int fId;
    int fHall;
    QString fName;
    QDateTime fLocked;
    QDateTime fOpened;
    QString fLockStation;
    QString fAmount;
    QString fDateOpen;
    QString fStaff;
    QString fComment;
    int fOrder;
    int fPrint;
    int fTaxPrint;
    int fPaymentMode;
    int fCitiLedger;
    QString fPaymentComment;
    QString fRoomComment;
    QString fCar;
    QString fGovNumber;
} TableStruct;
Q_DECLARE_METATYPE(TableStruct*)

class Hall : public QObject, Base
{
    Q_OBJECT
public:
    explicit Hall(QObject *parent = 0);
    void init();
    void refresh();
    static QList<HallStruct*> fHallTable;
    static QMap<int, HallStruct*> fHallMap;
    static QList<HallStruct*> fBanketHall;
    static QList<TableStruct*> fTables;
    static QMap<int, TableStruct*> fTablesMap;
    static HallStruct *getHallById(int id);
    static TableStruct *getTableById(int id);
    static void filterTables(int hallId, int busy,  QList<TableStruct*> &tables);
signals:

public slots:
};

#endif // HALL_H
