#ifndef REINVOICEITEM_H
#define REINVOICEITEM_H

#include "roweditordialog.h"
#include "dwselectorinvoiceitemgroup.h"

namespace Ui {
class REInvoiceItem;
}

class REInvoiceItem : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit REInvoiceItem(QList<QVariant> &values, QWidget *parent = 0);
    ~REInvoiceItem();
    virtual void valuesToWidgets();
private slots:
    void invoiceGroup(CI_InvoiceGroup *c);
    void on_tnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::REInvoiceItem *ui;
    DWSelectorInvoiceItemGroup *fDockGroup;
    QMap<QString, int> fTaxMap;
};

#endif // REINVOICEITEM_H
