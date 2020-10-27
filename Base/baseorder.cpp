#include "baseorder.h"
#include "defstore.h"
#include "storeoutput.h"
#include "databaseresult.h"

BaseOrder::BaseOrder(int id)
{
    fId = id;
}

void BaseOrder::calculateOutput(Database &db)
{
    calculateOutput(db, fId);
}

void BaseOrder::calculateOutput(Database &db, int id)
{
    QMap<QString, QVariant> fDbBind;
    DatabaseResult dr;
    fDbBind[":f_header"] = id;
    dr.select("delete from o_recipe where f_header=:f_header", fDbBind);

    fDbBind[":f_state1"] = DISH_STATE_READY;
    fDbBind[":f_state2"] = DISH_STATE_REMOVED_STORE;
    fDbBind[":f_header"] = id;
    dr.select(db, "select o.f_id, o.f_dish, o.f_store, r.f_part, r.f_qty*o.f_qty as f_qty, h.f_datecash "
                   "from o_dish o "
                   "left join o_header h on h.f_id=o.f_header "
                   "left join r_recipe r on o.f_dish=r.f_dish "
                   "where o.f_header=:f_header and (o.f_state=:f_state1 or o.f_state=:f_state2)", fDbBind);
    if (dr.rowCount() == 0) {
        return;
    }
    QDate docdate;
    for (int i = 0; i < dr.rowCount(); i++) {
        docdate = dr.value(i, "f_datecash").toDate();
        fDbBind[":f_header"] = id;
        fDbBind[":f_body"] = dr.value(i, "f_id");
        fDbBind[":f_dish"] = dr.value(i, "f_dish");
        fDbBind[":f_material"] = dr.value(i, "f_part");
        fDbBind[":f_qty"] = dr.value(i, "f_qty");
        fDbBind[":f_store"] = dr.value(i, "f_store");
        db.insert("o_recipe", fDbBind);
    }

    /* STORE OUTPUT */
    fDbBind[":f_date"] = docdate;
    fDbBind[":f_type"] = STORE_DOC_OUT;
    fDbBind[":f_state"] = 1;
    fDbBind[":f_partner"] = 0;
    fDbBind[":f_inv"] = "";
    fDbBind[":f_invDate"] = QVariant();
    fDbBind[":f_amount"] = 0;
    fDbBind[":f_remarks"] = QString("%1 %2").arg(QObject::tr("Sale")).arg(id);
    fDbBind[":f_op"] = 1;
    fDbBind[":f_fullDate"] = QDateTime::currentDateTime();
    fDbBind[":f_payment"] = 1;
    fDbBind[":f_rest"] = id;
    int docid = db.insert("r_docs", fDbBind);

    QList<QList<QVariant> > sr;
    fDbBind[":f_state1"] = DISH_STATE_READY;
    fDbBind[":f_state2"] = DISH_STATE_REMOVED_STORE;
    fDbBind[":f_header"] = id;
    db.select("select o.f_id, o.f_dish, o.f_store, r.f_part, r.f_qty*o.f_qty as f_qty "
                   "from o_dish o "
                   "left join r_recipe r on o.f_dish=r.f_dish "
                   "where o.f_header=:f_header and (o.f_state=:f_state1 or o.f_state=:f_state2)", fDbBind, sr);
    for (int i = 0; i < sr.count(); i++) {
        fDbBind[":f_doc"] = docid;
        fDbBind[":f_store"] =  sr.at(i).at(2);
        fDbBind[":f_material"] = sr.at(i).at(3);
        fDbBind[":f_sign"] = -1;
        fDbBind[":f_qty"] = sr.at(i).at(4);
        fDbBind[":f_price"] = 0;
        fDbBind[":f_total"] = 0;
        db.insert("r_body", fDbBind);
    }

    QMap<int, double> priceList;
    StoreOutput so(db, docid);
    so.output(priceList);
}
