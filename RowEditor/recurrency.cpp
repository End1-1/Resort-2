#include "recurrency.h"
#include "ui_recurrency.h"
#include "cachecurrencies.h"

RECurrency::RECurrency(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_CURRENCY, parent),
    ui(new Ui::RECurrency)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leShort, "Short");
    addWidget(ui->leDescription, "Description");
    addWidget(ui->leRate, "Rate");
    fTable = "f_acc_currencies";
    fCacheId = cid_currencies;
}

RECurrency::~RECurrency()
{
    delete ui;
}

void RECurrency::on_btnCancel_clicked()
{
    reject();
}

void RECurrency::on_btnOk_clicked()
{
    save();
}
