#ifndef FSTOCKREMINDER_H
#define FSTOCKREMINDER_H

#include "wfilterbase.h"

namespace Ui
{
class FStockReminder;
}

class FStockReminder : public WFilterBase
{
    Q_OBJECT
public:
    explicit FStockReminder(QWidget *parent = nullptr);
    ~FStockReminder();
    virtual void apply(WReportGrid *rg);
    virtual QString reportTitle();
    virtual QWidget* firstElement();
private:
    Ui::FStockReminder* ui;
    QString fStore;
private slots:
    void doubleClickOnRow(int row, int column, const QList<QVariant>& values);
};

#endif // FSTOCKREMINDER_H
