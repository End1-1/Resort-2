#ifndef DLGEXPORTSINGLEVAUCHER_H
#define DLGEXPORTSINGLEVAUCHER_H

#include "baseextendeddialog.h"
#include "databaseresult.h"

namespace Ui {
class DlgExportSingleVaucher;
}

class DlgExportSingleVaucher : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgExportSingleVaucher(QWidget *parent = 0);
    ~DlgExportSingleVaucher();
    void setupVaucher(const QString &id);
    static int exportSingleVaucher(const QString &id);
private slots:
    void showInvoices(bool v);
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_leInvoice_returnPressed();

private:
    Database fSDb;
    DatabaseResult fVaucher;
    QString fVaucherId;
    Ui::DlgExportSingleVaucher *ui;
};

#endif // DLGEXPORTSINGLEVAUCHER_H
