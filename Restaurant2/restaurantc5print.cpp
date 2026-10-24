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
    printer.setFullPage(false);
    QRectF pr = printer.pageRect(QPrinter::DevicePixel);
    qreal dpi = qMax<qreal>(1.0, printer.logicalDpiX());
    const qreal pageMm = pr.width() * 25.4 / dpi;

    if(pageMm > 100.0) {
        printer.setPageSize(QPageSize(QSizeF(80, 2000), QPageSize::Millimeter));
        pr = printer.pageRect(QPrinter::DevicePixel);
        dpi = qMax<qreal>(1.0, printer.logicalDpiX());
    }

    constexpr qreal kSafeRightMm = 6.0;
    const qreal safePx = kSafeRightMm * dpi / 25.4;
    const qreal width96 = qMax<qreal>(40.0, (pr.width() - safePx) * 96.0 / dpi);
    const qreal height96 = qMax<qreal>(400.0, pr.height() * 96.0 / dpi);
    doc.reset();
    doc.setSceneParams(width96, height96, 96.0);
    doc.setRightMarginMm(sideMarginMm);
}

bool printC5(C5Printing &doc, QPrinter &printer)
{
    return doc.print(printer);
}
