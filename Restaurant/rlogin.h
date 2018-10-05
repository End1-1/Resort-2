#ifndef RLOGIN_H
#define RLOGIN_H

#include "baseextendeddialog.h"

namespace Ui {
class RLogin;
}

class RLogin : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RLogin(QWidget *parent = 0);
    ~RLogin();
    static bool getLogin(QString &login, const QString &title, QWidget *parent = 0);
    virtual void returnPressed();
    virtual void escapePressed();
private slots:
    void numberButtonClicked();
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

    void on_btnClear_clicked();

private:
    Ui::RLogin *ui;
};

#endif // RLOGIN_H
