#ifndef RECREDITCARD_H
#define RECREDITCARD_H

#include "roweditordialog.h"

namespace Ui {
class RECreditCard;
}

class RECreditCard : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RECreditCard(QList<QVariant> &values, QWidget *parent = 0);
    ~RECreditCard();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::RECreditCard *ui;
};

#endif // RECREDITCARD_H
