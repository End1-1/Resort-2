#ifndef REROOMINVENTORY_H
#define REROOMINVENTORY_H

#include "roweditordialog.h"

namespace Ui {
class RERoomInventory;
}

class RERoomInventory : public RowEditorDialog
{
    Q_OBJECT

public:
    explicit RERoomInventory(QList<QVariant> &values, QWidget *parent = 0);
    ~RERoomInventory();
    static void openRoomInventoryReport();
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::RERoomInventory *ui;
};

#endif // REROOMINVENTORY_H
