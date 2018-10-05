#ifndef RERESTDISHCOMPLEX_H
#define RERESTDISHCOMPLEX_H

#include "roweditordialog.h"
#include "dwselectorrestmenu.h"
#include "deselectormenudishes.h"

namespace Ui {
class RERestDishComplex;
}

class RERestDishComplex : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERestDishComplex(QList<QVariant> &values, QWidget *parent = 0);
    ~RERestDishComplex();
    virtual void valuesToWidgets();
    virtual void clearWidgets();
private slots:
    void menu(CI_RestMenu *c);
    void dish(CI_MenuDishes *c);
    void removeRow(int row);
    void on_btnCancel_clicked();
    void on_btnOK_clicked();
    void on_btnAdd_clicked();

private:
    Ui::RERestDishComplex *ui;
    DWSelectorRestMenu *fDockMenu;
    DESelectorMenuDishes *fDockDish;
    void countTotal();
};

#endif // RERESTDISHCOMPLEX_H
