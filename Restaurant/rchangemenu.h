#ifndef RCHANGEMENU_H
#define RCHANGEMENU_H

#include "baseextendeddialog.h"

namespace Ui {
class RChangeMenu;
}

class RChangeMenu : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RChangeMenu(QWidget *parent = 0);
    ~RChangeMenu();
    static bool changeMenu(int currentMenu, int &newMenu, QWidget *parent = 0);
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::RChangeMenu *ui;
};

#endif // RCHANGEMENU_H
