#ifndef DLGCHANGEPASSWORD_H
#define DLGCHANGEPASSWORD_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgChangePassword;
}

class DlgChangePassword : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgChangePassword(QWidget *parent = nullptr);
    ~DlgChangePassword();

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgChangePassword *ui;
};

#endif // DLGCHANGEPASSWORD_H
