#include "dlgonetwoall.h"
#include "ui_dlgonetwoall.h"

DlgOneTwoAll::DlgOneTwoAll(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOneTwoAll)
{
    ui->setupUi(this);
    fResult = 0;
}

DlgOneTwoAll::~DlgOneTwoAll()
{
    delete ui;
}

int DlgOneTwoAll::getOption(const QString &title, const QString &all, const QString &one, const QString &two, QWidget *parent)
{
    auto *d = new DlgOneTwoAll(parent);
    d->ui->lbText->setText(title);
    d->ui->rbAll->setText(all);
    d->ui->rbOne->setText(one);
    d->ui->rbTwo->setText(two);
    int result = 0;
    if (d->exec() == QDialog::Accepted) {
        result = d->fResult;
    }
    d->deleteLater();
    return result;
}

void DlgOneTwoAll::on_btnCancel_clicked()
{
    fResult = 0;
    reject();
}

void DlgOneTwoAll::on_btnOk_clicked()
{
    accept();
}

void DlgOneTwoAll::on_rbAll_clicked()
{
    fResult = 1;
}

void DlgOneTwoAll::on_rbOne_clicked()
{
    fResult = 2;
}

void DlgOneTwoAll::on_rbTwo_clicked()
{
    fResult = 3;
}