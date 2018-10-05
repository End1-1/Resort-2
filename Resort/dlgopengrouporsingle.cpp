#include "dlgopengrouporsingle.h"
#include "ui_dlgopengrouporsingle.h"

DlgOpenGroupOrSingle::DlgOpenGroupOrSingle(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgOpenGroupOrSingle)
{
    ui->setupUi(this);
}

DlgOpenGroupOrSingle::~DlgOpenGroupOrSingle()
{
    delete ui;
}

int DlgOpenGroupOrSingle::choice()
{
    DlgOpenGroupOrSingle *d = new DlgOpenGroupOrSingle(fPreferences.getDefaultParentForMessage());
    int result = d->exec();
    delete d;
    return result;
}

void DlgOpenGroupOrSingle::on_btnOpenSingle_clicked()
{
    reject();
}

void DlgOpenGroupOrSingle::on_btnOpenGroup_clicked()
{
    accept();
}
