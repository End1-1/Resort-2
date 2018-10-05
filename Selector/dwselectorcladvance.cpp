#include "dwselectorcladvance.h"

DWSelectorCLAdvance::DWSelectorCLAdvance(QWidget *parent) :
    DWTemplateSelector(parent)
{
    setWindowTitle(tr("City ledger balance"));
}

void DWSelectorCLAdvance::configure()
{
    CacheCLAdvance *c = CacheCLAdvance::instance();
    fTable->setColumnCount(3);
    fTable->setRowCount(c->elementsCount());
    Utils::tableSetColumnWidths(fTable, fTable->columnCount(),
                                80, 200, 80);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(),
                                  tr("Code"), tr("Name"), tr("Amount"));
    adjustWidth();
    QMapIterator<QString, CI_CLAdvance*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        if (!i.value()->fValid) {
            continue;
        }
        CI_CLAdvance *v = i.value();
        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->setItem(row, 2, new QTableWidgetItem(float_str(v->fAmount, 0)));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
    fTable->setRowCount(row);
}
