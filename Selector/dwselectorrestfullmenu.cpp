#include "dwselectorrestfullmenu.h"

DWSelectorRestFullMenu::DWSelectorRestFullMenu(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Dishes"));
}

void DWSelectorRestFullMenu::configure()
{
    CacheRestFullMenu *c = CacheRestFullMenu::instance();
    fTable->setColumnCount(5);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 0, 200, 0, 300, 80);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Menu"), tr("Code"), tr("Dish"), tr("Price"));
    adjustWidth();
    QMapIterator<QString, CI_RestFullMenu*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
         CI_RestFullMenu *ci = i.value();
        if (!ci->fValid) {
            continue;
        }
        if (!fFilterList.isEmpty()) {
            if (!fFilterList.contains(ci->fCode)) {
                continue;
            }
        }
        if (!otherFilter(ci)) {
            continue;
        }
        fTable->setItem(row, 0, new QTableWidgetItem(ci->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(ci->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(QString::number(ci->fDishCode)));
        fTable->setItem(row, 3, new QTableWidgetItem(ci->fDishName[def_lang]));
        fTable->setItem(row, 4, new QTableWidgetItem(float_str(ci->fPrice, 0)));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(ci));
        row++;
    }
    fTable->setRowCount(row);
}
