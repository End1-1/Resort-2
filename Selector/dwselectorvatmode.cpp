#include "dwselectorvatmode.h"

DWSelectorVATMode::DWSelectorVATMode(QWidget *parent) :
    DWSelector(cid_vat_mode, parent)
{
    setWindowTitle(tr("VAT mode"));
}

void DWSelectorVATMode::configure()
{
    CacheVatMode *c = CacheVatMode::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 120);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_VATMode*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        CI_VATMode *v = i.value();
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
}

void DWSelectorVATMode::select(const QVariant &value)
{
    CI_VATMode *v = value.value<CI_VATMode*>();
    emit vatMode(v);
}

void DWSelectorVATMode::focusOut()
{
    CI_VATMode *v= 0;
    if (fLineSelector) {
        v = CacheVatMode::instance()->get(fLineSelector->text());
    }
    emit vatMode(v);
}
