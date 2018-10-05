#ifndef DLGDISHHISTORY_H
#define DLGDISHHISTORY_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgDishHistory;
}

class DlgDishHistory : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgDishHistory(const QString &dishId, QWidget *parent = 0);
    ~DlgDishHistory();

private:
    Ui::DlgDishHistory *ui;
};

#endif // DLGDISHHISTORY_H
