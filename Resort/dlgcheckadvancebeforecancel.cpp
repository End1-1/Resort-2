#include "dlgcheckadvancebeforecancel.h"
#include "ui_dlgcheckadvancebeforecancel.h"
#include "dlgnoshow.h"
#include "message.h"
#include "dlgrefundvaucher.h"
#include "dlgcityadvance.h"

DlgCheckAdvanceBeforeCancel::DlgCheckAdvanceBeforeCancel(const QString &inv, QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgCheckAdvanceBeforeCancel)
{
    ui->setupUi(this);
    fInvoice = inv;
    DatabaseResult dr;
    fDbBind[":f_invoice"] = inv;
    dr.select(fDb, "select r.f_id, r.f_room, g.guest from f_reservation r "
              "left join guests g on g.f_id=r.f_guest "
              "where r.f_invoice=:f_invoice", fDbBind);
    if (dr.rowCount() > 0) {
        ui->leReservation->setText(dr.value("f_id").toString());
        ui->leGuest->setText(dr.value("guest").toString());
        ui->leRoom->setText(dr.value("f_room").toString());
        ui->leInvoice->setText(inv);
    }
}

DlgCheckAdvanceBeforeCancel::~DlgCheckAdvanceBeforeCancel()
{
    delete ui;
}

int DlgCheckAdvanceBeforeCancel::checkAdvance(const QString &inv)
{
    DlgCheckAdvanceBeforeCancel *d = new DlgCheckAdvanceBeforeCancel(inv, fPreferences.getDefaultParentForMessage());
    d->getBalance();
    int result = d->exec();
    delete d;
    return result;
}

bool DlgCheckAdvanceBeforeCancel::getBalance()
{
    fDbBind[":f_inv"] = fInvoice;
    DatabaseResult dr;
    dr.select(fDb, "select sum(f_amountAmd*f_sign*-1) as amount from m_register where f_inv=:f_inv "
              "and f_finance=1 and f_canceled=0", fDbBind);
    if (dr.rowCount() == 0) {
        ui->leBalance->setText("0");
        return false;
    }
    ui->leBalance->setDouble(dr.value("amount").toDouble());
    ui->btnCancelreservation->setEnabled(ui->leBalance->asDouble() < 0.01);
    ui->btnRefund->setEnabled(ui->leBalance->asDouble() > 0.01);
    ui->btnTransferToCL->setEnabled(ui->leBalance->asDouble() > 0.01);
    ui->btnNoShow->setEnabled(r__(cr__reservation_cancelation_no_show));
    return true;
}

void DlgCheckAdvanceBeforeCancel::on_btnCancel_clicked()
{
    done(CR_NOCANCEL);
}

void DlgCheckAdvanceBeforeCancel::on_btnNoShow_clicked()
{
    DlgNoShow *d = new DlgNoShow(fPreferences.getDefaultParentForMessage());
    d->setReservation(ui->leReservation->text());
    d->exec();
    delete d;
    getBalance();
}

void DlgCheckAdvanceBeforeCancel::on_btnRefund_clicked()
{
    DlgRefundVaucher::refundVaucher("", ui->leReservation->text());
    getBalance();
}

void DlgCheckAdvanceBeforeCancel::on_btnCancelreservation_clicked()
{
    if (message_confirm(tr("Confirm to cancel reservation")) == RESULT_YES)  {
        done(CR_CANCEL);
    }
}

void DlgCheckAdvanceBeforeCancel::on_btnTransferToCL_clicked()
{
    DlgCityAdvance::cityAdvance("", ui->leInvoice->text(), 1);
    getBalance();
}
