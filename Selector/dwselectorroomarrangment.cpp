#include "dwselectorroomarrangment.h"

DWSelectorRoomArrangment::DWSelectorRoomArrangment(QWidget *parent) :
    DWSelector(cid_room_arrangment, parent)
{
    setWindowTitle(tr("Room arrangment"));
}

void DWSelectorRoomArrangment::configure()
{
    CacheRoomArrangment *c = CacheRoomArrangment::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RoomArrangment*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorRoomArrangment::select(const QVariant &value)
{
    CI_RoomArrangment *c = value.value<CI_RoomArrangment*>();
    emit roomArrangment(c);
}

void DWSelectorRoomArrangment::selectorFocusOut()
{
    emit roomArrangment(0);
}
