#include "reservewidgetaction.h"
#include "ui_reservewidgetaction.h"
#include "dlgraiseuser.h"

ReserveWidgetAction::ReserveWidgetAction(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::ReserveWidgetAction)
{
    ui->setupUi(this);
    fResult = 0;
    fReservation = 0;
}

ReserveWidgetAction::~ReserveWidgetAction()
{
    delete ui;
}

void ReserveWidgetAction::setAuthor(const QString &text)
{
    ui->leAuthor->setText(text);
}

void ReserveWidgetAction::setRemarks(const QString &text)
{
    ui->teRemarks->setPlainText(text);
}

void ReserveWidgetAction::setReservation(CI_Reservation *r)
{
    fReservation = r;
    ui->deStart->setDate(r->fDateStart);
    ui->deEnd->setDate(r->fDateEnd);
    ui->btnEdit->setEnabled(r->fState == RESERVE_SERVICE);
}

void ReserveWidgetAction::lock(int lock)
{
    fLock = lock;
    ui->btnLock->setEnabled(lock == 0);
    ui->btnUnlock->setEnabled(lock > 0);
    ui->btnEdit->setEnabled(lock == 0 || lock == WORKING_USERID);
    ui->btnRemove->setEnabled(lock == 0 || WORKING_USERID);
    ui->teRemarks->setEnabled(lock == 0 || lock == WORKING_USERID);
}

QString ReserveWidgetAction::remarks()
{
    return ui->teRemarks->toPlainText();
}

void ReserveWidgetAction::on_btnCancel_clicked()
{
    reject();
}

void ReserveWidgetAction::on_btnEdit_clicked()
{
    result(1);
}

void ReserveWidgetAction::result(int r)
{
    if (fLock > 0) {
        if (WORKING_USERID != fLock) {
            int raiseUser = DlgRaiseUser::raiseUser(cr__unlock_locked_reservation);
            if (raiseUser == 0) {
                message_error_tr("Accses denied");
                return;
            }
        }
    }
    fResult = r;
    accept();
}

void ReserveWidgetAction::on_btnLock_clicked()
{
    result(2);
}

void ReserveWidgetAction::on_btnUnlock_clicked()
{
    result(3);
}

void ReserveWidgetAction::on_btnRemove_clicked()
{
    result(4);
}

void ReserveWidgetAction::on_btnOk_clicked()
{
    result(0);
}

void ReserveWidgetAction::on_btnOutOfOrder_clicked()
{
    result(5);
}

void ReserveWidgetAction::on_btnOutOfInventory_clicked()
{
    result(6);
}
