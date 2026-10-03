#include "restaurantc5print.h"
#include <QPageSize>
#include <QPrinterInfo>

namespace {

QPrinterInfo receiptPrinterInfo(const QString &printerName)
{
    QPrinterInfo pi = QPrinterInfo::printerInfo(printerName);
    if(pi.isNull()) {
        return QPrinterInfo::defaultPrinter();
    }
    return pi;
}

} // namespace

ReceiptPrinter::ReceiptPrinter(const QString &printerName)
    : m_printer(receiptPrinterInfo(printerName))
{
    m_printer.setPageSize(QPageSize::Custom);
    m_printer.setFullPage(true);
}

void setupC5Printing(C5Printing &doc, QPrinter &printer, qreal sideMarginMm)
{
    doc.reset();
    doc.setSceneFromPrinter(printer);
    doc.setRightMarginMm(sideMarginMm);
}

bool printC5(C5Printing &doc, QPrinter &printer)
{
    return doc.print(printer);
}
