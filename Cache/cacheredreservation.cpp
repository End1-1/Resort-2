#include "cacheredreservation.h"
#include "utils.h"

CacheRedReservation *CacheRedReservation::fInstance;

CacheRedReservation *CacheRedReservation::instance()
{
    if (!fInstance) {
        fInstance = new CacheRedReservation();
        fCacheOne[cid_red_reservation] = fInstance;
    }
    return fInstance;
}

void CacheRedReservation::load()
{
    QSqlQuery *q = prepareDb(fQuery + " group by 1, 2, 3");
    while (q->next()) {
        CI_RedReservation *c = new CI_RedReservation();
        int i = 0;
        c->fCode = q->value(i++).toString();
        c->fName = q->value(i++).toString();
        c->fRoom = q->value(i++).toString();
        c->fBalance = float_str(q->value(i++).toDouble(), 0);
        c->fEntry = q->value(i++).toDate().toString(def_date_format);
        c->fDeparture = q->value(i++).toDate().toString(def_date_format);
        c->fInvoice = q->value(i++).toString();
        fStaticCache[fCacheId][c->fCode] = c;
    }
}

void CacheRedReservation::updateItem(const QString &id)
{
    QSqlQuery *q = prepareDb(fQuery + " and r.f_id=" + ap(id) + " group by 1, 2, 3");
    CI_RedReservation *c = get(q->value(0).toString());
    if (q->next()) {
        if (!c) {
            c = new CI_RedReservation();
        }
        int i = 0;
        c->fCode = q->value(i++).toString();
        c->fName = q->value(i++).toString();
        c->fRoom = q->value(i++).toString();
        c->fBalance = float_str(q->value(i++).toDouble(), 0);
        c->fEntry = q->value(i++).toDate().toString(def_date_format);
        c->fDeparture = q->value(i++).toDate().toString(def_date_format);
        c->fInvoice = q->value(i++).toString();
        c->fValid = true;
        fStaticCache[fCacheId][c->fCode] = c;
    } else {
        if (c) {
            c->fValid = false;
        }
    }
}

CacheRedReservation::CacheRedReservation() :
    CacheBase()
{
    fCacheId = cid_red_reservation;
    fQuery = "select r.f_id, concat(g.f_title, ' ', g.f_firstName, ' ' ,g.f_lastName), \
            r.f_room, a.amount, r.f_startDate, r.f_endDate, r.f_invoice \
            from f_reservation r \
            left join (select f_res, sum(f_amountAmd*f_sign*-1) as amount from m_register where f_source='AV' and f_canceled=0 group by 1) a on a.f_res=r.f_id \
            inner join f_guests g on g.f_id=r.f_guest \
            where r.f_state=2 ";
    if (!fStaticCache.contains(cid_red_reservation)) {
        load();
    }
}

CacheRedReservation::~CacheRedReservation()
{
    fInstance = 0;
}
