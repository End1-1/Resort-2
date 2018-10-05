#include "dlgreservequickinfo.h"
#include "ui_dlgreservequickinfo.h"
#include "basewidget.h"
#include "winvoice.h"
#include "wreservation.h"
#include "dlggroupreservationfuck.h"

DlgReserveQuickInfo::DlgReserveQuickInfo(CI_Reservation *reservation, QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgReserveQuickInfo)
{
    ui->setupUi(this);
    ui->btnOpenInvoice->setEnabled(reservation->fState == RESERVE_CHECKIN);
    ui->btnGroup->setEnabled(reservation->fGroup > 0);
    ui->tblData->setColumnWidth(0, 200);
    ui->tblData->setColumnWidth(1, 400);
    fReservation = reservation;
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Room"), "#" + fReservation->fRoom + " [" + fReservation->fRoomShort + "]");
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Guest"), fReservation->fGuest);
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Create date"), fReservation->fDateCreate);
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Entry date"), fReservation->fDateStart.toString(def_date_format));
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Departure date"), fReservation->fDateEnd.toString(def_date_format));
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Total nights"), QString::number(fReservation->fDateStart.daysTo(fReservation->fDateEnd)));
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Nights left"), QString::number(WORKING_DATE.daysTo(fReservation->fDateEnd)));
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Cardex"), "[" + fReservation->fCardex + "] " + fReservation->fCardexName);
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Credit"), fReservation->fCredit);
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Debet"), fReservation->fDebet);
    Utils::tableAppendRowData(ui->tblData, ui->tblData->columnCount(), tr("Balance"), fReservation->fBalance);
}

DlgReserveQuickInfo::~DlgReserveQuickInfo()
{
    delete ui;
}

void DlgReserveQuickInfo::escapePressed()
{
    accept();
}

void DlgReserveQuickInfo::on_btnOpenInvoice_clicked()
{
    accept();
    WInvoice::openInvoiceWindow(fReservation->fInvoice);
}

void DlgReserveQuickInfo::on_btnOpenReserve_clicked()
{
    accept();
    WReservation::openReserveWindows(fReservation->fId);
}

void DlgReserveQuickInfo::on_btnGroup_clicked()
{
    accept();
    //WReservation::openReserveGroup(fReservation->fGroup);
    DlgGroupReservationFuck *d = addTab<DlgGroupReservationFuck>();
    d->loadGroup(fReservation->fGroup);
}
