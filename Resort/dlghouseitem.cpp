#include "dlghouseitem.h"
#include "ui_dlghouseitem.h"
#include "cacheroominventorystate.h"
#include "dwselectorroom.h"

#define SEL_ROOM_IN 1

DlgHouseItem::DlgHouseItem(QWidget *parent) :
    BaseExtendedDialog(parent),
    ui(new Ui::DlgHouseItem)
{
    ui->setupUi(this);
    DWSelectorRoom *droom = new DWSelectorRoom(this);
    droom->configure();
    droom->setSelector(ui->leRoomCode);
    connect(droom, &DWSelectorRoom::room, [this](CI_Room *ci){
        dockResponse<CI_Room, CacheRoom>(ui->leRoomCode, ui->leRoomName, ci);
        loadRoom();
    });

    fDockInventory = new DWSelectorRoomInventory(this);
    fDockInventory->configure();
    fDockInventory->setDialog(this, SEL_ROOM_IN);

    Utils::tableSetColumnWidths(ui->tblMain, ui->tblMain->columnCount(),
                                0, 300, 150, 120, 300);

    fleInventory = 0;
}

DlgHouseItem::~DlgHouseItem()
{
    delete ui;
}

void DlgHouseItem::selector(int selectorNumber, const QVariant &value)
{
    switch (selectorNumber) {
    case SEL_ROOM_IN: {
        if (!fleInventory) {
            return;
        }
        dockResponse<CI_RoomInventory, CacheRoomInventory>(fleInventory, value.value<CI_RoomInventory*>());
    }
    }
}

void DlgHouseItem::openWindow()
{
    DlgHouseItem *d = new DlgHouseItem(fPreferences.getDefaultParentForMessage());
    d->exec();
    delete d;
}

void DlgHouseItem::leFocusOut()
{
    fDockInventory->setSelector(0);
}

void DlgHouseItem::leFocusIn()
{
    fleInventory = static_cast<EQLineEdit*>(sender());
    fDockInventory->setSelector(fleInventory);
}

void DlgHouseItem::comboStateCurrentIndexChanged(int index)
{
    Q_UNUSED(index)
    bool ok = true;
    for (int i = 0; i < ui->tblMain->rowCount(); i++) {
        ok = ok && ui->tblMain->comboBox(i, 2)->currentData().toInt() == 1;
    }
    if (ok) {
        ui->lbStatus->setPixmap(QPixmap(":/images/ball-green.png"));
    } else {
        ui->lbStatus->setPixmap(QPixmap(":/images/ball-red.png"));
    }
}

void DlgHouseItem::loadRoom()
{
    ui->tblMain->clearContents();
    ui->tblMain->setRowCount(0);
    fDockInventory->setSelector(0);
    DatabaseResult dr;
    fDbBind[":f_room"] = ui->leRoomCode->asInt();
    dr.select(fDb, "select * from f_room_inventory_journal where f_room=:f_room", fDbBind);
    for (int i = 0; i < dr.rowCount(); i++) {
        int row = addRow();
        ui->tblMain->setItemWithValue(row, 0, dr.value("f_id"));
        ui->tblMain->lineEdit(row, 1)->fHiddenText = dr.value("f_inventory").toString();
        CI_RoomInventory *ci = CacheRoomInventory::instance()->get(dr.value("f_inventory").toString());
        dockResponse<CI_RoomInventory, CacheRoomInventory>(ui->tblMain->lineEdit(row, 1), ci);
        ui->tblMain->comboBox(row, 2)->setIndexForData(dr.value("f_state").toInt());
        ui->tblMain->dateEdit(row, 3)->setDate(dr.value("f_date").toDate());
        ui->tblMain->lineEdit(row, 4)->setText(dr.value("f_comment").toString());
    }
    comboStateCurrentIndexChanged(0);
}

int DlgHouseItem::addRow()
{
    int row = ui->tblMain->rowCount();
    ui->tblMain->setRowCount(row + 1);
    ui->tblMain->setItemWithValue(row, 0, 0);
    EQLineEdit *l = ui->tblMain->addLineEdit(row, 1, false);
    connect(l, SIGNAL(focusIn()), this, SLOT(leFocusIn()));
    connect(l, SIGNAL(focusOut()), this, SLOT(leFocusOut()));
    EQComboBox *c = ui->tblMain->addComboBox(row, 2);
    c->setCache(cid_room_inventory_state);
    connect(c, SIGNAL(currentIndexChanged(int)), this, SLOT(comboStateCurrentIndexChanged(int)));
    ui->tblMain->addDateEdit(row, 3, false);
    ui->tblMain->addLineEdit(row, 4, false);
    l->setFocus();
    return row;
}

void DlgHouseItem::on_btnAddItem_clicked()
{
    addRow();
}

void DlgHouseItem::on_btnSave_clicked()
{
    for (int i = 0; i < ui->tblMain->rowCount(); i++) {
        if (ui->tblMain->lineEdit(i, 1)->fHiddenText.toInt() == 0) {
            message_error(tr("Incomplete document"));
            return;
        }
    }
    for (int i = 0; i < ui->tblMain->rowCount(); i++) {
        if (ui->tblMain->item(i, 0)->data(Qt::EditRole).toInt() == 0) {
            fDbBind[":f_id"] = 0;
            ui->tblMain->setItemWithValue(i, 0, fDb.insert("f_room_inventory_journal", fDbBind));
        }
        fDbBind[":f_room"] = ui->leRoomCode->asInt();
        fDbBind[":f_inventory"] = ui->tblMain->lineEdit(i, 1)->fHiddenText.toInt();
        fDbBind[":f_state"] = ui->tblMain->comboBox(i, 2)->currentData();
        fDbBind[":f_date"] = ui->tblMain->dateEdit(i, 3)->date();
        fDbBind[":f_comment"] = ui->tblMain->lineEdit(i, 4)->text();
        fDb.update("f_room_inventory_journal", fDbBind, where_id(ui->tblMain->item(i, 0)->data(Qt::EditRole).toInt()));
    }
}

void DlgHouseItem::on_btnRemoveItem_clicked()
{
    int row = ui->tblMain->currentRow();
    ui->tblMain->removeRow(row);
}
