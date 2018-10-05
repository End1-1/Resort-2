#include "dwselectordish.h"

DWSelectorDish::DWSelectorDish(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Dish"));
}

void DWSelectorDish::configure()
{
    CacheDish *c = CacheDish::instance();
    fTable->setColumnCount(3);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(),
                                80, 300, 80);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
                                  tr("Code"), tr("Name"), tr("Unit"));
    adjustWidth();
    QMapIterator<QString, CI_Dish*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            continue;
        }
        CI_Dish *v = i.value();
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(v->fUnitName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
    fTable->setRowCount(row);
}
