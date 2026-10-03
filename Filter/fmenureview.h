#ifndef FMENUREVIEW_H
#define FMENUREVIEW_H

#include "wfilterbase.h"

namespace Ui
{
class FMenuReview;
}

class FMenuReview : public WFilterBase
{
    Q_OBJECT
public:
    explicit FMenuReview(QWidget *parent = nullptr);
    ~FMenuReview();
    virtual QString reportTitle();
    virtual QWidget* firstElement();
    virtual void apply(WReportGrid *rg);
private slots:
    void viewModeChanged(int id);
    void doubleClickOnRow(const QList<QVariant> &values);
private:
    enum ViewMode {
        ViewRecipe = 0,
        ViewPrices = 1
    };

    QString menuFilterIds() const;
    void applyRecipe(WReportGrid *rg);
    void applyPrices(WReportGrid *rg);

    Ui::FMenuReview* ui;
    ViewMode fViewMode;
};

#endif // FMENUREVIEW_H
