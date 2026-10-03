#ifndef RESTAURANTC5PRINT_H
#define RESTAURANTC5PRINT_H

#include "c5printing.h"
#include <QPrinter>
#include <QString>

/** Design-time point size → actual print size (Restaurant2 receipts). */
constexpr int kReceiptFontScale = 2;

inline int receiptFontPt(int designPt)
{
    return designPt * kReceiptFontScale;
}

/** Receipt/report printer: custom page, full bleed; content scaled in C5Printing::print(). */
class ReceiptPrinter {
public:
    explicit ReceiptPrinter(const QString &printerName);
    QPrinter &printer() { return m_printer; }

private:
    QPrinter m_printer;
};

void setupC5Printing(C5Printing &doc, QPrinter &printer, qreal sideMarginMm = 4.0);

bool printC5(C5Printing &doc, QPrinter &printer);

#endif // RESTAURANTC5PRINT_H
