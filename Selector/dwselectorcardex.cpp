#include "dwselectorcardex.h"

DWSelectorCardex::DWSelectorCardex(QWidget *parent) :
    DWSelector(cid_cardex, parent)
{
    setWindowTitle(tr("Cardex"));
}

void DWSelectorCardex::configure()
{
    CacheCardex *c = CacheCardex::instance();
    fTable->setColumnCount(5);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 250, 100, 100, 120);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"), tr("VAT"), tr("Commission"), tr("Valid to"));
    adjustWidth();
    QMapIterator<QString, CI_Cardex*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        CI_Cardex *v = i.value();
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(v->fVatName));
        fTable->setItem(row, 3, new QTableWidgetItem(v->fCommission));
        fTable->setItem(row, 4, new QTableWidgetItem(v->fDateTo));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
}

void DWSelectorCardex::select(const QVariant &value)
{
    CI_Cardex *c = value.value<CI_Cardex*>();
    emit cardex(c);
}

void DWSelectorCardex::selectorFocusOut()
{
    emit cardex(0);
}
