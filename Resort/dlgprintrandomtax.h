#ifndef DLGPRINTRANDOMTAX_H
#define DLGPRINTRANDOMTAX_H

#include "baseextendeddialog.h"
#include "dwselectorinvoiceitem.h"

namespace Ui {
class DlgPrintRandomTax;
}

class DlgPrintRandomTax : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPrintRandomTax(QWidget *parent = 0);
    ~DlgPrintRandomTax();
    void setHeader(const QString &room, const QString &guest, const QString &invoice);
private:
    Ui::DlgPrintRandomTax *ui;
    DWSelectorInvoiceItem *fDockItem;
    void countTotal();
    void correctAmounts(EQLineEdit *l1, EQLineEdit *l2, EQLineEdit *l3);
private slots:
    void invoiceItem(CI_InvoiceItem *c);
    void on_btnAdd_clicked();
    void btnClickWithTag(int tag);
    void on_leCash_textEdited(const QString &arg1);
    void on_btnPrint_clicked();
    void on_leCard_textEdited(const QString &arg1);
    void on_btnRetryPrint_clicked();
    void on_lePrepaiment_textEdited(const QString &arg1);
    void on_leItemQty_textEdited(const QString &arg1);
};

#endif // DLGPRINTRANDOMTAX_H
