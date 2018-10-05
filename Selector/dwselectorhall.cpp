#include "dwselectorhall.h"

DWSelectorHall::DWSelectorHall(QWidget *parent) :
    DWSelector(cid_rest_hall, parent)
{
    setWindowTitle(tr("Hall"));
}

void DWSelectorHall::configure()
{
    CacheRestHall *c = CacheRestHall::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestHall*> it = c->it();
    int row = 0;
    while (it.hasNext()) {
        it.next();
        CI_RestHall *v = it.value();
        if (!fFilterList.isEmpty()) {
            if (!fFilterList.contains(v->fCode)) {
                continue;
            }
        }
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
    fTable->setRowCount(row);
}

void DWSelectorHall::select(const QVariant &value)
{
    CI_RestHall *r = value.value<CI_RestHall*>();
    emit hall(r);
}

void DWSelectorHall::selectorFocusOut()
{
    emit hall(0);
}
