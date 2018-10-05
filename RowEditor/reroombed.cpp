#include "reroombed.h"
#include "ui_reroombed.h"

RERoomBed::RERoomBed(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_ROOM_BED, parent),
    ui(new Ui::RERoomBed)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "f_room_bed";
}

RERoomBed::~RERoomBed()
{
    delete ui;
}

void RERoomBed::on_btnCancel_clicked()
{
    reject();
}

void RERoomBed::on_btnOk_clicked()
{
    fDbBind[":f_id"] = ui->leCode->text();
    fDbBind[":f_name"] = ui->leName->text();
    if (isNew) {
        fDb.insert("f_room_bed", fDbBind);
    } else {
        fDb.update("f_room_bed", fDbBind, where_id(fTrackControl->oldValue(ui->leCode)));
    }
    fTrackControl->saveChanges();  //TODO tracking by qstring
    accept();
}
