#include "dwselectorreservestatus.h"

DWSelectorReserveStatus::DWSelectorReserveStatus(QWidget *parent) :
    DWSelector(cid_reserve_status, parent)
{
    setWindowTitle(tr("Reservation status"));
}

void DWSelectorReserveStatus::configure()
{
    CacheReserveStatus *c = CacheReserveStatus::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_ReserveStatus*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorReserveStatus::select(const QVariant &value)
{
    CI_ReserveStatus *r = value.value<CI_ReserveStatus*>();
    emit reserveStatus(r);
}

void DWSelectorReserveStatus::selectorFocusOut()
{
    emit reserveStatus(0);
}
