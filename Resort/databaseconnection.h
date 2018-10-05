#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include "baseextendeddialog.h"

namespace Ui {
class DatabaseConnection;
}

class DatabaseConnection : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DatabaseConnection(QWidget *parent = 0);
    void setConnection(const QString &name);
    ~DatabaseConnection();

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

    void on_btnTest_clicked();

private:
    Ui::DatabaseConnection *ui;
    QString fOldDbName;
    bool fAppendMode;
};

#endif // DATABASECONNECTION_H
