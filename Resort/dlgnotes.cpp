#include "dlgnotes.h"
#include "ui_dlgnotes.h"

DlgNotes::DlgNotes(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_NOTE, parent),
    ui(new Ui::DlgNotes)
{
    ui->setupUi(this);
    ui->wdSchedule->setVisible(false);
    fDockGuest = new DWSelectorGuest(this);
    fDockGuest->configure();
    fDockGuest->setSelector(ui->leGuestCode);
    connect(fDockGuest, SIGNAL(guest(CI_Guest*)), this, SLOT(guest(CI_Guest*)));
    fDockGroup = new DWSelectorUsersGroups(this);
    fDockGroup->configure();
    fDockGroup->setSelector(ui->leGroupCode);
    connect(fDockGroup, SIGNAL(userGroup(CI_UsersGroups*)), this, SLOT(usersGroup(CI_UsersGroups*)));
    fDockRoom = new DWSelectorRoom(this);
    fDockRoom->configure();
    fDockRoom->setSelector(ui->leRoomCode);
    connect(fDockRoom, SIGNAL(room(CI_Room*)), this, SLOT(room(CI_Room*)));
}

DlgNotes::~DlgNotes()
{
    delete ui;
}

void DlgNotes::setValues()
{
    ui->leCode->clear();
    ui->leRoomCode->clear();
    ui->leRoomName->clear();
    ui->deStart->setDateTime(QDateTime::currentDateTime());
    ui->deInterval->setTime(QTime::fromString("00:00:00", "HH:mm:ss"));
    ui->leGuestCode->clear();
    ui->leGuestName->clear();
    ui->leNationality->clear();
    ui->teText->clear();
    ui->leAuthor->setText(WORKING_USERNAME);
    if (fValues.count() > 0) {
        ui->leCode->setText(fValues.at(0).toString());
        ui->cbState->setIndexForData(fValues.at(1).toInt());
        ui->leRoomCode->setText(fValues.at(3).toString());
        ui->leRoomName->setText(fValues.at(4).toString());
        ui->leGroupCode->setText(fValues.at(5).toString());
        ui->leGroupName->setText(fValues.at(5).toString());
        ui->deStart->setDateTime(fValues.at(6).toDateTime());
        ui->deInterval->setTime(fValues.at(7).toTime());
        ui->teText->setPlainText(fValues.at(8).toString());
        ui->leGuestCode->setText(fValues.at(9).toString());
        ui->leGuestName->setText(fValues.at(10).toString());
        ui->leNationality->setText(fValues.at(11).toString());
        ui->leAuthor->setText(fValues.at(12).toString());
    }
}

void DlgNotes::setScheduleVisible(bool value)
{
    ui->wdSchedule->setVisible(value);
    ui->buttonBox->setVisible(!value);
}

void DlgNotes::guest(CI_Guest *c)
{
    dockResponse<CI_Guest, CacheGuest>(ui->leGuestCode, ui->leGuestName, c);
}

void DlgNotes::room(CI_Room *c)
{
    dockResponse<CI_Room, CacheRoom>(ui->leRoomCode, ui->leRoomName, c);
}

void DlgNotes::usersGroup(CI_UsersGroups *c)
{
    dockResponse<CI_UsersGroups, CacheUsersGroups>(ui->leGroupCode, ui->leGroupName, c);
}

void DlgNotes::on_buttonBox_accepted()
{
    if (ui->wdSchedule->isVisible()) {
        message_info_tr("You must schedule or dissmis the task");
        return;
    }
    int code = ui->leCode->text().toInt();
    QMap<QString, QVariant> v;
    v[":f_state"] = ui->cbState->asInt();
    v[":f_room"] = ui->leRoomCode->text().toInt();
    v[":f_group"] = ui->leGroupCode->asInt();
    v[":f_dateStart"] = ui->deStart->dateTime();
    v[":f_interval"] = ui->deInterval->time();
    v[":f_text"] = ui->teText->toPlainText();
    v[":f_guest"] = ui->leGuestCode->text().toInt();
    if (code == 0) {
        v[":f_author"] = WORKING_USERID;
        code = fDb.insert("f_reminder", v);
        ui->leCode->setText(QString::number(code));
    } else {
        fDb.update("f_reminder", v, "where f_id=" + ui->leCode->text());
    }
    fValues.clear();
    fValues << ui->leCode->text()
            << ui->cbState->asInt()
            << ui->cbState->currentText()
            << ui->leRoomCode->text()
            << ui->leRoomName->text()
            << ui->leGroupCode->text()
            << ui->leGroupName->text()
            << ui->deStart->dateTime()
            << ui->deInterval->time()
            << ui->teText->toPlainText()
            << ui->leGuestCode->text()
            << ui->leGuestName->text()
            << ui->leNationality->text()
            << ui->leAuthor->text();

    accept();
}

void DlgNotes::on_btnSchedule_clicked()
{
    fDbBind[":f_state"] = 1;
    fDbBind[":f_dateLastComplete"] = QDateTime::currentDateTime();
    fDb.update("f_reminder", fDbBind, "where f_id=" + ui->leCode->text());
    reject();
}

void DlgNotes::on_btnDissmiss_clicked()
{
    fDbBind[":f_state"] = 2;
    fDbBind[":f_dateLastComplete"] = QDateTime::currentDateTime();
    fDb.update("f_reminder", fDbBind, "where f_id=" + ui->leCode->text());
    reject();
}

void DlgNotes::on_buttonBox_rejected()
{
    if (ui->wdSchedule->isVisible()) {
        message_info_tr("You must schedule or dissmis the task");
        return;
    }
}
