#include "dwselectordishpart.h"

DWSelectorDishPart::DWSelectorDishPart(QWidget *parent) :
    DWSelector(cid_rest_dish_part, parent)
{
    setWindowTitle(tr("Menu main partitions"));
}

void DWSelectorDishPart::configure()
{
    CacheRestDishPart *c = CacheRestDishPart::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestDishPart*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorDishPart::select(const QVariant &value)
{
    CI_RestDishPart *d = value.value<CI_RestDishPart*>();
    emit dishPart(d);
}

void DWSelectorDishPart::selectorFocusOut()
{
    emit dishPart(0);
}
