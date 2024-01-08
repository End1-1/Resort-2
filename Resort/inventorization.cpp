#include "inventorization.h"
#include "ui_inventorization.h"
#include "dlggetidname.h"
#include "database2.h"
#include "freportfromstoredproc.h"
#include <QInputDialog>

#define col_goods_id 0
#define col_goods_name 1
#define col_unit_name 2
#define col_qty_before 3
#define col_qty_input 4
#define col_qty_output 5
#define col_qty_realout 6
#define col_qty_diff 7
#define col_qty_remain 8
#define col_qty_counted 9
#define col_avg_price 10
#define col_amount 11

Inventorization::Inventorization(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::Inventorization),
    fDocId(0)
{
    ui->setupUi(this);
    connect(ui->leStore, &EQLineEdit::customButtonClicked, this, &Inventorization::editStoreClicked);
}

Inventorization::~Inventorization()
{
    delete ui;
}

void Inventorization::setupTab()
{
    setupTabTextAndIcon("Գույքագրում", ":/images/refund.png");
}

void Inventorization::editStoreClicked()
{
    QString id, name;
    if (DlgGetIDName::get(id, name, idname_store, this)) {
        ui->leStore->setText(name);
        ui->leStore->fHiddenText = id;
    }
}

void Inventorization::on_btnLoad_clicked()
{
    ui->tbl->setRowCount(0);
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2[":date1"] = ui->date1->date();
    db2[":date2"] = ui->date2->date();
    db2[":store"] = ui->leStore->fHiddenText.toInt();
    db2.exec("call sp_inventorization_doc (:date1, :date2, :store);");
    db2.exec("select * from rep");
    while (db2.next()) {
        int r = ui->tbl->addRow();
        for (int i = 0; i < ui->tbl->columnCount(); i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->tbl->setItem(r, i, item);
        }
        for (int i = 0; i < db2.columnCount(); i++){
            if (i > ui->tbl->columnCount() - 1) {
                break;
            }
            ui->tbl->item(r, i)->setData(Qt::EditRole, db2.value(i));;
        }
    }
    ui->tbl->resizeColumnsToContents();

    db2[":date1"] = ui->date1->date();
    db2[":date2"] = ui->date2->date();
    db2[":store"] = ui->leStore->fHiddenText.toInt();
    db2.exec("SELECT f_id FROM r_inventorization WHERE f_datestart=:date1 AND f_dateend = :date2 AND f_store=:store");
    if (db2.next()) {
        ui->leInvDoc->setInt(db2.integer("f_id"));
        db2[":f_invdoc"] = db2.integer("f_id");
        db2.exec("select * from r_docs where f_invdoc=:f_invdoc");
        if (db2.next()) {
            ui->btnRemoveOutput->setEnabled(true);
            ui->btnWriteOutput->setEnabled(false);
        } else {
            ui->btnRemoveOutput->setEnabled(false);
            ui->btnWriteOutput->setEnabled(true);
        }
    } else {
        ui->btnRemoveOutput->setEnabled(false);
        ui->btnWriteOutput->setEnabled(true);
    }
}

void Inventorization::on_tbl_cellDoubleClicked(int row, int column)
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    bool ok = false;
    double v = 0;
    switch (column) {
    case col_qty_remain:
        v = QInputDialog::getDouble(this, "Մնացորդ", "", 0, 0, 999999, 3, &ok);
        if (!ok) {
            return;
        }
        ui->tbl->item(row, col_qty_remain)->setData(Qt::EditRole, v);
        ui->tbl->item(row, col_qty_realout)->setData(Qt::EditRole,
                                                     ui->tbl->item(row, col_qty_before)->data(Qt::EditRole).toDouble()
                                                     + ui->tbl->item(row, col_qty_input)->data(Qt::EditRole).toDouble()
                                                     - ui->tbl->item(row, col_qty_remain)->data(Qt::EditRole).toDouble());
        if (ui->tbl->item(row, col_qty_realout)->data(Qt::EditRole).toDouble() < 0.001) {
            ui->tbl->item(row, col_qty_realout)->setData(Qt::EditRole, 0);
        }
        ui->tbl->item(row, col_qty_diff)->setData(Qt::EditRole,
                                                  ui->tbl->item(row, col_qty_output)->data(Qt::EditRole).toDouble()
                                                  - ui->tbl->item(row, col_qty_realout)->data(Qt::EditRole).toDouble());
        ui->tbl->item(row, col_amount)->setData(Qt::EditRole,
                                                ui->tbl->item(row, col_qty_realout)->data(Qt::EditRole).toDouble()
                                                * ui->tbl->item(row, col_avg_price)->data(Qt::EditRole).toDouble());
        db2[":date1"] = ui->date1->date();
        db2[":date2"] = ui->date2->date();
        db2[":store"] = ui->leStore->fHiddenText.toInt();
        db2[":goods"] = ui->tbl->item(row, col_goods_id)->data(Qt::EditRole);
        db2[":qtyout"] = ui->tbl->item(row, col_qty_realout)->data(Qt::EditRole);
        db2[":qtyremain"] = v;
        db2[":qtydiff"] = ui->tbl->item(row, col_qty_diff)->data(Qt::EditRole);
        db2.exec("call sp_inventorization_row_update(:date1, :date2, :store, :goods, :qtyout, :qtyremain, :qtydiff)");
        break;
    case col_qty_output:
        auto *r = new FReportFromStoredProc();
        db2[":goods"] = ui->tbl->item(row, col_goods_id)->data(Qt::EditRole);
        db2[":store"]= ui->leStore->fHiddenText.toInt();
        db2[":date1"] = ui->date1->date();
        db2[":date2"] = ui->date2->date();
        db2.exec("call sp_view_output_dishes(:goods, :store, :date1, :date2)");
        db2.exec("select dishname as `Ծառայություն`, goodsname as `Ապրանք`,"
                 "recipeqty as `Բաղադրատոմս`, soldqty as `Վաճառք`, outqty as `Ելք` from rep");
        r->tbl(db2);
        r->fTitle = ui->tbl->item(row, col_goods_name)->data(Qt::EditRole).toString();
        fMainWindow->addTabWidget(r);
        break;
    }
}

