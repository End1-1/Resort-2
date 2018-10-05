#include "dwselectorguest.h"

DWSelectorGuest::DWSelectorGuest(QWidget *parent) :
    DWSelector(cid_guest, parent)
{
    setWindowTitle(tr("Guest"));
    connect(CacheGuest::instance(), SIGNAL(cacheUpdated(QString)), this, SLOT(cacheUpdated(QString)));
}

void DWSelectorGuest::configure()
{
    CacheGuest *cg = CacheGuest::instance();
    fTable->setColumnCount(5);
    fTable->setRowCount(cg->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 40, 200, 100, 80);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Title"), tr("Name"), tr("Passport"), tr("Nat."));
    adjustWidth();
    QMapIterator<QString, CI_Guest*> i = cg->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            return;
        }
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fTitle));
        fTable->setItem(row, 2, new QTableWidgetItem(i.value()->fName));
        fTable->setItem(row, 3, new QTableWidgetItem(i.value()->fPassport));
        fTable->setItem(row, 4, new QTableWidgetItem(i.value()->fNatShort));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
    fTable->setRowCount(row);
}

void DWSelectorGuest::select(const QVariant &value)
{
    emit guest(value.value<CI_Guest*>());
}

void DWSelectorGuest::selectorFocusOut()
{
    CI_Guest *g = 0;
    if (fLineSelector) {
        g = CacheGuest::instance()->get(fLineSelector->text());
    }
    emit guest(g);
}
