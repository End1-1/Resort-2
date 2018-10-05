#ifndef DLGRAISEUSER_H
#define DLGRAISEUSER_H

#include "baseextendeddialog.h"
#include "cacherights.h"

namespace Ui {
class DlgRaiseUser;
}

class DlgRaiseUser : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgRaiseUser(QWidget *parent = 0);
    ~DlgRaiseUser();
    static int raiseUser(int role);
protected:
    virtual void returnPressed();
    virtual void escapePressed();
private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgRaiseUser *ui;
    int fUser;
    int fRole;
};

#endif // DLGRAISEUSER_H
