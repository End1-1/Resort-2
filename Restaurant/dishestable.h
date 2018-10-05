#ifndef DISHESTABLE_H
#define DISHESTABLE_H

#include "base.h"
#include "splash.h"
#include <QObject>

typedef struct {
    int fId;
    QMap<QString, QString> fName;
} MenuStruct;
Q_DECLARE_METATYPE(MenuStruct*)

typedef struct {
    int fId;
    QMap<QString, QString> fName;
} DishPartStruct;
Q_DECLARE_METATYPE(DishPartStruct*)

typedef struct {
    int fId;
    int fPart;
    QMap<QString, QString> fName;
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
    QMap<QString, QString> fName;
    int fBgColor;
    int fTextColor;
    QMap<QString, QString> fText;
    QList<QMap<QString, QString> > fMod;
    QString fPrint1;
    QString fPrint2;
    int fStore;
    double fSvcValue;
    float fPrice;
    int fQueue;
    int fComplex;
    QString fComplexRec;
    QString fAdgt;
} DishStruct;
Q_DECLARE_METATYPE(DishStruct*)

typedef struct {
    QString fRecId;
    int fState;
    int fCancelUser;
    QDateTime fCancelDate;
    int fDishId;
    QMap<QString, QString> fName;
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
} OrderDishStruct;
Q_DECLARE_METATYPE(OrderDishStruct*)

typedef struct {
    QString fRecId;
    int fId;
    QMap<QString, QString> fName;
    QTime fStart;
    QTime fEnd;
    double fQty;
    double fPrice;
    QString fAdgt;
    double fPriceDeviation;
    QList<DishStruct*> fDishes;
} DishComplexStruct;
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
    QMap<QString, QString> fName;
} DishMod;

class DishesTable : public QObject, Base
{
    Q_OBJECT
public:
    explicit DishesTable(QObject *parent = 0);
    void init(Splash *s = 0);
    void filterType(int menuId, int partId, QMap<int, TypeStruct*> &type);
    void filterDish(int menuId, int typeId, QMap<int, DishStruct*> &dish);
    static QList<QMap<QString, QString> > fMods;
    static QMap<int, MenuStruct*> fMenu;
    static QList<DishPartStruct*> fDishPart;
    static QMap<int, TypeStruct*> fType;
    static QMap<int, QSet<TypeStruct*> > fTypeMenu;
    static QMap<QString, TypeStruct*> fTypeProxy;
    static QList<DishStruct*> fDish;
    static QMap<int, QList<DishStruct*> > fDishMenu;
    static QList<DishComplexStruct*> fDishComplex;
signals:

public slots:
};

#endif // DISHESTABLE_H
