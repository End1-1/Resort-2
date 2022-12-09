#include "dwselectorinvoiceitem.h"
#include "cacheinvoiceitem.h"

DWSelectorInvoiceItem::DWSelectorInvoiceItem(QWidget *parent) :
    DWSelector(cid_invoice_item, parent)
{
    setWindowTitle(tr("Type of sale"));
    fId = 0;
    fName = 0;
    fCode = 0;
    fGroup = 0;
}

void DWSelectorInvoiceItem::configure()
{
    CacheInvoiceItem *c = CacheInvoiceItem::instance();
    fTable->setColumnCount(2);
    fTable->setRowCount(c->elementsCount());

    Utils::tableSetColumnWidths(fTable, fTable->columnCount(), 80, 300);
    Utils::tableSetHeaderCaptions(fTable, fTable->columnCount(), tr("Code"), tr("Name"));
    adjustWidth();
    QMapIterator<QString, CI_InvoiceItem*> i = c->it();
    int row = 0;
    while (i.hasNext()) {
        i.next();
        CI_InvoiceItem *v = i.value();
        if (fGroup > 0) {
            if (v->fGroup != fGroup) {
                continue;
            }
        }
        if (!fFilter.isEmpty()) {
            if (!fFilter.contains(v->fCode)) {
                continue;
            }
        }
        if (!fFilterSource.isEmpty()) {
            if (v->fVaucher != fFilterSource) {
                continue;
            }
        }

        fTable->setItem(row, 0, new QTableWidgetItem(v->fCode));
        fTable->setItem(row, 1, new QTableWidgetItem(v->fName));
        fTable->item(row, 0)->setData(Qt::UserRole, qVariantFromValue(v));
        row++;
    }
    fTable->setRowCount(row);
}

void DWSelectorInvoiceItem::setWidgets(EQLineEdit *id, EQLineEdit *code, EQLineEdit *name)
{
    fId = id;
    fCode = code;
    fName = name;
    if (fCode) {
        connect(fCode, SIGNAL(customButtonClicked(bool)), this, SLOT(thisShow(bool)));
        connect(fCode, SIGNAL(focusOut()), this, SLOT(codeFocusOut()));
    }
}

void DWSelectorInvoiceItem::setFilter(const QString &filter)
{
    fFilter = filter.split(",", QString::SkipEmptyParts);
}

void DWSelectorInvoiceItem::setSourceFilter(const QString &filter)
{
    fFilterSource = filter;
}

void DWSelectorInvoiceItem::select(const QVariant &value)
{
    CI_InvoiceItem *ci = value.value<CI_InvoiceItem*>();
    if (fId) {
        fId->setText(ci->fCode);
    }
    if (fCode) {
        fCode->setText(ci->fCode);
    }
    if (fName) {
        fName->setText(ci->fName);
    }
    emit invoiceItem(ci);
}

void DWSelectorInvoiceItem::selectorFocusOut()
{
    CI_InvoiceItem *ci = 0;
    if (fLineSelector) {
        ci = CacheInvoiceItem::instance()->get(fLineSelector->text());
    }
    emit invoiceItem(ci);
}

void DWSelectorInvoiceItem::codeFocusOut()
{
    CI_InvoiceItem *ci = 0;
    if (fCode) {
        if (fCode->text().isEmpty()) {
            if (fId) {
                fId->clear();
            }
            if (fName) {
                fName->clear();
            }
        } else {
            ci = CacheInvoiceItem::instance()->get(fCode->text());
            if (ci) {
                if (fId) {
                    fId->setText(ci->fCode);
                }
                if (fName) {
                    fName->setText(ci->fName);
                }
            }
        }
    }
    emit invoiceItem(ci);
}
