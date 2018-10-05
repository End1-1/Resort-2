#include "dlgdate.h"
#include "ui_dlgdate.h"

DlgDate::DlgDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDate)
{
    ui->setupUi(this);
    ui->leDate->setMinimumWidth(200);
    ui->leDate->setMaximumWidth(400);
}

DlgDate::~DlgDate()
{
    delete ui;
}

bool DlgDate::getDate(QDate &date)
{
    bool result = false;
    DlgDate *d = new DlgDate();
    if (d->exec() == QDialog::Accepted) {
        result = true;
        date = d->ui->leDate->date();
    }
    delete d;
    return result;
}

void DlgDate::on_btnCancel_clicked()
{
    reject();
}

void DlgDate::on_btnOk_clicked()
{
    accept();
}
