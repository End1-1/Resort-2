#include "dbdialog.h"
#include "ui_dbdialog.h"

DbDialog::DbDialog() :
    Dialog(),
    ui(new Ui::DbDialog)
{
    ui->setupUi(this);
    fWidget = nullptr;
}

DbDialog::~DbDialog()
{
    delete ui;
}

void DbDialog::on_btnCancel_clicked()
{
    reject();
}

void DbDialog::setWidget(WdbWidget *w)
{
    fWidget = w;
    ui->vl->addWidget(static_cast<QWidget *>(w));
    adjustSize();
}

void DbDialog::on_btnAdd_clicked()
{
    if (fWidget->save(fWidget)) {
        accept();
    }
}
