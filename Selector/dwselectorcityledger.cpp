#include "dwselectorcityledger.h"

DWSelectorCityLedger::DWSelectorCityLedger(QWidget *parent) :
    DWSelector(cid_city_ledger, parent)
{
    setWindowTitle(tr("City ledger"));
}

void DWSelectorCityLedger::configure()
{
    CacheCityLedger *c = CacheCityLedger::instance();
    fTable->setColumnCount(4);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 250, 100, 120);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
        tr("Code"), tr("Name"), tr("VAT"), tr("Valid to"));
    adjustWidth();
    int row = 0;
    QMapIterator<QString, CI_CityLedger*> it = c->it();
    while (it.hasNext()) {
        it.next();
        CI_CityLedger *v = it.value();
        if (!v->fValid) {
            continue;
        }
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(v->fVatName));
        fTable->setItem(row, 3, new QTableWidgetItem(v->fCommission));
        fTable->setItem(row, 4, new QTableWidgetItem(v->fDateTo));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
    fTable->setRowCount(row);
}

void DWSelectorCityLedger::select(const QVariant &value)
{
    CI_CityLedger *c = value.value<CI_CityLedger*>();
    emit cityLedger(c);
}

void DWSelectorCityLedger::selectorFocusOut()
{
    CI_CityLedger *c = 0;
    if (fLineSelector) {
        c = CacheCityLedger::instance()->get(fLineSelector->text());
    }
    emit cityLedger(c);
}
