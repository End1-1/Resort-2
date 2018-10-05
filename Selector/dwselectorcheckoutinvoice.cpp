#include "dwselectorcheckoutinvoice.h"

DWSelectorCheckoutInvoice::DWSelectorCheckoutInvoice(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("Checkout invoices"));
}

void DWSelectorCheckoutInvoice::configure()
{
    CacheCheckoutInvoice *c = CacheCheckoutInvoice::instance();
    fTable->setColumnCount(7);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(),
                                80, 80, 200, 120, 120, 120, 200);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
                                  tr("Code"), tr("Room"), tr("Guest"),
                                  tr("Entry"), tr("Departure"), tr("Amount"), tr("Operator"));
    adjustWidth();
    QMapIterator<QString, CI_CheckoutInvoice*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            continue;
        }
        CI_CheckoutInvoice *v = i.value();
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fRoom));
        fTable->setItem(row, 2, new QTableWidgetItem(v->fGuest));
        fTable->setItem(row, 3, new QTableWidgetItem(v->fEntry));
        QTableWidgetItem *itemDep = new QTableWidgetItem();
        itemDep->setData(Qt::DisplayRole, v->fDeparture);
        fTable->setItem(row, 4, itemDep);
        fTable->setItem(row, 5, new QTableWidgetItem(v->fAmount));
        fTable->setItem(row, 6, new QTableWidgetItem(v->fOperator));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
    fTable->setRowCount(row);
    fTable->sortByColumn(4, Qt::DescendingOrder);
}

CI_CheckoutInvoice *DWSelectorCheckoutInvoice::nextRow(int &row)
{
    if (row < fTable->rowCount() - 1) {
        row++;
    } else {
        return 0;
    }
    if (row > fTable->rowCount() - 1) {
        return 0;
    }
    return fTable->item(row, 0)->data(Qt::UserRole).value<CI_CheckoutInvoice*>();
}

CI_CheckoutInvoice *DWSelectorCheckoutInvoice::prevRow(int &row)
{
    if (row > 0) {
        row--;
    } else {
        return 0;
    }
    if (row > fTable->rowCount() - 1) {
        return 0;
    }
    return fTable->item(row, 0)->data(Qt::UserRole).value<CI_CheckoutInvoice*>();
}
