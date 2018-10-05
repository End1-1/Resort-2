#ifndef FINVOICES_H
#define FINVOICES_H

#include "wfilterbase.h"

namespace Ui {
class FInvoices;
}

class WReportGrid;

class FInvoices : public WFilterBase
{
    Q_OBJECT

public:
    explicit FInvoices(QWidget *parent = 0);
    ~FInvoices();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    QCheckBox *chFreeRooming();
    static void openReport(bool free);
private:
    Ui::FInvoices *ui;
private slots:
    void removeInvoiceWithAllReference();
    void on_btnNext_clicked();
    void on_btnPrev_clicked();
};

#endif // FINVOICES_H
