#include "dlgroom.h"
#include "ui_dlgroom.h"

DlgRoom::DlgRoom(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_ROOM, parent),
    ui(new Ui::DlgRoom)
{
    ui->setupUi(this);
    fDockCat = new DWSelectorRoomCategory(this);
    fDockCat->configure();
    fDockCat->setSelector(ui->leClassCode);
    connect(fDockCat, SIGNAL(roomCategory(CI_RoomCategory*)), this, SLOT(roomCategory(CI_RoomCategory*)));
    fDockBed = new DWSelectorBed(this);
    fDockBed->configure();
    fDockBed->setSelector(ui->leBedCode);
    connect(fDockBed, SIGNAL(bed(CI_Bed*)), this, SLOT(bed(CI_Bed*)));
    fDockView = new DWSelectorRoomView(this);
    fDockView->configure();
    fDockView->setSelector(ui->leViewCode);
    connect(fDockView, SIGNAL(roomView(CI_RoomView*)), this, SLOT(roomView(CI_RoomView*)));
    ui->btnOK->setText(tr("New"));
    fWidgets << ui->leCode
             << ui->leClassCode
             << ui->leClassName
             << ui->leViewCode
             << ui->leViewName
             << ui->leFloor
             << ui->leShortName
             << ui->leDescription
             << ui->leRate
             << ui->leBedCode
             << ui->leBedName
             << ui->leQty
             << ui->leQueue
             << ui->chSmoking;
}

DlgRoom::~DlgRoom()
{
    delete ui;
}

void DlgRoom::setValues()
{
    ui->leCode->clear();
    ui->leClassCode->clear();
    ui->leClassName->clear();
    ui->leViewCode->clear();
    ui->leViewName->clear();
    ui->leFloor->clear();
    ui->leShortName->clear();
    ui->leDescription->clear();
    ui->leRate->clear();
    ui->leBedCode->clear();
    ui->leBedName->clear();
    ui->leQty->clear();
    ui->leQueue->clear();
    ui->chSmoking->setChecked(false);
    if (fValues.count() > 0) {
        ui->leCode->setReadOnly(true);
        valuesToWidgets();
        ui->btnOK->setText(tr("Edit"));
        isNew = false;
    }
}

void DlgRoom::roomCategory(CI_RoomCategory *c)
{
    dockResponse<CI_RoomCategory, CacheRoomCategory>(ui->leClassCode, ui->leClassName, c);
}

void DlgRoom::roomView(CI_RoomView *c)
{
    dockResponse<CI_RoomView, CacheRoomView>(ui->leViewCode, ui->leViewName, c);
}

void DlgRoom::bed(CI_Bed *c)
{
    dockResponse<CI_Bed, CacheBed>(ui->leBedCode, ui->leBedName, c);
}

void DlgRoom::on_btnCancel_clicked()
{
    reject();
}

void DlgRoom::on_btnOK_clicked()
{
    fDbBind[":f_id"] = ui->leCode->text();
    fDbBind[":f_class"] = ui->leClassCode->text();
    fDbBind[":f_floor"] = ui->leFloor->text();
    fDbBind[":f_short"] = ui->leShortName->text();
    fDbBind[":f_description"] = ui->leDescription->text();
    fDbBind[":f_rate"] = ui->leRate->text();
    fDbBind[":f_bed"] = ui->leBedCode->text();
    fDbBind[":f_bedQty"] = ui->leQty->text();
    fDbBind[":f_queue"] = ui->leQueue->text();
    fDbBind[":f_smoke"] = (int) ui->chSmoking->isChecked();
    if (isNew) {
        QMap<QString, QVariant> e;
        fDb.select("select f_id from f_room where f_id=" + ui->leCode->text(), e, fDbRows);
        if (fDbRows.count() > 0) {
            message_error_tr("The room with selected code exists");
            return;
        }
        fDb.insert("f_room", fDbBind);
    } else {
        fDb.update("f_room", fDbBind, "where f_id=" + ui->leCode->text());
    }
    widgetsToValues();
    accept();
}
