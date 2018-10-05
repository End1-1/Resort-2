#include "dwselectorstorepartners.h"

DWSelectorStorePartners::DWSelectorStorePartners(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Partners"));
}

void DWSelectorStorePartners::configure()
{
    CacheStorePartners *c = CacheStorePartners::instance();
    fTable->setColumnCount(4);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 100, 200, 100);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Tax code"), tr("Name"), tr("Phone"));
    adjustWidth();
    QMapIterator<QString, CI_StorePartners*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            continue;
        }
        if (!fFilterList.isEmpty()) {
            if (!fFilterList.contains(i.value()->fCode)) {
                continue;
            }
        }
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fTaxCode));
        fTable->setItem(row, 2, new QTableWidgetItem(i.value()->fName));
        fTable->setItem(row, 3, new QTableWidgetItem(i.value()->fPhone));
        CI_StorePartners *ct = i.value();
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(ct));
        row++;
    }
    fTable->setRowCount(row);
}
