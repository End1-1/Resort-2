#include "dlgreserveshortinfo.h"
#include "ui_dlgreserveshortinfo.h"
#include "dwselectorusers.h"
#include "message.h"

DlgReserveShortInfo::DlgReserveShortInfo(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgReserveShortInfo)
{
    ui->setupUi(this);

    DWSelectorUsers *dUser = new DWSelectorUsers(this);
    dUser->configure();
    dUser->setSelector(ui->leCheckoutUsercode);
    connect(dUser, &DWSelectorUsers::user, [this](CI_User *ci) {
       dockResponse<CI_User, CacheUsers>(ui->leCheckoutUsercode, ui->leCheckoutUserName, ci);
    });
}

DlgReserveShortInfo::~DlgReserveShortInfo()
{
    delete ui;
}

void DlgReserveShortInfo::loadShortInfo(const QString &id)
{
    DlgReserveShortInfo *d = new DlgReserveShortInfo(fPreferences.getDefaultParentForMessage());
    d->ui->leCode->setText(id);
    if (!d->loadInfo()) {
        delete d;
        return;
    }
    d->exec();
    delete d;
}

void DlgReserveShortInfo::on_btnCancel_clicked()
{
    reject();
}

void DlgReserveShortInfo::on_btnOK_clicked()
{
    fDbBind[":f_startDate"] = ui->deCheckin->date();
    fDbBind[":f_checkinDate"] = ui->deCheckin->date();
    fDbBind[":f_endDate"] = ui->deDeparture->date();
    fDbBind[":f_checkInTime"] = ui->teCheckin->time();
    fDbBind[":f_checkOutTime"] = ui->teCheckout->time();
    fDbBind[":f_checkOutUser"] = ui->leCheckoutUsercode->asInt();
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leCode->text())));
    fDbBind[":f_wdate"] = ui->deDeparture->date();
    fDbBind[":f_rdate"] = ui->deDeparture->date();
    fDbBind[":f_time"] = ui->teCheckout->time();
    fDbBind[":f_inv"] = ui->leInvoice->text();
    fDb.select("update m_register set f_wdate=:f_wdate, f_rdate=:f_rdate, f_time=:f_time where f_source='CO' and f_inv=:f_inv", fDbBind, fDbRows);
    message_info_tr("Saved");
}

bool DlgReserveShortInfo::loadInfo()
{
    DatabaseResult dr;
    fDbBind[":f_id"] = ui->leCode->text();
    dr.select(fDb, "select * from f_reservation where f_id=:f_id", fDbBind);
    if (dr.rowCount() == 0) {
        return false;
    }
    ui->leInvoice->setText(dr.value("f_invoice").toString());
    ui->leRoom->setText(dr.value("f_room").toString());
    ui->deCheckin->setDate(dr.value("f_startDate").toDate());
    ui->deDeparture->setDate(dr.value("f_endDate").toDate());
    ui->teCheckin->setTime(dr.value("f_checkInTime").toTime());
    ui->teCheckout->setTime(dr.value("f_checkOutTime").toTime());
    ui->leCheckoutUsercode->setInt(dr.value("f_checkOutUser").toInt());
    dockResponse<CI_User, CacheUsers>(ui->leCheckoutUsercode, ui->leCheckoutUserName, 0);
    return true;
}
