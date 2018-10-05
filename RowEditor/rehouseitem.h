#ifndef REHOUSEITEM_H
#define REHOUSEITEM_H

#include "baseextendeddialog.h"

namespace Ui {
class REHouseItem;
}

class REHouseItem : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit REHouseItem(QWidget *parent = 0);
    ~REHouseItem();

private:
    Ui::REHouseItem *ui;
};

#endif // REHOUSEITEM_H
