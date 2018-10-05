#ifndef RECOUPONSERIA_H
#define RECOUPONSERIA_H

#include "roweditordialog.h"

namespace Ui {
class RECouponSeria;
}

class RECouponSeria : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECouponSeria(QList<QVariant> &values, QWidget *parent = 0);
    ~RECouponSeria();
    static void openReport();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RECouponSeria *ui;
};

#endif // RECOUPONSERIA_H
