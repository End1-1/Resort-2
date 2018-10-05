#include "dwselectortrackingtables.h"

DWSelectorTrackingTables::DWSelectorTrackingTables(QWidget *parent) :
    DWSelector(cid_tracking_tables, parent)
{
    setWindowTitle(tr("Table"));
}

void DWSelectorTrackingTables::configure()
{
    CacheTrackingTables *c = CacheTrackingTables::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_TrackingTables*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        CI_TrackingTables *ct = i.value();
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(ct));
        row++;
    }
}

void DWSelectorTrackingTables::select(const QVariant &value)
{
    emit tracking(value.value<CI_TrackingTables*>());
}

void DWSelectorTrackingTables::selectorFocusOut()
{
    emit tracking(0);
}
