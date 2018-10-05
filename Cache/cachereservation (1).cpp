#include "cachereservation.h"
#include "utils.h"

CacheReservation *CacheReservation::fInstance = 0;

CacheReservation *CacheReservation::instance()
{
    if (fInstance == 0) {
        fInstance = new CacheReservation();
        fCacheOne.insert(cid_reservation, fInstance);
    }
    return fInstance;
}

void CacheReservation::load()
{
    Preferences p;
    QString where = QString(" r.f_state in (1,2,7) or (r.f_state in (4, 9) and r.f_endDate>="
                            + ap(p.getLocalDate(def_working_day).toString(def_mysql_date_format)) + ") ");
    QSqlQuery *q = prepareDb(fQuery + where);
    while (q->next()) {
        int i = 0;
        CI_Reservation *c = new CI_Reservation();
        c->fId = q->value(i++).toString();
        c->fState = q->value(i++).toInt();
        c->fStatus = q->value(i++).toInt();
        c->fRoom = q->value(i++).toString();
        c->fRoomShort = q->value(i++).toString();
        c->fCardex = q->value(i++).toString();
        c->fDateStart = q->value(i++).toDate();
        c->fDateEnd = q->value(i++).toDate();
        c->fGuest = q->value(i++).toString();
        c->fCardexName = q->value(i++).toString();
        c->fInvoice = q->value(i++).toString();
        c->fStatusName = q->value(i++).toString();
        c->fAdvance = q->value(i++).toString();
        c->fCredit = q->value(i++).toString();
        c->fDebet = q->value(i++).toString();
        c->fBalance = float_str(c->fCredit.toFloat() - c->fDebet.toFloat(), 0);
        c->fBalance = float_str(c->fCredit.toFloat() - c->fDebet.toFloat(), 2);
        c->fRemarks = q->value(i++).toString();
        c->fArrangement = q->value(i++).toInt();
        c->fAuthor = q->value(i++).toInt();
        c->fGroup = q->value(i++).toInt();
        c->fDateCreate = q->value(i++).toDate().toString(def_date_format);
        c->fDateCreate += " " + q->value(i++).toTime().toString(def_time_format);
        fStaticCache[fCacheId][c->fId] = c;
    }
    closeDb(q);
}

