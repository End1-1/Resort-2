#ifndef DLGCHANGEEXPORTVAUCHER_H
#define DLGCHANGEEXPORTVAUCHER_H

#include "baseextendeddialog.h"
#include "dwselectorinvoiceitem.h"

namespace Ui {
class DlgChangeExportVaucher;
}

class DlgChangeExportVaucher : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgChangeExportVaucher(DatabaseResult &dr, QWidget *parent = 0);
    ~DlgChangeExportVaucher();
    void setDatabaseResult(int i = 0);
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    int fIndex;
    Ui::DlgChangeExportVaucher *ui;
    DWSelectorInvoiceItem *fDockItem;
    DatabaseResult &fDr;
};

#endif // DLGCHANGEEXPORTVAUCHER_H
