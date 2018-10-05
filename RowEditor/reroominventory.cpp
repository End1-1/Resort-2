#include "reroominventory.h"
#include "ui_reroominventory.h"
#include "wreportgrid.h"
#include "cacheroominventory.h"

RERoomInventory::RERoomInventory(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_ROOM_INVENTORY, parent),
    ui(new Ui::RERoomInventory)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "f_room_inventory";
    fCacheId = cid_room_inventory;
}

RERoomInventory::~RERoomInventory()
{
    delete ui;
}

void RERoomInventory::openRoomInventoryReport()
{
    QList<int> widths;
    widths << 100
           << 400;
    QStringList fields;
    fields << "f_id"
           << "f_name";
    QStringList titles;
    titles << tr("Code")
           << tr("Name");
    QString title = tr("Room inventory");
    QString icon = ":/images/configure.png";
    QString query = "select f_id, f_name from f_room_inventory";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERoomInventory>(widths, fields, titles, title, icon, query);
}

void RERoomInventory::on_btnCancel_clicked()
{
    reject();
}

void RERoomInventory::on_btnSave_clicked()
{
    save();
}
