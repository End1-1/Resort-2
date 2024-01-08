#ifndef FMATERIALSINSTORE_H
#define FMATERIALSINSTORE_H

#include "wfilterbase.h"
#include "dwselectorreststore.h"

namespace Ui {
class FMaterialsInStore;
}

class FMaterialsInStore : public WFilterBase
{
    Q_OBJECT

public:
    explicit FMaterialsInStore(QWidget *parent = 0);
    ~FMaterialsInStore();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);

private:
    Ui::FMaterialsInStore *ui;
    DWSelectorRestStore *fDockStore;

private slots:
    void store(CI_RestStore *c);
    void doubleClickOnRow(const QList<QVariant> &row);
    void goodsClick();
};

#endif // FMATERIALSINSTORE_H
