#ifndef FASEXPORTSALE_H
#define FASEXPORTSALE_H

#include "wfilterbase.h"
#include "asexportconstants.h"

namespace Ui {
class FAsExportSale;
}

class ReportQuery;
class QToolButton;

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
    ReportQuery *fTotalQuery;
    bool fHasSubType;
    AsExportConstants fConstants;
    QToolButton *fConfigButton;
    void initReportTypes();
    void loadReportType();
    void updateFilterVisibility();
    void applyCommon1(WReportGrid *rg);
    void applyCommon1Detail(WReportGrid *rg);
    void applyImportRetailInvoice(WReportGrid *rg);
    static void setupArmSoftColumns(WReportGrid *rg);

private slots:
    void branchEditDoubleClick(bool v);
    void hallEditDoubleClick(bool v);
    void storeEditDoubleClick(bool v);
    void reportTypeChanged(int index);
    void configConstants();
};

#endif // FASEXPORTSALE_H
