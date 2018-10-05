#ifndef DLGTAXPRINTSETUP_H
#define DLGTAXPRINTSETUP_H

#include "baseextendeddialog.h"
#include "dwselectorhall.h"
#include "dwselectorrestmenu.h"

namespace Ui {
class DlgTaxPrintSetup;
}

class DlgTaxPrintSetup : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgTaxPrintSetup(QWidget *parent = 0);
    ~DlgTaxPrintSetup();

private slots:
    void hall(CI_RestHall *c);
    void menu(CI_RestMenu *c);
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::DlgTaxPrintSetup *ui;
    DWSelectorHall *fDockHall;
    DWSelectorRestMenu *fDockMenu;
};

#endif // DLGTAXPRINTSETUP_H
