#ifndef DLGVIEWINVOICECORRECTIONS_H
#define DLGVIEWINVOICECORRECTIONS_H

#include "baseextendeddialog.h"
namespace Ui {
class DlgViewInvoiceCorrections;
}

class DlgViewInvoiceCorrections : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgViewInvoiceCorrections(const QString &invoice, QWidget *parent = 0);
    ~DlgViewInvoiceCorrections();

private:
    Ui::DlgViewInvoiceCorrections *ui;
};

#endif // DLGVIEWINVOICECORRECTIONS_H
