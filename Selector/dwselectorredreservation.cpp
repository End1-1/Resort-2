#include "dwselectorredreservation.h"

DWSelectorRedReservation::DWSelectorRedReservation(QWidget *parent) :
    DWSelector(cid_red_reservation, parent)
{
    setWindowTitle(tr("Reservation"));
}

void DWSelectorRedReservation::configure()
{
    CacheRedReservation *c = CacheRedReservation::instance();
    fTable->setColumnCount(4);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200, 100, 150);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Guest"), tr("Room"), tr("Balance"));
    adjustWidth();
    QMapIterator<QString, CI_RedReservation*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(i.value()->fRoom));
        fTable->setItem(row, 3, new QTableWidgetItem(i.value()->fBalance));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorRedReservation::select(const QVariant &value)
{
    emit redReservation(value.value<CI_RedReservation*>());
}

void DWSelectorRedReservation::selectorFocusOut()
{
    CI_RedReservation *c = 0;
    if (fLineSelector) {
        c = CacheRedReservation::instance()->get(fLineSelector->text());
    }
    emit redReservation(c);
}
