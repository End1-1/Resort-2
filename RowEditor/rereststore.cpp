#include "rereststore.h"
#include "ui_rereststore.h"
#include "cachereststore.h"

RERestStore::RERestStore(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_REST_STORE, parent),
    ui(new Ui::RERestStore)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "r_store";
    fCacheId = cid_rest_store;
}

RERestStore::~RERestStore()
{
    delete ui;
}

void RERestStore::valuesToWidgets()
{
    RowEditorDialog::valuesToWidgets();
    if (ui->leCode->asInt() > 0) {
        fDbBind[":f_id"] = ui->leCode->asInt();
        fDb.select("select f_state from r_store where f_id=:f_id", fDbBind, fDbRows);
        foreach_rows {
            ui->chActive->setChecked(it->at(0).toInt() == 1);
        }
    }
}

void RERestStore::save()
{
    RowEditorDialog::save();
    fDbBind[":f_state"] = ui->chActive->isChecked() ? 1 : 0;
    fDb.update("r_store", fDbBind, where_id(ui->leCode->asInt()));
}

void RERestStore::on_btnCancel_clicked()
{
    reject();
}

void RERestStore::on_btnOk_clicked()
{
    save();
}
