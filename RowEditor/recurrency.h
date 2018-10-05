#ifndef RECURRENCY_H
#define RECURRENCY_H

#include "roweditordialog.h"

namespace Ui {
class RECurrency;
}

class RECurrency : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECurrency(QList<QVariant> &values, QWidget *parent = 0);
    ~RECurrency();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::RECurrency *ui;
};

#endif // RECURRENCY_H
