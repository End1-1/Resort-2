#include "dlgreserveconfirmstatus.h"
#include "ui_dlgreserveconfirmstatus.h"

DlgReserveConfirmStatus::DlgReserveConfirmStatus(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReserveConfirmStatus)
{
    ui->setupUi(this);
}

DlgReserveConfirmStatus::~DlgReserveConfirmStatus()
{
    delete ui;
}

void DlgReserveConfirmStatus::on_btnCancel_clicked()
{
    done(0);
}

void DlgReserveConfirmStatus::on_btnConfirmed_clicked()
{
    done(CONFIRM_CONFIRM);
}

void DlgReserveConfirmStatus::on_btnGuaranted_clicked()
{
    done(CONFIRM_GUARANTEED);
}

void DlgReserveConfirmStatus::on_btnTentative_clicked()
{
    done(CONFIRM_TENTATIVE);
}

void DlgReserveConfirmStatus::on_btnWaiting_clicked()
{
    done(CONFIRM_WAITING);
}

void DlgReserveConfirmStatus::on_btnBlock_clicked()
{
    done(CONFIRM_BLOCK);
}
