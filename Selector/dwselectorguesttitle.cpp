#include "dwselectorguesttitle.h"

DWSelectorGuestTitle::DWSelectorGuestTitle(QWidget *parent) :
    DWSelector(cid_guest_title, parent)
{
    setWindowTitle(tr("Guest title"));
}

void DWSelectorGuestTitle::configure()
{
    CacheGuestTitle *c = CacheGuestTitle::instance();
    fTable->setColumnCount(1);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 100);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_GuestTitle*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        CI_GuestTitle *v = i.value();
        fTable->setItem(row, 0, new QTableWidgetItem(v->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
}

void DWSelectorGuestTitle::select(const QVariant &value)
{
    CI_GuestTitle *g = value.value<CI_GuestTitle*>();
    emit guestTitle(g);
}

void DWSelectorGuestTitle::selectorFocusOut()
{
    emit guestTitle(0);
}
