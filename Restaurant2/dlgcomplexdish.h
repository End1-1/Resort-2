#ifndef DLGCOMPLEXDISH_H
#define DLGCOMPLEXDISH_H

#include "baseextendeddialog.h"
#include "dishestable.h"

namespace Ui {
class DlgComplexDish;
}

class DlgComplexDish : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgComplexDish(QWidget *parent = 0);
    ~DlgComplexDish();
    static DishComplexStruct *complex(QWidget* parent = 0);
private slots:
    void on_btnCancel_clicked();
    void on_tblData_clicked(const QModelIndex &index);
    void on_btnOk_clicked();
    void on_btnBack_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnSelectAll_clicked();

private:
    Ui::DlgComplexDish *ui;
    DishComplexStruct *fResult;
    QList<DishStruct*> fDishes;
    int fMode;
    void setup();
    void setupDishes();
};

#endif // DLGCOMPLEXDISH_H
