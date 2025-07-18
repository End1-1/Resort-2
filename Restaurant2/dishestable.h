#ifndef DISHESTABLE_H
#define DISHESTABLE_H

#include "base.h"
#include "splash.h"
#include <QObject>

typedef struct {
    int fId;
    QMap < QString, QString > fName;
} MenuStruct;
Q_DECLARE_METATYPE(MenuStruct*)

typedef struct {
    int fId;
    QMap < QString, QString > fName;
} DishPartStruct;
Q_DECLARE_METATYPE(DishPartStruct*)

typedef struct {
    int fId;
    int fPart;
    QMap < QString, QString > fName;
    int fBgColor;
    int fTextColor;
    int fQueue;
} TypeStruct;
Q_DECLARE_METATYPE(TypeStruct*)

typedef struct {
    int fId;
    int fMenu;
    int fPart;
    int fType;
    QString fName;
    int fBgColor;
    int fTextColor;
    QString fText;
    QList < QMap < QString, QString> > fMod;
    QString fPrint1;
    QString fPrint2;
    int fStore;
    float fPrice;
    float fSvcValue;
    int fQueue;
    int fComplex;
    int fNeedEmarks;
    QString fComplexRec;
    QString fAdgt;
    int fTax;
    QString barcode;
    QString tempEmark;
} DishStruct;
Q_DECLARE_METATYPE(DishStruct*)

typedef struct {
    int fRecId;
    int fState;
    int fCancelUser;
    QDateTime fCancelDate;
    int fDishId;
    QString fName;
    QString fPrint1;
    QString fPrint2;
    int fStore;
    float fQty;
    float fQtyPrint;
    float fPrice;
    float fSvcValue;
    float fSvcAmount;
    float fDctValue;
    float fDctAmount;
    float fTotal;
    QString fComment;
    int fStaff;
    int fComplex;
    QString fComplexRecId;
    QString fAdgt;
    int fTax;
    int fRow;
    QString fEmark;
} OrderDishStruct;
Q_DECLARE_METATYPE(OrderDishStruct*)

struct DishComplexStruct {
    int fRecId;
    int fId;
    QMap < QString, QString > fName;
    QTime fStart;
    QTime fEnd;
    double fQty;
    double fPrice;
    QString fAdgt;
    double fPriceDeviation;
    QList < DishStruct* > fDishes;
    DishComplexStruct& operator= (const DishComplexStruct &d)
    {
        if (this == &d) {
            return *this;
        }

        fRecId = d.fRecId;
        fId = d.fId;
        fName = d.fName;
        fStart = d.fStart;
        fEnd = d.fEnd;
        fQty = d.fQty;
        fPrice = d.fPrice;
        fAdgt = d.fAdgt;
        fPriceDeviation = d.fPriceDeviation;

        foreach (DishStruct *ds, d.fDishes) {
            DishStruct *dn = new DishStruct();
            *dn = *ds;
            fDishes << dn;
        }

        return *this;
    }
} ;
Q_DECLARE_METATYPE(DishComplexStruct*)

typedef struct {
    int fId;
    int fState;
    int fTable;
    int fStaffId;
    QString fStaffName;
    QDateTime fDateOpen;
    QString fComment;
    int fPrint;
} OrderHeaderStruct;
Q_DECLARE_METATYPE(OrderHeaderStruct*)

typedef struct {
    QMap < QString, QString > fName;
} DishMod;

class DishesTable : public QObject, Base
{
    Q_OBJECT
public:
    explicit DishesTable(QObject *parent = 0);
    void init(Splash *s = 0);
    void filterType(int menuId, int partId, QMap < int, TypeStruct* > &type);
    void filterDish(int menuId, int typeId, QMap < int, DishStruct* > &dish);
    static QList < QMap < QString, QString> > fMods;
    static QMap < int, MenuStruct* > fMenu;
    static QList < DishPartStruct* > fDishPart;
    static QMap < int, TypeStruct* > fType;
    static QMap < int, QSet < TypeStruct*> > fTypeMenu;
    static QMap < QString, TypeStruct* > fTypeProxy;
    static QList < DishStruct* > fDish;
    static QMap < int, QList < DishStruct*> > fDishMenu;
    static QList < DishComplexStruct* > fDishComplex;
    static DishStruct* getDishStructByBarcode(const QString &barcode, int menuId);
signals:

public slots:
};

#endif // DISHESTABLE_H
