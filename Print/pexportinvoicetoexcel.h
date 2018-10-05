#ifndef PEXPORTINVOICETOEXCEL_H
#define PEXPORTINVOICETOEXCEL_H

#include "base.h"
#include <QString>

class PExportInvoiceToExcel : public QObject, public Base
{
    Q_OBJECT
public:
    PExportInvoiceToExcel();
    static void exportInvoice(const QString &invoice, int side);
private:
    void run(const QString &invoice, int fSide);
};

#endif // PEXPORTINVOICETOEXCEL_H
