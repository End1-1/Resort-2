#ifndef REROOMINVENTORYSTATE_H
#define REROOMINVENTORYSTATE_H

#include "roweditordialog.h"

namespace Ui {
class RERoomInventoryState;
}

class RERoomInventoryState : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERoomInventoryState(QList<QVariant>& values, QWidget *parent = 0);
    ~RERoomInventoryState();
    static void openReport();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RERoomInventoryState *ui;
};

#endif // REROOMINVENTORYSTATE_H
