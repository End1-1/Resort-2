#ifndef FGIFTCART_H
#define FGIFTCART_H

#include "wfilterbase.h"

namespace Ui {
class FGiftCart;
}

class FGiftCart : public WFilterBase
{
    Q_OBJECT

public:
    explicit FGiftCart(QWidget *parent = 0);
    ~FGiftCart();
    virtual QString reportTitle() override;
    virtual QWidget *firstElement() override;
    virtual void apply(WReportGrid *rg) override;
    bool isSummaryMode() const;

private slots:
    void showCardUsage();
    void onRowDoubleClick(const QList<QVariant> &row);

private:
    void loadFilters();
    void applyDetailed(WReportGrid *rg);
    void applySummary(WReportGrid *rg);
    QString filterSql() const;

    Ui::FGiftCart *ui;
    bool fFiltersLoaded;
};

#endif // FGIFTCART_H
