#include "dwselectorrestmenu.h"

DWSelectorRestMenu::DWSelectorRestMenu(QWidget *parent) :
    DWSelector(cid_rest_menu, parent)
{
    setWindowTitle(tr("Menu"));
}

void DWSelectorRestMenu::configure()
{
    CacheRestMenu *c = CacheRestMenu::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestMenu*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorRestMenu::select(const QVariant &value)
{
    CI_RestMenu *m = value.value<CI_RestMenu*>();
    emit menu(m);
}

void DWSelectorRestMenu::selectorFocusOut()
{
    CI_RestMenu *m = 0;
    if (fLineSelector) {
        m = CacheRestMenu::instance()->get(fLineSelector->text());
    }
    emit menu(m);
}
