#ifndef RERESTDISHMOD_H
#define RERESTDISHMOD_H

#include "roweditordialog.h"

namespace Ui {
class RERestDishMod;
}

class RERestDishMod : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestDishMod(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestDishMod();

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::RERestDishMod *ui;
};

#endif // RERESTDISHMOD_H
