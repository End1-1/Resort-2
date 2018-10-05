#ifndef RECOUPON_H
#define RECOUPON_H

#include "roweditordialog.h"

namespace Ui {
class RECoupon;
}

class RECoupon : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECoupon(QList<QVariant> &values, QWidget *parent = 0);
    ~RECoupon();
    static void openReport();
    virtual void selector(int selectorNumber, const QVariant &value);
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RECoupon *ui;
};

#endif // RECOUPON_H
