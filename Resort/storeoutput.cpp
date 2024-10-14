#include "storeoutput.h"

StoreOutput::StoreOutput(Database &db, int doc) :
    fDoc(doc),
    fDb(db)
{
    
}

void StoreOutput::rollbackSale(Database &db, int order)
{
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > r;
    bind[":f_rest"] = order;
    db.select("select f_id from r_docs where f_rest=:f_rest", bind, r);
    if (r.count() == 0) {
        return;
    }
    for (int i = 0; i < r.count(); i++) {
        rollback(db, r.at(i).at(0).toInt());
    }
}

void StoreOutput::rollback(Database &db, int doc)
{
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > r;
    bind[":f_doc"] = doc;
    db.select("delete from r_store_acc where f_doc=:f_doc", bind, r);
    bind[":f_doc"] = doc;
    db.select("delete from r_body where f_doc=:f_doc", bind, r);
    bind[":f_id"] = doc;
    db.select("delete from r_docs where f_id=:f_id", bind, r);
}

void StoreOutput::output(QMap<int, double> &priceList)
{
    QMap<QString, QVariant> bind;
    QList<QList<QVariant> > rows;
    bind[":f_doc"] = fDoc;
    fDb.select("select s.f_id, s.f_store, s.f_material, s.f_qty, g.f_lastprice "
               "from r_body s "
               "inner join r_dish g on g.f_id=s.f_material "
               "where f_doc=:f_doc and f_sign=-1", bind, rows);
    int docrow = 1;
    double docamount = 0;
    for (QList<QVariant> &v: rows) {
        double qty_total = v.at(3).toDouble();
        QMap<QString, QVariant> b;
        QList<QList<QVariant> > r;
        b[":f_goods"] = v.at(2);
        fDb.select("select s.f_base, s.f_price, sum(s.f_qty*s.f_sign) "
                   "from r_store_acc s "
                   "inner join r_docs d on d.f_id=s.f_doc "
                   "where s.f_goods=:f_goods "
                   "group by s.f_base "
                   "having sum(s.f_qty*s.f_sign)>0 "
                   "order by d.f_date ", b, r);

        double qp = 0, qs = 0;
        for (int i = 0; i < r.count(); i++) {
            if (v.at(3).toDouble() < 0.00001) {
                break;
            }
            double qty = 0.0;
            if (r.at(i).at(2).toDouble() > v.at(3).toDouble()) {
                qp += v[3].toDouble();
                qs += r.at(i).at(1).toDouble() * v[3].toDouble();
                qty = v[3].toDouble();
                v[3] = 0;
            } else {
                qp += r.at(i).at(2).toDouble();
                qs += r.at(i).at(2).toDouble() * r.at(i).at(1).toDouble();
                qty = r.at(i).at(2).toDouble();
                priceList[v.at(0).toInt()] = r.at(i).at(1).toDouble();
                v[3] = v[3].toDouble() - r.at(i).at(2).toDouble();
            }

            QMap<QString, QVariant> iv;
            iv[":f_base"] = r.at(i).at(0);
            iv[":f_doc"] = fDoc;
            iv[":f_sign"] = -1;
            iv[":f_docrow"] = docrow++;
            iv[":f_store"] = v.at(1);
            iv[":f_goods"] = v.at(2);
            iv[":f_price"] = r.at(i).at(1);
            iv[":f_qty"] = qty;
            fDb.insert("r_store_acc", iv);
        }
        if (v.at(3).toDouble() > 0.00001) {
            qp += v.at(3).toDouble();
            qs += v.at(3).toDouble() * v.at(4).toDouble();
            priceList[v.at(0).toInt()] = v.at(4).toDouble();
            QMap<QString, QVariant> iv;
            iv[":f_base"] = 0;
            iv[":f_doc"] = fDoc;
            iv[":f_sign"] = -1;
            iv[":f_docrow"] = docrow++;
            iv[":f_store"] = v.at(1);
            iv[":f_goods"] = v.at(2);
            iv[":f_price"] = v.at(4);
            iv[":f_qty"] = v.at(3).toDouble();
            fDb.insert("r_store_acc", iv);
        }
        priceList[v.at(0).toInt()] = qs / qp;
        bind[":f_id"] = v.at(0).toInt();
        bind[":f_price"] = qp > 0.001 ? (qs / qp) : 0;
        bind[":f_total"] = qp > 0.001 ? (qs / qp) * qty_total : 0;
        fDb.update("r_body", bind, where_id(v.at(0).toInt()));

        docamount += qs;
    }
    bind[":f_amount"] = docamount;
    fDb.update("r_docs", bind, where_id(fDoc));
}