void CacheReservation::updateItem(const QString &id)
{
    QString where = QString("  r.f_state in (%1, %2, %3, %4, %5) and r.f_id=" + ap(id))
            .arg(RESERVE_CHECKIN)
            .arg(RESERVE_RESERVE)
            .arg(RESERVE_SERVICE)
            .arg(RESERVE_OUTOFINVENTORY)
            .arg(RESERVE_OUTOFROOM)
            ;
    QSqlQuery *q = prepareDb(fQuery + where);
    CI_Reservation *c = get(id);
    if (q->next()) {
        int i = 0;
        if (!c) {
            c = new CI_Reservation();
        }
        c->fId = q->value(i++).toString();
        c->fState = q->value(i++).toInt();
        c->fStatus = q->value(i++).toInt();
        c->fRoom = q->value(i++).toString();
        c->fRoomShort = q->value(i++).toString();
        c->fCardex = q->value(i++).toString();
        c->fDateStart = q->value(i++).toDate();
        c->fDateEnd = q->value(i++).toDate();
        c->fGuest = q->value(i++).toString();
        c->fCardexName = q->value(i++).toString();
        c->fInvoice = q->value(i++).toString();
        c->fStatusName = q->value(i++).toString();
        c->fAdvance = q->value(i++).toString();
        c->fCredit = q->value(i++).toString();
        c->fDebet = q->value(i++).toString();
        c->fBalance = float_str(c->fCredit.toFloat() - c->fDebet.toFloat(), 2);
        c->fRemarks = q->value(i++).toString();
        c->fArrangement = q->value(i++).toInt();
        c->fAuthor = q->value(i++).toInt();
        c->fGroup = q->value(i++).toInt();
        c->fDateCreate = q->value(i++).toDate().toString(def_date_format);
        c->fDateCreate += " " + q->value(i++).toTime().toString(def_time_format);
        c->fValid = true;
        fStaticCache[fCacheId][c->fId] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
    closeDb(q);
}

bool CacheReservation::hasNext(CI_Reservation *c)
{
    QMapIterator<QString, CI_Reservation*> i(it());
    while (i.hasNext()) {
        i.next();
        if (i.value()->fRoom != c->fRoom) {
            continue;
        }
        if (i.value()->fId == c->fId) {
            continue;
        }
        if (i.value()->fDateStart == c->fDateEnd) {
            return true;
        }
    }
    return false;
}

bool CacheReservation::hasPrev(CI_Reservation *c)
{
    QMapIterator<QString, CI_Reservation*> i(it());
    while (i.hasNext()) {
        i.next();
        if (i.value()->fRoom != c->fRoom) {
            continue;
        }
        if (i.value()->fId == c->fId) {
            continue;
        }
        if (i.value()->fDateEnd == c->fDateStart) {
            return true;
        }
    }
    return false;
}

CacheReservation::CacheReservation() :
    CacheBase()
{
    fQuery = "select r.f_id, r.f_state, r.f_reserveState, r.f_room, rm.f_short, r.f_cardex, r.f_startDate, r.f_endDate, "
                "concat(g.f_firstName, ' ', g.f_lastName) as f_guest_name, c.f_name, r.f_invoice,  "
                "rs.f_" + def_lang + ", a.amount, coalesce(cc.credit, 0), coalesce(cd.debet, 0), "
                "r.f_remarks, r.f_arrangement, r.f_author, r.f_group, "
                "r.f_created, r.f_createTime "
                "from f_reservation r "
                "left join f_room rm on rm.f_id=r.f_room "
                "left join f_guests g on r.f_guest=g.f_id "
                "inner join f_reservation_status rs on rs.f_id=r.f_reserveState "
                "left join f_cardex c on c.f_cardex=r.f_cardex "
                "left join (select f_inv, sum(f_amountAmd) as amount from m_register where f_source='AV' and f_canceled=0 and f_finance=1) a on  a.f_inv=r.f_invoice "
                "left join (select f_inv, sum(f_amountAmd) as credit from m_register where f_sign=1 and f_canceled=0 and f_finance=1 group by 1) cc on cc.f_inv=r.f_invoice "
                "left join (select f_inv, sum(f_amountAmd) as debet from m_register where f_sign=-1 and f_canceled=0 and f_finance=1 group by 1) cd on cd.f_inv=r.f_invoice "
                " where ";
    fCacheId = cid_reservation;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

void CacheReservation::check(const QDate &start, const QDate &end, int room,
                             QMap<QString, CI_Reservation *> &out,
                             bool &startOk, bool &endOk, const QString &reserveId)
{
//    Preferences pref;
//    if (start < QDate::fromString(pref.getDb(def_working_day).toString(), def_date_format)) {
//        startOk = false;
//        return;
//    }
    QMapIterator<QString, CI_Reservation*> i = it();
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            continue;
        }
        if (i.value()->fRoom.toInt() != room) {
            continue;
        }
        if (i.value()->fId == reserveId) {
            continue;
        }
        if (i.value()->fState == RESERVE_OUTOFINVENTORY || i.value()->fState == RESERVE_OUTOFROOM) {
            if (start == i.value()->fDateEnd || end == i.value()->fDateStart) {
                out.insert(i.value()->fId, i.value());
                startOk = false;
                endOk = false;
                continue;
            }
        }
        if (start <= i.value()->fDateStart && end >= i.value()->fDateEnd && i.value()->fDateStart != i.value()->fDateEnd) {
            out.insert(i.value()->fId, i.value());
            startOk = false;
            endOk = false;
            continue;
        }
        if (start > i.value()->fDateStart && end < i.value()->fDateEnd) {
            out.insert(i.value()->fId, i.value());
            startOk = false;
            endOk = false;
            continue;
        }
        if (start <= i.value()->fDateStart) {
            if (end > i.value()->fDateStart) {
                if (i.value()->fDateStart != i.value()->fDateEnd) {
                    out.insert(i.value()->fId, i.value());
                    endOk = false;
                    continue;
                }
            }
        }
        if (end > i.value()->fDateEnd) {
            if (start < i.value()->fDateEnd) {
                out.insert(i.value()->fId, i.value());
                startOk = false;
                continue;
            }
        }
    }
}

void CacheReservation::exludeList(const QDate &start, const QDate &end, QSet<int> &excludeRooms)
{
    excludeRooms.clear();
    QMapIterator<QString, CI_Reservation*> i = it();
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            continue;
        }
        if (start <= i.value()->fDateStart && end >= i.value()->fDateEnd) {
            excludeRooms.insert(i.value()->fRoom.toInt());
            continue;
        }
        if (start >= i.value()->fDateStart && end <= i.value()->fDateEnd) {
            excludeRooms.insert(i.value()->fRoom.toInt());
            continue;
        }
        if (start <= i.value()->fDateStart) {
            if (end > i.value()->fDateStart) {
                excludeRooms.insert(i.value()->fRoom.toInt());
                continue;
            }
        }
        if (end > i.value()->fDateEnd) {
            if (start < i.value()->fDateEnd) {
                excludeRooms.insert(i.value()->fRoom.toInt());
                continue;
            }
        }
    }
}

CacheReservation::CacheReservation(CacheReservation &c) :
    CacheBase()
{
    Q_UNUSED(c)
}

CacheReservation::~CacheReservation()
{
    fInstance = 0;
}
