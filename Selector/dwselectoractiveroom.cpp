#include "dwselectoractiveroom.h"

DWSelectorActiveRoom::DWSelectorActiveRoom(QWidget *parent) :
    DWSelector(cid_active_room, parent)
{
    setWindowTitle(tr("Room"));
    connect(CacheActiveRoom::instance(), SIGNAL(cacheUpdated(QString)), this, SLOT(cacheUpdated(QString)));
}

void DWSelectorActiveRoom::configure()
{
    CacheActiveRoom *c = CacheActiveRoom::instance();
    fTable->setColumnCount(4);
    fTable->clearContents();
    fTable->setRowCount(0);
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 120, 100, 250);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Room"), tr("Room name"), tr("Invoice"), tr("Guest"));
    adjustWidth();
    QMapIterator<QString, CI_ActiveRoom*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        CI_ActiveRoom *v = i.value();
        if (!v->fValid) {
            continue;
        }
        row = fTable->rowCount();
        fTable->setRowCount(row + 1);
        fTable->setItem(row, 0, new QTableWidgetItem(v->fRoomCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fRoomName));
        fTable->setItem(row, 2, new QTableWidgetItem(v->fInvoice));
        fTable->setItem(row, 3, new QTableWidgetItem(v->fGuestName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
    }
}

void DWSelectorActiveRoom::select(const QVariant &value)
{
    CI_ActiveRoom *ci = static_cast<CI_ActiveRoom*>(value.value<CI_ActiveRoom*>());
    emit roomChanged(ci);
}

void DWSelectorActiveRoom::selectorFocusOut()
{
    CI_ActiveRoom *ci = 0;
    if (fLineSelector) {
        ci = CacheActiveRoom::instance()->get(fLineSelector->text());
    }
    emit roomChanged(ci);
}



