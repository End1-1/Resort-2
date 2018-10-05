#ifndef REROOMCATEGORYRATE_H
#define REROOMCATEGORYRATE_H

#include "recustomaction.h"

namespace Ui {
class RERoomCategoryRate;
}

class WReportGrid;

class RERoomCategoryRate : public RECustomAction
{
    Q_OBJECT

public:
    explicit RERoomCategoryRate(WReportGrid *parent);
    ~RERoomCategoryRate();

protected slots:
    virtual void handleValues(const QList<QVariant> &out);

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnAddRate_clicked();
    void on_btnRemoveRate_clicked();
    void on_btnEditRate_clicked();

private:
    Ui::RERoomCategoryRate *ui;
    int fCategory;
    void load();
};

#endif // REROOMCATEGORYRATE_H
