#include "dlgcalculateoutputofrestaurant.h"
#include "ui_dlgcalculateoutputofrestaurant.h"
#include "message.h"
#include "defstore.h"

DlgCalculateOutputOfRestaurant::DlgCalculateOutputOfRestaurant(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgCalculateOutputOfRestaurant)
{
    ui->setupUi(this);
}

DlgCalculateOutputOfRestaurant::~DlgCalculateOutputOfRestaurant()
{
    delete ui;
}

void DlgCalculateOutputOfRestaurant::openDialog()
{
    DlgCalculateOutputOfRestaurant *d = new DlgCalculateOutputOfRestaurant(fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}

void DlgCalculateOutputOfRestaurant::on_btnCancel_clicked()
{
    reject();
}

void DlgCalculateOutputOfRestaurant::on_btnGo_clicked()
{
    DatabaseResult dr;
    fDbBind[":d1"] = ui->deStart->date();
    fDbBind[":d2"] = ui->deEnd->date();
    fDbBind[":f_type"] = STORE_DOC_IN;
    fDbBind[":f_state"] = 1;
    dr.select(fDb, "select b.f_material, sum(b.f_total)/sum(b.f_qty) as f_price "
              "from r_body b "
              "left join r_docs d on d.f_id=b.f_doc "
              "where d.f_date between :d1 and :d2 and d.f_type=:f_type and d.f_state=:f_state", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fMiddlePrices[dr.value(i, "f_material").toInt()] = dr.value(i, "f_price").toDouble();
    }
    ui->pb->setValue(0);
    ui->pb->setMaximum(ui->deStart->date().daysTo(ui->deEnd->date()));
    for (QDate d = ui->deStart->date(); d < ui->deEnd->date().addDays(1); d = d.addDays(1)) {
        calculateForDate(d);
    }
    accept();
    message_info_tr("Done!");
}

void DlgCalculateOutputOfRestaurant::calculateForDate(const QDate &date)
{
    /* remove old calculation */
    fDbBind[":f_dateCash"] = date;
    fDb.select("delete from o_recipe where f_header in (select f_id from o_header where f_dateCash=:f_dateCash)", fDbBind, fDbRows);
    /* insert new calculation */
    fDbBind[":f_state1"] = DISH_STATE_READY;
    fDbBind[":f_state2"] = DISH_STATE_REMOVED_STORE;
    fDbBind[":f_hstate"] = ORDER_STATE_CLOSED;
    fDbBind[":f_dateCash"] = date;
    DatabaseResult dr;
    dr.select(fDb, "select h.f_id as hid, o.f_id, o.f_dish, o.f_store, r.f_part, r.f_qty*o.f_qty as f_qty "
                       "from o_dish o "
                       "left join r_recipe r on o.f_dish=r.f_dish "
                       "left join o_header h on h.f_id=o.f_header "
                       "where h.f_dateCash=:f_dateCash and h.f_state=:f_hstate and (o.f_state=:f_state1 or o.f_state=:f_state2)", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_header"] = dr.value(i, "hid");
        fDbBind[":f_body"] = dr.value(i, "f_id");
        fDbBind[":f_dish"] = dr.value(i, "f_dish");
        fDbBind[":f_material"] = dr.value(i, "f_part");
        fDbBind[":f_qty"] = dr.value(i, "f_qty");
        fDbBind[":f_store"] = dr.value(i, "f_store");
        fDb.insert("o_recipe", fDbBind);
    }
    /* and make output doc for store */
    fDbBind[":f_dateCash"] = date;
    dr.select(fDb, "select distinct(r.f_store) as f_store "
              "from o_recipe r "
              "left join o_header oh on oh.f_id=r.f_header "
              "where oh.f_dateCash=:f_dateCash ", fDbBind);
    ui->pb->setMaximum(ui->pb->maximum() + dr.rowCount());
    for (int i = 0; i < dr.rowCount(); i++) {
        calculateForStore(dr.value(i, "f_store").toInt(), date);
    }
}

void DlgCalculateOutputOfRestaurant::calculateForStore(int store, const QDate &date)
{
    ui->pb->setValue(ui->pb->value() + 1);
    fDbBind[":f_store"] = store;
    fDbBind[":f_date"] = date;
    DatabaseResult dr;
    dr.select(fDb, "select f_id from r_docs where f_rest=1 and f_date=:f_date and f_id in (select f_doc from r_body where f_store=:f_store and f_date=:f_date)", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        fDbBind[":f_id"] = dr.value(i, "f_id");
        fDb.select("delete from r_docs where f_id=:f_id", fDbBind, fDbRows);
        fDbBind[":f_doc"] = dr.value(i, "f_id");
        fDb.select("delete from r_body where f_doc=:f_doc", fDbBind, fDbRows);
    }
    fDbBind[":f_dateCash"] = date;
    fDbBind[":f_store"] = store;
    dr.select(fDb, "select r.f_material, sum(r.f_qty) as f_qty "
                   "from o_recipe r "
              "left join o_header oh on oh.f_id=r.f_header "
              "where oh.f_dateCash=:f_dateCash and r.f_store=:f_store "
              "group by 1 ", fDbBind);

    if (dr.rowCount() == 1 && dr.value("f_qty").toDouble() < 0.01) {
        return;
    }

    QString docId = uuid("ST", fAirDb);
    fDbBind[":f_id"] = docId;
    fDb.insertWithoutId("r_docs", fDbBind);
    fDbBind[":f_date"] = date;
    fDbBind[":f_type"] = STORE_DOC_OUT;
    fDbBind[":f_state"] = 1;
    fDbBind[":f_partner"] = 0;
    fDbBind[":f_inv"] = "";
    fDbBind[":f_invDate"] = QVariant();
    fDbBind[":f_amount"] = 0;
    fDbBind[":f_remarks"] = tr("Output of sale");
    fDbBind[":f_op"] = WORKING_USERID;
    fDbBind[":f_fullDate"] = QDateTime::currentDateTime();
    fDbBind[":f_rest"] = 1;
    fDb.update("r_docs", fDbBind, where_id(ap(docId)));

    for (int i = 0; i < dr.rowCount(); i++) {
        if (dr.value(i, "f_qty") < 0.01) {
            continue;
        }
        fDbBind[":f_id"] = 0;
        int newid = fDb.insert("r_body", fDbBind);
        fDbBind[":f_doc"] = docId;
        fDbBind[":f_store"] = store;
        fDbBind[":f_material"] = dr.value(i, "f_material");
        fDbBind[":f_sign"] = -1;
        fDbBind[":f_qty"] = dr.value(i, "f_qty");
        if (fMiddlePrices.contains(dr.value(i, "f_material").toInt())) {
            fDbBind[":f_price"] = fMiddlePrices[dr.value(i, "f_material").toInt()];
            fDbBind[":f_total"] = fMiddlePrices[dr.value(i, "f_material").toInt()] * dr.value(i, "f_qty").toDouble() * -1;
        } else {
            fDbBind[":f_price"] = fLastPrices[dr.value(i, "f_material").toInt()];
            fDbBind[":f_total"] = fLastPrices[dr.value(i, "f_material").toInt()] * dr.value(i, "f_qty").toDouble() * -1;
        }
        fDb.update("r_body", fDbBind, where_id(newid));
    }
}
