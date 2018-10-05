#include "deselectormenudishes.h"

DESelectorMenuDishes::DESelectorMenuDishes(QWidget *parent) :
    DWSelector(cid_menu_dishes, parent)
{
    setWindowTitle(tr("Menu dishes"));
    fMenu = "";
}

void DESelectorMenuDishes::configure()
{
    CacheMenuDishes *c = CacheMenuDishes::instance();
    fTable->setColumnCount(4);
    QList<CI_MenuDishes*> proxy;
    QMapIterator<QString, CI_MenuDishes*> it (c->it());
    while (it.hasNext()) {
        it.next();
        if (!fMenu.isEmpty()) {
            if (fMenu != it.value()->fMenuId) {
                continue;
            }
        }
        proxy.append(it.value());
    }
    fTable->setRowCount(proxy.count());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 120, 250, 350, 80);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Menu"), tr("Type"), tr("Dish"), tr("Price"));
    adjustWidth();
    QListIterator<CI_MenuDishes*> id(proxy);
    int row = 0;
    while (id.hasNext()) {
        CI_MenuDishes *c = id.next();
        fTable->setItem(row, 0, new QTableWidgetItem(c->fMenu));
        fTable->setItem(row, 1, new QTableWidgetItem(c->fTypeName));
        fTable->setItem(row, 2, new QTableWidgetItem(c->fName));
        fTable->setItem(row, 3, new QTableWidgetItem(c->fPrice));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(c));
        row++;
    }
}

void DESelectorMenuDishes::setMenu(const QString &menu)
{
    fMenu = menu;
    configure();
}

void DESelectorMenuDishes::select(const QVariant &value)
{
    emit menuDish(value.value<CI_MenuDishes*>());
}

void DESelectorMenuDishes::selectorFocusOut()
{
    emit menuDish(0);
}
