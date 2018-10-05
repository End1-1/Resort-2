#include "wguest.h"
#include "ui_wguest.h"
#include "cacheguest.h"

WGuest::WGuest(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_GUEST, parent),
    ui(new Ui::WGuest)
{
    ui->setupUi(this);
    ui->btnRemove->setVisible(r__(cr__super_correction));
    fDockTitle = new DWSelectorGuestTitle(this);
    fDockTitle->configure();
    fDockTitle->setSelector(ui->leTitle);
    connect(fDockTitle, SIGNAL(guestTitle(CI_GuestTitle*)), this, SLOT(guestTitle(CI_GuestTitle*)));
    fDockNation = new DWSelectorNation(this);
    fDockNation->configure();
    fDockNation->setSelector(ui->leNationalityCode);
    connect(fDockNation, SIGNAL(nation(CI_Nation*)), this, SLOT(nation(CI_Nation*)));
    addWidget(ui->leGuestCode, "Code")
            .addWidget(ui->leTitle, "Title")
            .addWidget(ui->leFirstname, "First name")
            .addWidget(ui->leLastname, "Last name")
            .addWidget(ui->cbSex, "Sex")
            .addWidget(ui->deBirth, "Birth date")
            .addWidget(ui->lePlaceBirth, "Place of birth")
            .addWidget(ui->leNationalityCode, "")
            .addWidget(ui->leNationality, "Nationality")
            .addWidget(ui->lePassport, "Passport")
            .addWidget(ui->leAddress, "Address")
            .addWidget(ui->leTel1, "Tel.1")
            .addWidget(ui->leTel2, "Tel.2")
            .addWidget(ui->leEmail, "Email")
            .addWidget(ui->lePostBox, "Postbox")
            .addWidget(ui->teRemarks, "Remarks");
    fTable = "f_guests";
    fCacheId = cid_guest;
}

WGuest::~WGuest()
{
    delete ui;
}


WGuest *WGuest::guest(QList<QVariant> &values, bool noCheckNation)
{
    WGuest *w = new WGuest(values, fPreferences.getDefaultParentForMessage());
    w->fNoCheckNation = noCheckNation;
    return w;
}

void WGuest::nation(CI_Nation *c)
{
    dockResponse<CI_Nation, CacheNation>(ui->leNationalityCode, ui->leNationality, c);
}

void WGuest::guestTitle(CI_GuestTitle *gt)
{
    if (gt) {
        ui->leTitle->setText(gt->fName);
    } else {
        gt = CacheGuestTitle::instance()->get(ui->leTitle->text());
        if (gt) {
            ui->leTitle->setText(gt->fName);
        } else {
            ui->leTitle->clear();
        }
    }
}

void WGuest::on_btnCancel_clicked()
{
    reject();
}

void WGuest::on_btnOk_clicked()
{
    DatabaseResult dr;
    if (!ui->lePassport->isEmpty()) {
        fDbBind[":f_passport"] = ui->lePassport->text();
        dr.select(fDb, "select f_id from f_guests where f_passport=:f_passport", fDbBind);
        if (dr.rowCount() > 0) {
            if (ui->leGuestCode->asInt() == 0) {
                message_error_tr("Duplicate passport record, the guest exists");
                return;
            }
            if (dr.value("f_id").toInt() != ui->leGuestCode->asInt()) {
                message_error_tr("Duplicate passport record, the guest exists");
                return;
            }
        }
    }
//    if (!fNoCheckNation) {
//        CI_Nation *n = CacheNation::instance()->get(ui->leNationalityCode->text());
//        if (!n) {
//            message_error_tr("Nationality not selected");
//            return;
//        }
//        if (n->fName == "-") {
//            message_error_tr("Nationality not selected");
//            return;
//        }
//    }

    save();
}

void WGuest::on_btnRemove_clicked()
{
    if (ui->leGuestCode->asInt() == 0) {
        return;
    }
    fDbBind[":f_guest"] = ui->leGuestCode->asInt();
    fDb.select("select f_id from f_reservation_guests where f_guest=:f_guest", fDbBind, fDbRows);
    if (fDbRows.count() > 0) {
        message_error_tr("Cannot remove this guest. Name in use");
        return;
    }
    fDbBind[":f_id"] = ui->leGuestCode->asInt();
    fDb.select("delete from f_guests where f_id=:f_id", fDbBind, fDbRows);
    fDb.fDb.commit();
    BroadcastThread::cmdRefreshCache(cid_guest, ui->leGuestCode->text());
    fTrackControl->insert("Guest removed", ui->leGuestCode->text() + " " + ui->leFirstname->text() + " " + ui->leLastname->text(), "");
    fTrackControl->saveChanges();
    message_info_tr("Guest was removed");
    reject();
}
