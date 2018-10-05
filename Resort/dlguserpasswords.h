#ifndef DLGUSERPASSWORDS_H
#define DLGUSERPASSWORDS_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgUserPasswords;
}

class DlgUserPasswords : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgUserPasswords(QWidget *parent = 0);
    ~DlgUserPasswords();
    void setSelfMode();
    void setParams(int userId, const QString &name);

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_chManager_clicked(bool checked);

    void on_chWaiter_clicked(bool checked);

private:
    Ui::DlgUserPasswords *ui;
    int fUserId;
};

#endif // DLGUSERPASSWORDS_H
