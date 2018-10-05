#ifndef FONLINEREST_H
#define FONLINEREST_H

#include "wfilterbase.h"

namespace Ui {
class FOnlineRest;
}

class FOnlineRest : public WFilterBase
{
    Q_OBJECT

public:
    explicit FOnlineRest(QWidget *parent = 0);
    ~FOnlineRest();
    virtual QWidget *firstElement();
    virtual QString reportTitle();
    virtual void apply(WReportGrid *rg);
    static void openOnlineRestaurant();
private slots:
    void doubleClickOnRow(const QList<QVariant> &row);
    void on_tblFilter_clicked(const QModelIndex &index);

private:
    Ui::FOnlineRest *ui;
    int fCurrentHall;
};

#endif // FONLINEREST_H
