#ifndef FRESERVEBYCREATE_H
#define FRESERVEBYCREATE_H

#include "wfilterbase.h"

namespace Ui {
class FReserveByCreate;
}

class FReserveByCreate : public WFilterBase
{
    Q_OBJECT

public:
    explicit FReserveByCreate(QWidget *parent = 0);
    ~FReserveByCreate();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void openReport();
private slots:
    void doubleClick(const QList<QVariant> &row);
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::FReserveByCreate *ui;
};

#endif // FRESERVEBYCREATE_H
