#include "dlgroomview.h"
#include "ui_dlgroomview.h"

DlgRoomView::DlgRoomView(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_ROOM_VIEW, parent),
    ui(new Ui::DlgRoomView)
{
    ui->setupUi(this);
    fWidgets << ui->leCode
             << ui->leShortName
             << ui->leDescrAM
             << ui->leDescrEN
             << ui->leDescrRU;
    fTable = "f_room_view";
}

DlgRoomView::~DlgRoomView()
{
    delete ui;
}

void DlgRoomView::on_btnCancel_clicked()
{
    reject();
}

void DlgRoomView::on_btnOk_clicked()
{
    save();
}
