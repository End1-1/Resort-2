#ifndef DATABASESCONNECTIONS_H
#define DATABASESCONNECTIONS_H

#include "baseextendeddialog.h"

namespace Ui {
class DatabasesConnections;
}

class DatabasesConnections : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DatabasesConnections(QWidget *parent = 0);
    ~DatabasesConnections();

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnRemove_clicked();
    void on_btnChangePassword_clicked();
    void on_chDirectConnection_clicked(bool checked);

private:
    Ui::DatabasesConnections *ui;
    void loadDatabases();
};

#endif // DATABASESCONNECTIONS_H
