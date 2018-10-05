#ifndef DLGHOUSEITEM_H
#define DLGHOUSEITEM_H

#include "baseextendeddialog.h"
#include "dwselectorroominventory.h"

namespace Ui {
class DlgHouseItem;
}

class DlgHouseItem : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgHouseItem(QWidget *parent = 0);
    ~DlgHouseItem();
    virtual void selector(int selectorNumber, const QVariant &value);
    static void openWindow();
private slots:
    void leFocusOut();
    void leFocusIn();
    void comboStateCurrentIndexChanged(int index);
    void on_btnAddItem_clicked();

    void on_btnSave_clicked();

    void on_btnRemoveItem_clicked();

private:
    Ui::DlgHouseItem *ui;
    EQLineEdit *fleInventory;
    DWSelectorRoomInventory *fDockInventory;
    void loadRoom();
    int addRow();
};

#endif // DLGHOUSEITEM_H
