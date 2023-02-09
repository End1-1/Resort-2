#include "hall.h"
#include "defrest.h"

QList<HallStruct*> Hall::fHallTable;
QMap<int, HallStruct*> Hall::fHallMap;
QList<HallStruct*> Hall::fBanketHall;
QList<TableStruct*> Hall::fTables;
QMap<int, TableStruct*> Hall::fTablesMap;

Hall::Hall(QObject *parent) :
  QObject(parent)
{

}

void Hall::init()
{
    qDeleteAll(fHallMap);
    fHallMap.clear();
    qDeleteAll(fBanketHall);
    fBanketHall.clear();
    fTablesMap.clear();
    qDeleteAll(fHallTable);
    fHallTable.clear();
    qDeleteAll(fTables);
    fTables.clear();
    QString query = QString("select h.f_id, h.f_name, h.f_defaultMenu, h.f_itemForInvoice, h.f_receiptPrinter, "
            "h.f_vatDept, h.f_noVatDept, h.f_serviceitem, h.f_servicevalue, r.f_en as f_servicename "
            "from r_hall h "
            "left join r_dish r on r.f_id=h.f_serviceitem "
            "where h.f_showhall=1 and h.f_branch=%1 order by h.f_name").arg(defrest(dr_branch));
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.constBegin(); it != fDbRows.constEnd(); it++) {
        HallStruct *h = new HallStruct();
        h->fId = it->value(0).toInt();
        h->fName = it->value(1).toString();
        h->fDefaultMenu = it->value(2).toInt();
        h->fItemIdForInvoice = it->value(3).toInt();
        h->fReceiptPrinter = it->value(4).toString();
        h->fVatDept = it->value(5).toString();
        h->fNoVatDept = it->value(6).toString();
        h->fServiceItem = it->value(7).toInt();
        h->fServiceValue = it->value(8).toDouble();
        h->fServiceName = it->value(9).toString();
        fHallTable.append(h);
        fHallMap[h->fId] = h;
    }

    query = "select h.f_id, h.f_name, h.f_defaultMenu, h.f_itemForInvoice, h.f_receiptPrinter, "
            "h.f_vatDept, h.f_noVatDept, h.f_serviceitem, h.f_servicevalue, r.f_en as f_servicename "
            "from r_hall h "
            "left join r_dish r on r.f_id=h.f_serviceitem "
            "where h.f_showBanket=1 order by h.f_name";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.constBegin(); it != fDbRows.constEnd(); it++) {
        HallStruct *h = new HallStruct();
        h->fId = it->value(0).toInt();
        h->fName = it->value(1).toString();
        h->fDefaultMenu = it->value(2).toInt();
        h->fItemIdForInvoice = it->value(3).toInt();
        h->fReceiptPrinter = it->value(4).toString();
        h->fVatDept = it->value(5).toString();
        h->fNoVatDept = it->value(6).toString();
        h->fServiceItem = it->value(7).toInt();
        h->fServiceValue = it->value(8).toDouble();
        h->fServiceName = it->value(9).toString();
        fBanketHall.append(h);
        if (!fHallMap.contains(h->fId)) {
            fHallMap[h->fId] = h;
        }
    }

    query = "select t.f_id, t.f_hall, t.f_name, t.f_lockTime, t.f_order, c.f_govnumber, cm.f_model "
            "from r_table t "
            "left join o_car c on t.f_order=c.f_order "
            "left join d_car_model cm on cm.f_id=c.f_model "
            "order by f_hall, f_queue ";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.constBegin(); it != fDbRows.constEnd(); it++) {
        TableStruct *t = new TableStruct();
        t->fId = it->value(0).toInt();
        t->fHall = it->value(1).toInt();
        t->fName = it->value(2).toString();
        t->fLocked = it->value(3).toDateTime();
        t->fOrder = it->value(4).toInt();
        t->fGovNumber = it->value(5).toString();
        t->fCar = it->value(6).toString();
        fTables.append(t);
        fTablesMap[t->fId] = t;
    }

    refresh();
}

void Hall::refresh()
{
    fDbBind[":f_state"] = ORDER_STATE_OPENED;
    QString query = "select t.f_id, t.f_lockHost, t.f_order, "
            "h.f_dateOpen, h.f_comment, u.f_firstName, "
            "h.f_total, c.f_govnumber, cm.f_model "
            "from r_table t "
            "left join o_header h on t.f_order=h.f_id "
            "left join users u on u.f_id=h.f_staff "
            "left join o_car c on t.f_order=c.f_order "
            "left join d_car_model cm on cm.f_id=c.f_model "
            "where h.f_state=:f_state "
            "group by 1 ";
    fDb.select(query, fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        TableStruct *t = getTableById(it->at(0).toInt());
        if (!t) {
            continue;
        }
        t->fLockStation = it->at(1).toString();
        t->fOrder = it->at(2).toInt();
        t->fDateOpen = it->at(3).toDateTime().toString(def_date_time_format);
        t->fComment = it->at(4).toString();
        t->fStaff = it->at(5).toString();
        t->fAmount = it->at(6).toString();
        t->fGovNumber = it->at(7).toString();
        t->fCar = it->at(8).toString();
    }
    fDb.select("select f_id, f_lockHost, f_order from r_table", fDbBind, fDbRows);
    for (QList<QList<QVariant> >::const_iterator it = fDbRows.begin(); it != fDbRows.end(); it++) {
        TableStruct *t = getTableById(it->at(0).toInt());
        if (!t) {
            continue;
        }
        t->fLockStation = it->at(1).toString();
        t->fOrder = it->at(2).toInt();
    }
    fDb.fDb.commit();
}

HallStruct *Hall::getHallById(int id)
{
    if (fHallMap.contains(id)) {
        return fHallMap[id];
    }
    return 0;
}

TableStruct *Hall::getTableById(int id)
{
    if (fTablesMap.contains(id)) {
        return fTablesMap[id];
    }
    return 0;
}

void Hall::filterTables(int hallId, int busy, QList<TableStruct *> &tables)
{
    tables.clear();
    for (QList<TableStruct*>::const_iterator it = fTables.constBegin(); it != fTables.constEnd(); it++) {
        if (hallId > 0) {
            if ((*it)->fHall != hallId) {
                continue;
            }
        }
        if (busy > 0) {
            switch (busy) {
            case 1:
                if (!(*it)->fOrder == 0) {
                    continue;
                }
                break;
            case 2:
                if ((*it)->fOrder == 0) {
                    continue;
                }
                break;
            }
        }
        tables.append(*it);
    }
}
