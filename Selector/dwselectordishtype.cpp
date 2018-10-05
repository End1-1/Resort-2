#include "dwselectordishtype.h"

DWSelectorDishType::DWSelectorDishType(QWidget *parent) :
    DWSelector(cid_rest_dish_type, parent)
{
    setWindowTitle(tr("Dish type"));
}

void DWSelectorDishType::configure()
{
    CacheRestDishType *c = CacheRestDishType::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestDishType*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorDishType::select(const QVariant &value)
{
    CI_RestDishType *d = value.value<CI_RestDishType*>();
    emit dishType(d);
}

void DWSelectorDishType::selectorFocusOut()
{
    emit dishType(0);
}
