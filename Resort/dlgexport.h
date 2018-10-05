#ifndef DLGEXPORT_H
#define DLGEXPORT_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgExport;
}

class DlgExport : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgExport(QWidget *parent = 0);
    ~DlgExport();
    static void start();
private slots:
    void on_btnStart_clicked();

private:
    Ui::DlgExport *ui;
    void replacePtLastLine(const QString &str);
    bool processTable(Database &db, const QString &table, QCheckBox *ch);
};

#endif // DLGEXPORT_H
