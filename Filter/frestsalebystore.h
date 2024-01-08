#ifndef FRESTSALEBYSTORE_H
#define FRESTSALEBYSTORE_H

#include "wfilterbase.h"

namespace Ui {
class FRestSaleByStore;
}

class FRestSaleByStore : public WFilterBase
{
    Q_OBJECT

public:
    explicit FRestSaleByStore(QWidget *parent = 0);
    ~FRestSaleByStore();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
private slots:
    void on_btnNext_clicked();

    void on_btnBack_clicked();

    void branchEditDoubleClick(bool v);
    void hallEditDoubleClick(bool v);

private:
    Ui::FRestSaleByStore *ui;
};

#endif // FRESTSALEBYSTORE_H