void Inventorization::on_btnWriteOutput_clicked()
{
    bool emptydoc = true;
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        if (ui->tbl->item(i, col_qty_realout)->data(Qt::EditRole).toDouble() > 0.001) {
            emptydoc = false;
            break;
        }
    }
    if (emptydoc) {
        message_error("Դատարկ փասթաթուղթ");
        return;
    }
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2.startTransaction();
    db2[":f_date"] = ui->date2->date();
    db2[":f_type"] = 3;
    db2[":f_state"] = 0;
    db2[":f_partner"] = 0;
    db2[":f_inv"] = "";
    db2[":f_invDate"] = QVariant();
    db2[":f_invdoc"] = ui->leInvDoc->asInt();
    db2[":f_amount"] = 0;
    db2[":f_remarks"] = QString("%1 %2 - %3, %4").arg("Ծախս վաճառքի հիման վրա", ui->date1->text(), ui->date2->text(), ui->leStore->text());
    db2[":f_op"] = Base::fPreferences.getLocal(def_working_user_id);
    db2[":f_fullDate"] = QDateTime::currentDateTime();
    db2[":f_payment"] = 1;
    db2[":f_rest"] = 0;
    int docid = 0;
    db2.insert("r_docs", docid);
    for (int i = 0; i < ui->tbl->rowCount(); i++) {
        if (ui->tbl->item(i, col_qty_realout)->data(Qt::EditRole).toDouble() < 0.001) {
            continue;
        }
        db2[":f_doc"] = docid;
        db2[":f_store"] =  ui->leStore->fHiddenText.toInt();
        db2[":f_material"] = ui->tbl->item(i, col_goods_id)->data(Qt::EditRole);
        db2[":f_sign"] = -1;
        db2[":f_qty"] = ui->tbl->item(i, col_qty_realout)->data(Qt::EditRole);
        db2[":f_price"] = 0;
        db2[":f_total"] = 0;
        db2.insert("r_body");
    }
    db2[":docid"] = docid;
    db2.exec("call sp_store_output(:docid)");
    db2.exec("select * from ret");
    if (db2.next()) {

    } else {
        db2.rollback();
        message_error("Տեղի է ունեցել սխալ");
        return;
    }
    db2.commit();
    on_btnLoad_clicked();
}

void Inventorization::on_btnRemoveOutput_clicked()
{
    Db b = Preferences().getDatabase(Base::fDbName);
    Database2 db2;
    db2.open(b.dc_main_host, b.dc_main_path, b.dc_main_user, b.dc_main_pass);
    db2.startTransaction();
    db2[":date1"] = ui->date1->date();
    db2[":date2"] = ui->date2->date();
    db2[":store"] = ui->leStore->fHiddenText.toInt();
    db2.exec("SELECT f_id FROM r_inventorization "
            "WHERE f_datestart=:date1 AND f_dateend = :date2 AND f_store=:store");
    if (db2.next()) {
        db2[":f_invdoc"] = db2.integer("f_id");
        db2.exec("select f_id from r_docs where f_invdoc=:f_invdoc");
        if (db2.next()) {
            db2[":docid"] = db2.integer("f_id");
            db2.exec("call sp_remove_store_doc(:docid)");
            db2.exec("select * from ret");
            if (db2.next()) {
                QString msg = db2.string("f_result");
                db2.rollback();
                message_error("Օգտագործված է հետևյալ փասթաթղթերի մեջ՝\r\n" + msg);
                return;
            }
        }
    }
    db2.commit();
    on_btnLoad_clicked();
}
