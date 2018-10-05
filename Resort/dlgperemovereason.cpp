#include "dlgperemovereason.h"
#include "ui_dlgperemovereason.h"

DlgPERemoveReason::DlgPERemoveReason(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgPERemoveReason)
{
    ui->setupUi(this);
}

DlgPERemoveReason::~DlgPERemoveReason()
{
    delete ui;
}

void DlgPERemoveReason::on_btnCancel_clicked()
{
    reject();
}

void DlgPERemoveReason::on_btnVoid_clicked()
{
    done(DISH_STATE_REMOVED_STORE);
}

void DlgPERemoveReason::on_btnMistake_clicked()
{
    done(DISH_STATE_REMOVED_NOSTORE);
}
