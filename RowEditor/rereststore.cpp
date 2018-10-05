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

void RERestStore::on_btnCancel_clicked()
{
    reject();
}

void RERestStore::on_btnOk_clicked()
{
    save();
}
