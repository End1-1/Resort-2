#include "dwselectorreststore.h"

DWSelectorRestStore::DWSelectorRestStore(QWidget *parent) :
    DWSelector(cid_rest_store, parent)
{
    setWindowTitle(tr("Store"));
}

void DWSelectorRestStore::configure()
{
    CacheRestStore *c = CacheRestStore::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestStore*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorRestStore::select(const QVariant &value)
{
    CI_RestStore *s = value.value<CI_RestStore*>();
    emit store(s);
}

void DWSelectorRestStore::selectorFocusOut()
{
    emit store(0);
}
