#ifndef DWSELECTORINVOICEITEM_H
#define DWSELECTORINVOICEITEM_H

#include "dwselector.h"
#include "cacheinvoiceitem.h"

class DWSelectorInvoiceItem : public DWSelector
{
    Q_OBJECT
public:
    DWSelectorInvoiceItem(QWidget *parent);
    virtual void configure();
    void setWidgets(EQLineEdit *id, EQLineEdit *code, EQLineEdit *name);
    void setFilter(const QString &filter);
    void setSourceFilter(const QString &filter);
    int fGroup;
protected:
    virtual void select(const QVariant &value);
    virtual void selectorFocusOut();
private:
   QStringList fFilter;
   QString fFilterSource;
   EQLineEdit *fId;
   EQLineEdit *fCode;
   EQLineEdit *fName;
private slots:
   void codeFocusOut();
signals:
   void invoiceItem(CI_InvoiceItem *c);
};

#endif // DWSELECTORINVOICEITEM_H
