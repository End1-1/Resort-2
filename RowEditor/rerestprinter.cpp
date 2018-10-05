#include "rerestprinter.h"
#include "ui_rerestprinter.h"
#include "cacherestprinters.h"

RERestPrinter::RERestPrinter(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_REST_PRINTERS, parent),
    ui(new Ui::RERestPrinter)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "r_printers";
    fCacheId = cid_rest_printers;
}

RERestPrinter::~RERestPrinter()
{
    delete ui;
}

void RERestPrinter::on_btnOk_clicked()
{
    save();
}

void RERestPrinter::on_btnCancel_clicked()
{
    reject();
}
