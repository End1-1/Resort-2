#include "dwselectordishmod.h"

DWSelectorDishMod::DWSelectorDishMod(QWidget *parent) :
    DWSelector(cid_rest_dish_mod, parent)
{
    setWindowTitle(tr("Dish modifiers"));
}

void DWSelectorDishMod::configure()
{
    CacheRestDishMod *c = CacheRestDishMod::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestDishMod*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorDishMod::select(const QVariant &value)
{
    CI_RestDishMod *m = value.value<CI_RestDishMod*>();
    emit dishMod(m);
}

void DWSelectorDishMod::selectorFocusOut()
{
    emit dishMod(0);
}
