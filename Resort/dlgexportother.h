#ifndef DLGEXPORTOTHER_H
#define DLGEXPORTOTHER_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgExportOther;
}

class DlgExportOther : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgExportOther(QWidget *parent = 0);
    ~DlgExportOther();

private slots:
    void on_btnStart_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgExportOther *ui;
    void processTable(const QString &table, Database &db);
    void processTable2(const QString &table, Database &db);
    void logCount(int total, int current);
};

#endif // DLGEXPORTOTHER_H
