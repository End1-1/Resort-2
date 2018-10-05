#ifndef FACCMONTHLYREPORT_H
#define FACCMONTHLYREPORT_H

#include "wfilterbase.h"

namespace Ui {
class FAccMonthlyReport;
}

class FAccMonthlyReport : public WFilterBase
{
    Q_OBJECT

public:
    explicit FAccMonthlyReport(QWidget *parent = 0);
    ~FAccMonthlyReport();
    virtual void apply(WReportGrid *rg);
    virtual QWidget *firstElement();
    virtual QString reportTitle();
private slots:
    void on_deStart_dateChanged(const QDate &date);

    void on_deEnd_dateChanged(const QDate &date);

    void on_cbYear_currentIndexChanged(const QString &arg1);

    void on_cbMonth_currentIndexChanged(const QString &arg1);

private:
    Ui::FAccMonthlyReport *ui;
    void setTabText();
    void setDateRange();
    void getInvoiceContent(QList<QList<QVariant> > &rows, QMap<QString, int> &dateMap, int col, const QString &item);
    void getGPOSContent(QList<QList<QVariant> > &rows, QMap<QString, int> &dateMap, int col, const QString &store);
};

#endif // FACCMONTHLYREPORT_H
