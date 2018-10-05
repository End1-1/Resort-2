#include "cachecheckoutinvoice.h"
#include "utils.h"

CacheCheckoutInvoice *CacheCheckoutInvoice::fInstance = 0;

CacheCheckoutInvoice *CacheCheckoutInvoice::instance()
{
    if (!fInstance) {
        fInstance = new CacheCheckoutInvoice();
        fCacheOne[cid_checkout_invoice] = fInstance;
    }
    return fInstance;
}


void CacheCheckoutInvoice::load()
{
    QString query = fQuery;
    query.replace(":date1", QDate::currentDate().addDays(-365).toString(def_mysql_date_format))
            .replace(":date2", QDate::currentDate().toString(def_mysql_date_format))
            .replace(":where_id", "");
    QSqlQuery *q = prepareDb(query);
    while (q->next()) {
        loadRecord(q);
    }
    closeDb(q);
}

CacheCheckoutInvoice::CacheCheckoutInvoice()
{
    fQuery = "select r.f_invoice, r.f_room, g.guest, r.f_startDate, r.f_endDate, "
            "i.amount, u.f_username, r.f_id "
            "from f_reservation r "
            "left join guests g on g.f_id=r.f_guest "
            "left join users u on u.f_Id=r.f_checkoutUser "
            "left join (select f_inv, sum(f_amountAmd) as amount from m_register m "
                "where m.f_finance=1 and m.f_canceled=0 and m.f_sign=1 "
                "group by 1) i on i.f_inv=r.f_invoice "
            "where r.f_endDate between ':date1' and ':date2' and r.f_state=3 :where_id "
            "order by r.f_endDate desc ";
    fCacheId = cid_checkout_invoice;
    if (!fStaticCache.contains(fCacheId)) {
        load();
    }
}

CacheCheckoutInvoice::~CacheCheckoutInvoice()
{
    fInstance = 0;
}

void CacheCheckoutInvoice::loadRecord(QSqlQuery *q)
{
    CI_CheckoutInvoice *c = 0;
    if (fStaticCache[fCacheId].contains(q->value(0).toString())) {
        c = fStaticCache[fCacheId][q->value(0).toString()];
    }
    if (!c) {
        c = new CI_CheckoutInvoice();
    }
    c->fCode = q->value(0).toString();
    c->fRoom = q->value(1).toString();
    c->fGuest = q->value(2).toString();
    c->fEntry = q->value(3).toDate().toString(def_date_format);
    c->fDeparture = q->value(4).toDate();
    c->fAmount = float_str(q->value(5).toDouble(), 2);
    c->fOperator = q->value(6).toString();
    c->fReservation = q->value(7).toString();
    fStaticCache[fCacheId][c->fCode] = c;
}
