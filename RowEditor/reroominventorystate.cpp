#include "reroominventorystate.h"
#include "ui_reroominventorystate.h"
#include "wreportgrid.h"
#include "cacheroominventorystate.h"

RERoomInventoryState::RERoomInventoryState(QList<QVariant> &values, QWidget *parent) :
    RowEditorDialog(values, TRACK_ROOM_INVENTORY_STATE, parent),
    ui(new Ui::RERoomInventoryState)
{
    ui->setupUi(this);
    addWidget(ui->leCode, "Code");
    addWidget(ui->leName, "Name");
    fTable = "f_room_inventory_state";
    fCacheId = cid_room_inventory_state;
}

RERoomInventoryState::~RERoomInventoryState()
{
    delete ui;
}

void RERoomInventoryState::openReport()
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
    QString title = tr("Room inventory state");
    QString icon = ":/images/configure.png";
    QString query = "select f_id, f_name from f_room_inventory_state";
    WReportGrid *r = addTab<WReportGrid>();
    r->fullSetup<RERoomInventoryState>(widths, fields, titles, title, icon, query);
}

void RERoomInventoryState::on_btnCancel_clicked()
{
    reject();
}

void RERoomInventoryState::on_btnSave_clicked()
{
    save();
}
