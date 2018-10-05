#include "dlggroupreserveoption.h"
#include "ui_dlggroupreserveoption.h"
#include "dwselectorgroupreservation.h"
#include "cacheguest.h"
#include "trackcontrol.h"
#include "message.h"

DlgGroupReserveOption::DlgGroupReserveOption(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::DlgGroupReserveOption)
{
    ui->setupUi(this);
    DWSelectorGroupReservation *dockGroup = new DWSelectorGroupReservation(this);
    dockGroup->configure();
    dockGroup->setSelector(ui->leGroupCode);
    dockGroup->setDialog(this, cid_group_reservation);
}

DlgGroupReserveOption::~DlgGroupReserveOption()
{
    delete ui;
}

void DlgGroupReserveOption::addRemove(const QString &id)
{
    DlgGroupReserveOption *d = new DlgGroupReserveOption(fPreferences.getDefaultParentForMessage());
    DatabaseResult dr;
    d->fDbBind[":f_id"] = id;
    dr.select(d->fDb, "select * from f_reservation where f_id=:f_id", d->fDbBind);
    d->ui->leReservation->setText(id);
    d->ui->leRoom->setText(dr.value("f_room").toString());
    CI_Guest *g = CacheGuest::instance()->get(dr.value("f_guest").toString());
    d->ui->leGuest->setText(g->fName);
    d->ui->leGroupCode->setText(dr.value("f_group").toString());
    d->dockResponse<CI_GroupReservation, CacheGroupReservations>(d->ui->leGroupCode, d->ui->leGroupName, 0);
    d->exec();
    delete d;
}

void DlgGroupReserveOption::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case cid_group_reservation:
        dockResponse<CI_GroupReservation, CacheGroupReservations>(ui->leGroupCode, ui->leGroupName, value.value<CI_GroupReservation*>());
        break;
    }
}

void DlgGroupReserveOption::on_btnAddToGroup_clicked()
{
    fDbBind[":f_group"] = ui->leGroupCode->asInt();
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservation->text())));
    TrackControl::insert(TRACK_RESERVATION_GROUP, "Merging with group", ui->leGroupCode->text() + "/" + ui->leGroupName->text(), "", "-", "-", ui->leReservation->text());
    TrackControl::insert(TRACK_RESERVATION, "Merging with group", ui->leGroupCode->text() + "/" + ui->leGroupName->text(), "", "-", "-", ui->leReservation->text());
    accept();
}

void DlgGroupReserveOption::on_bgtnRemoveFromGroup_clicked()
{
    if (message_confirm(tr("Confirm removing from group")) != QDialog::Accepted) {
        return;
    }
    fDbBind[":f_group"] = 0;
    fDb.update("f_reservation", fDbBind, where_id(ap(ui->leReservation->text())));
    TrackControl::insert(TRACK_RESERVATION_GROUP, "Removed from group", ui->leGroupCode->text() + "/" + ui->leGroupName->text(), "", "-", "-", ui->leReservation->text());
    TrackControl::insert(TRACK_RESERVATION, "Removed from group", ui->leGroupCode->text() + "/" + ui->leGroupName->text(), "", "-", "-", ui->leReservation->text());
    accept();
}
