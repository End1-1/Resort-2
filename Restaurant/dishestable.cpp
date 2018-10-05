#include "dishestable.h"

QMap<int, MenuStruct*> DishesTable::fMenu;
QList<DishPartStruct*> DishesTable::fDishPart;
QMap<int, TypeStruct*> DishesTable::fType;
QMap<int, QSet<TypeStruct*> > DishesTable::fTypeMenu;
QMap<QString, TypeStruct*> DishesTable::fTypeProxy;
QList<DishStruct*> DishesTable::fDish;
QMap<int, QList<DishStruct*> > DishesTable::fDishMenu;
QList<QMap<QString, QString> > DishesTable::fMods;
QList<DishComplexStruct*> DishesTable::fDishComplex;

DishesTable::DishesTable(QObject *parent) :
    QObject(parent),
    Base()
{

}

void DishesTable::init(Splash *s)
{
    if (s) {
        s->setText(tr("Loading menus..."));
    }
    qDeleteAll(fMenu);
    fMenu.clear();
    QString query = "select f_id, f_en, f_ru, f_am from r_menu_names";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        MenuStruct *m = new MenuStruct();
        m->fId = it->at(0).toInt();
        m->fName["en"] = it->at(1).toString();
        m->fName["ru"] = it->at(2).toString();
        m->fName["am"] = it->at(3).toString();
        fMenu[m->fId] = m;
    }

    if (s) {
        s->setText(tr("Loading dishes parts..."));
    }
    qDeleteAll(fDishPart);
    fDishPart.clear();
    query = "select f_id, f_en, f_ru, f_am from r_dish_part order by f_id";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        DishPartStruct *d = new DishPartStruct();
        d->fId = it->at(0).toInt();
        d->fName["en"] = it->at(1).toString();
        d->fName["ru"] = it->at(2).toString();
        d->fName["am"] = it->at(3).toString();
        fDishPart.append(d);
    }

    if (s) {
        s->setText(tr("Loading dishes types..."));
    }
    fTypeProxy.clear();
    qDeleteAll(fType);
    fType.clear();
    query = "select f_id, f_part, f_en, f_ru, f_am, f_bgColor, f_textColor, f_queue from r_dish_type where f_active=1";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        TypeStruct *t = new TypeStruct();
        t->fId = it->at(0).toInt();
        t->fPart = it->at(1).toInt();
        t->fName["en"] = it->at(2).toString();
        t->fName["ru"] = it->at(3).toString();
        t->fName["am"] = it->at(4).toString();
        t->fBgColor = it->at(5).toInt();
        t->fTextColor = it->at(6).toInt();
        t->fQueue = it->at(7).toInt();
        fType[t->fId] = t;
    }

    if (s) {
        s->setText(tr("Loading complex dishes..."));
    }
    qDeleteAll(fDishComplex);
    fDishComplex.clear();
    query = "select d.f_id, d.f_en, d.f_ru, d.f_am, d.f_startTime, d.f_endTime, d.f_price, "
            "d.f_priceDeviation, d.f_adgt "
            "from r_dish_complex d where d.f_enabled=1 ";
    fDb.select(query, fDbBind, fDbRows);
    foreach_rows {
        DishComplexStruct *dc =  new DishComplexStruct();
        int c = 0;
        dc->fId = it->at(c++).toInt();
        dc->fName["en"] = it->at(c++).toString();
        dc->fName["ru"] = it->at(c++).toString();
        dc->fName["am"] = it->at(c++).toString();
        dc->fStart = QTime::fromString(it->at(c++).toString(), def_time_format);
        dc->fEnd = QTime::fromString(it->at(c++).toString(), def_time_format);
        dc->fQty = 1;
        dc->fPrice = it->at(c++).toFloat();
        dc->fPriceDeviation = it->at(c++).toFloat();
        dc->fAdgt = it->at(c++).toString();
        fDishComplex.append(dc);
    }
    if (s) {
        s->setText(tr("Loading complex dishes components"));
        query = "select m.f_dish, m.f_menu, t.f_part, d.f_type, d.f_en, d.f_ru, d.f_am,"
                "d.f_bgColor, d.f_textColor, d.f_text_en, d.f_text_ru, d.f_text_am,"
                "m.f_print1, m.f_print2, m.f_store, m.f_price, d.f_queue, mc.f_complex "
                "from r_menu m "
                "inner join r_dish d on d.f_id=m.f_dish "
                "inner join r_dish_type t on d.f_type=t.f_id "
                "inner join r_dish_complex_list mc on mc.f_dish=m.f_id "
                "where m.f_state=1 and d.f_type in (select f_id from r_dish_type where f_active=1)";
        fDb.select(query, fDbBind, fDbRows);
        foreach_rows {
            DishStruct *d = new DishStruct();
            d->fId = it->at(0).toInt();
            d->fMenu = it->at(1).toInt();
            d->fPart = it->at(2).toInt();
            d->fType = it->at(3).toInt();
            d->fName["en"] = it->at(4).toString();
            d->fName["ru"] = it->at(5).toString();
            d->fName["am"] = it->at(6).toString();
            d->fBgColor = it->at(7).toInt();
            d->fTextColor = it->at(8).toInt();
            d->fText["en"] = it->at(9).toString();
            d->fText["ru"] = it->at(10).toString();
            d->fText["am"] = it->at(11).toString();
            d->fPrint1 = it->at(12).toString();
            d->fPrint2 = it->at(13).toString();
            d->fStore = it->at(14).toInt();
            d->fPrice = it->at(15).toFloat();
            d->fQueue = it->at(16).toInt();
            d->fComplex = it->at(17).toInt();
            for (int i = 0; i < fDishComplex.count(); i++) {
                if (d->fComplex == fDishComplex.at(i)->fId) {
                    fDishComplex[i]->fDishes.append(d);
                }
            }
        }
    }

    if (s) {
        s->setText(tr("Loading menu..."));
    }
    query = "select m.f_dish, m.f_menu, t.f_part, d.f_type, d.f_en, d.f_ru, d.f_am,"
            "d.f_bgColor, d.f_textColor, d.f_text_en, d.f_text_ru, d.f_text_am,"
            "m.f_print1, m.f_print2, m.f_store, m.f_price, d.f_queue, m.f_complex, "
            "d.f_adgt "
            "from r_menu m "
            "inner join r_dish d on d.f_id=m.f_dish "
            "inner join r_dish_type t on d.f_type=t.f_id "
            "where m.f_state=1  and m.f_complex=0 and t.f_active=1";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        DishStruct *d = new DishStruct();
        d->fId = it->at(0).toInt();
        d->fMenu = it->at(1).toInt();
        d->fPart = it->at(2).toInt();
        d->fType = it->at(3).toInt();
        d->fName["en"] = it->at(4).toString();
        d->fName["ru"] = it->at(5).toString();
        d->fName["am"] = it->at(6).toString();
        d->fBgColor = it->at(7).toInt();
        d->fTextColor = it->at(8).toInt();
        d->fText["en"] = it->at(9).toString();
        d->fText["ru"] = it->at(10).toString();
        d->fText["am"] = it->at(11).toString();
        d->fPrint1 = it->at(12).toString();
        d->fPrint2 = it->at(13).toString();
        d->fStore = it->at(14).toInt();
        d->fPrice = it->at(15).toFloat();
        d->fQueue = it->at(16).toInt();
        d->fComplex = it->at(17).toBool();
        d->fAdgt = it->at(18).toString();
        fDish.append(d);
    }

    if (s) {
        s->setText(tr("Processing modifiers"));
    }
    query = "select m.f_dish, mn.f_en, mn.f_ru, mn.f_am "
            "from r_dish_mod_required m "
            "inner join r_dish_mod mn on mn.f_id=m.f_mod "
            "order by m.f_dish";
    fDb.select(query, fDbBind, fDbRows);
    foreach_rows {
        for (QList<DishStruct*>::iterator d = fDish.begin(); d != fDish.end(); d++) {
            DishStruct *dish = *d;
            if (dish->fId == it->at(0).toInt()) {
                QMap<QString, QString> m;
                m["en"] = it->at(1).toString();
                m["ru"] = it->at(2).toString();
                m["am"] = it->at(3).toString();
                dish->fMod.append(m);
                continue;
            }
        }
    }
    query = "select f_id, f_en, f_ru, f_am from r_dish_mod";
    fDb.select(query, fDbBind, fDbRows);
    foreach_rows {
        QMap<QString, QString> m;
        m["en"] = it->at(1).toString();
        m["ru"] = it->at(2).toString();
        m["am"] = it->at(3).toString();
        fMods.append(m);
    }

    if (s) {
        s->setText(tr("Processing menu..."));
    }
    fTypeMenu.clear();
    for (QMap<int, MenuStruct*>::const_iterator it = fMenu.begin(); it != fMenu.end(); it++) {
        fTypeMenu[it.key()] = QSet<TypeStruct*>();
        fDishMenu[it.key()] = QList<DishStruct*>();
    }
    for (QList<DishStruct*>::const_iterator it = fDish.begin(); it != fDish.end(); it++) {
        DishStruct *d = *it;
        TypeStruct *t = fType[d->fType];
        fTypeMenu[d->fMenu].insert(t);
        fDishMenu[d->fMenu].append(d);
    }
}

void DishesTable::filterType(int menuId, int partId, QMap<int, TypeStruct *> &type)
{
    QSet<TypeStruct*> &typeProxy = fTypeMenu[menuId];
    for (QSet<TypeStruct*>::const_iterator it = typeProxy.begin(); it != typeProxy.end(); it++) {
        TypeStruct *t = *it;
        if (partId != 0) {
            if (t->fPart != partId) {
                continue;
            }
        }
        type[t->fQueue] = t;
    }
}

void DishesTable::filterDish(int menuId, int typeId, QMap<int, DishStruct *> &dish)
{
    QList<DishStruct*> &dishProxy = fDishMenu[menuId];
    for (QList<DishStruct*>::const_iterator it = dishProxy.begin(); it != dishProxy.end(); it++) {
        DishStruct *d = *it;
        if (typeId != d->fType) {
            continue;
        }
        dish[d->fQueue] = d;
    }
}
