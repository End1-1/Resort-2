#ifndef REMODELOFCARS_H
#define REMODELOFCARS_H

#include "roweditordialog.h"

namespace Ui {
class REModelOfCars;
}

class REModelOfCars : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit REModelOfCars(QList<QVariant> &values, QWidget *parent = 0);
    ~REModelOfCars();
    static void openReport();
private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

private:
    Ui::REModelOfCars *ui;
};

#endif // REMODELOFCARS_H
