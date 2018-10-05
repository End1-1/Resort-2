#include "dwselectorreservationcardex.h"

DWSelectorReservationCardex::DWSelectorReservationCardex(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Cardex"));
}

void DWSelectorReservationCardex::configure()
{
    CacheReservation *c = CacheReservation::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 400);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_Reservation*> i = c->it();
    QMap<QString, CI_Reservation*> proxy;
    while (i.hasNext()) {
        i.next();
        proxy.insert(i.value()->fCardex, i.value());
    }
    int row = 0;
    foreach (QString ca, proxy.keys()) {
        fTable->setItem(row, 0, new QTableWidgetItem(proxy[ca]->fCardex));
        fTable->setItem(row, 1, new QTableWidgetItem(proxy[ca]->fCardexName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(proxy[ca]));
        row++;
    }
    fTable->setRowCount(row);
}
