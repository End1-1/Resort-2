#include "dwselectorrestprinter.h"

DWSelectorRestPrinter::DWSelectorRestPrinter(QWidget *parent) :
    DWSelector(cid_rest_printers, parent)
{
    setWindowTitle(tr("Printers"));
}

void DWSelectorRestPrinter::configure()
{
    CacheRestPrinters *c = CacheRestPrinters::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_RestPrinter*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        fTable->setItem(row, 0, new QTableWidgetItem(i.value()->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(i.value()->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(i.value()));
        row++;
    }
}

void DWSelectorRestPrinter::select(const QVariant &value)
{
    CI_RestPrinter *p = value.value<CI_RestPrinter*>();
    emit printer(p);
}

void DWSelectorRestPrinter::selectorFocusOut()
{
    emit printer(0);
}
