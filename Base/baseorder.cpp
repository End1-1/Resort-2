#include "baseorder.h"

BaseOrder::BaseOrder(const QString &id) :
    Base()
{
    fDb.close();
    fId = id;
}

void BaseOrder::calculateOutput()
{
    calculateOutput(fId);
}

void BaseOrder::calculateOutput(const QString &id)
{
    if (!fDb.open()) {
        return;
    }
    DatabaseResult dr;
    fDbBind[":f_state1"] = DISH_STATE_READY;
    fDbBind[":f_state2"] = DISH_STATE_REMOVED_STORE;
    fDbBind[":f_header"] = id;
    dr.select(fDb, "select o.f_id, o.f_dish, o.f_store, r.f_part, r.f_qty*o.f_qty as f_qty "
                   "from o_dish o "
                   "left join r_recipe r on o.f_dish=r.f_dish "
                   "where o.f_header=:f_header and (o.f_state=:f_state1 or o.f_state=:f_state2)", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_header"] = id;
        fDbBind[":f_body"] = dr.value(i, "f_id");
        fDbBind[":f_dish"] = dr.value(i, "f_dish");
        fDbBind[":f_material"] = dr.value(i, "f_part");
        fDbBind[":f_qty"] = dr.value(i, "f_qty");
        fDbBind[":f_store"] = dr.value(i, "f_store");
        fDb.insert("o_recipe", fDbBind);
    }
    fDb.close();
}
