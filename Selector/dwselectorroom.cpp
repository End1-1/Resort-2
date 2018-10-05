#include "dwselectorroom.h"

DWSelectorRoom::DWSelectorRoom(QWidget *parent) :
    DWSelector(cid_room, parent)
{
    setWindowTitle(tr("Room"));
}

void DWSelectorRoom::configure()
{
    CacheRoom *c = CacheRoom::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_Room*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorRoom::select(const QVariant &value)
{
    emit room(value.value<CI_Room*>());
}

void DWSelectorRoom::selectorFocusOut()
{
    emit room(0);
}
