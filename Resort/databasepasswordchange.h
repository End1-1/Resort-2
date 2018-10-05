#ifndef DATABASEPASSWORDCHANGE_H
#define DATABASEPASSWORDCHANGE_H

#include "baseextendeddialog.h"

namespace Ui {
class DatabasePasswordChange;
}

class DatabasePasswordChange : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DatabasePasswordChange(QWidget *parent = 0);
    ~DatabasePasswordChange();

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DatabasePasswordChange *ui;
};

#endif // DATABASEPASSWORDCHANGE_H
