#include "dwselectorroomcategory.h"

DWSelectorRoomCategory::DWSelectorRoomCategory(QWidget *parent) :
    DWSelector(cid_room_category, parent)
{
    setWindowTitle(tr("Room category"));
}

void DWSelectorRoomCategory::configure()
{
    CacheRoomCategory *c = CacheRoomCategory::instance();
    fTable->setColumnCount(3);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 120, 250);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Short"), tr("Description"));
    adjustWidth();
    QMapIterator<QString, CI_RoomCategory*> it = c->it();
    int row = 0;
    while (it.hasNext()) {
        it.next();
        CI_RoomCategory *c = it.value();
        fTable->setItem(row, 0, new QTableWidgetItem(c->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(c->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(c->fFullName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(c));
        row++;
    }
}

void DWSelectorRoomCategory::select(const QVariant &value)
{
    emit roomCategory(value.value<CI_RoomCategory*>());
}

void DWSelectorRoomCategory::selectorFocusOut()
{
    emit roomCategory(0);
}
