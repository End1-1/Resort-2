#ifndef FDAILYMOVEMENT_H
#define FDAILYMOVEMENT_H

#include "wfilterbase.h"
#include "dwselectorinvoiceitem.h"
#include "godaily.h"

namespace Ui {
class FDailyMovement;
}

class WReportGrid;

class FDailyMovement : public WFilterBase
{
    Q_OBJECT

public:
    explicit FDailyMovement(QWidget *parent = 0);
    ~FDailyMovement();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual GOWidget *gridOptionWidget();
    static void open();
private:
    Ui::FDailyMovement *ui;
    void setupTab();
    DWSelectorInvoiceItem *fDockItem;
    void processPayments(QList<QVariant> &emptyRow);
    void processItems(QString items);
private slots:
    void invoiceItem(CI_InvoiceItem *c);
    void openInvoice();
    void on_btnPrevDate_clicked();
    void on_btnNextDate_clicked();
};

#endif // FDAILYMOVEMENT_H
