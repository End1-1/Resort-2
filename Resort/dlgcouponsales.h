#ifndef DLGCOUPONSALES_H
#define DLGCOUPONSALES_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgCouponSales;
}

class DlgCouponSales : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgCouponSales(QWidget *parent = 0);
    ~DlgCouponSales();
    static void sale();
    virtual void selector(int selectorNumber, const QVariant &value);
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

    void on_leQty_textChanged(const QString &arg1);

private:
    Ui::DlgCouponSales *ui;
};

#endif // DLGCOUPONSALES_H
