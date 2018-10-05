#ifndef DLGCARSELECTION_H
#define DLGCARSELECTION_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgCarSelection;
}

class DlgCarSelection : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgCarSelection(QWidget *parent = 0);
    ~DlgCarSelection();
    static void selectCar(int &model, QString &govNum, int &costumer);
private slots:
    void on_btnModelSelection_clicked();

    void on_btnCancel_clicked();

    void on_btnSave_clicked();

    void on_btnNumberSelection_clicked();

    void on_btnCostumer_clicked();

private:
    Ui::DlgCarSelection *ui;
    int fModel;

};

#endif // DLGCARSELECTION_H
