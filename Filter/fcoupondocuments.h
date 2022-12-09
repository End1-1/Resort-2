#ifndef FCOUPONDOCUMENTS_H
#define FCOUPONDOCUMENTS_H

#include "wfilterbase.h"

namespace Ui {
class FCouponDocuments;
}

class ReportQuery;

class FCouponDocuments : public WFilterBase
{
    Q_OBJECT

public:
    explicit FCouponDocuments(QWidget *parent = nullptr);
    ~FCouponDocuments();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();

private:
    Ui::FCouponDocuments *ui;
    ReportQuery *fReportQuery;

private slots:
    void doubleClickOnRow(const QList<QVariant> &values);
    void newDoc();
    void returnDoc();
    void paymentDoc();
    void on_cbTemplate_currentIndexChanged(int index);
};

#endif // FCOUPONDOCUMENTS_H
