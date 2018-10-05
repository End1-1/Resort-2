#ifndef RSELECTTABLE_H
#define RSELECTTABLE_H

#include "baseextendeddialog.h"
#include "hall.h"

namespace Ui {
class RSelectTable;
}

class RSelectTable : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit RSelectTable(QWidget *parent = 0);
    ~RSelectTable();
    void setup(int hallId);
    TableStruct *table();
private slots:
    void on_btnReject_clicked();
    void on_btnChangeHall_clicked();
    void on_tblTables_clicked(const QModelIndex &index);
private:
    TableStruct *fTable;
    Ui::RSelectTable *ui;
};

#endif // RSELECTTABLE_H
