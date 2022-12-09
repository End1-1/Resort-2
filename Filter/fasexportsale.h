#ifndef FASEXPORTSALE_H
#define FASEXPORTSALE_H

#include "wfilterbase.h"

namespace Ui {
class FAsExportSale;
}

class ReportQuery;

class FAsExportSale : public WFilterBase
{
    Q_OBJECT

public:
    explicit FAsExportSale(QWidget *parent = nullptr);
    ~FAsExportSale();
    virtual void apply(WReportGrid *rg) override;
    virtual QWidget *firstElement() override;

private:
    Ui::FAsExportSale *ui;
    ReportQuery *fReportQuery;

private slots:
    void branchEditDoubleClick(bool v);
};

#endif // FASEXPORTSALE_H
