#ifndef DLGWELCOMEBUTTONCONFIG_H
#define DLGWELCOMEBUTTONCONFIG_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgWelcomeButtonConfig;
}

class DlgWelcomeButtonConfig : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgWelcomeButtonConfig(QWidget *parent = 0);
    ~DlgWelcomeButtonConfig();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::DlgWelcomeButtonConfig *ui;
};

#endif // DLGWELCOMEBUTTONCONFIG_H
