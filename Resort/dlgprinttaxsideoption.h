#ifndef DLGPRINTTAXSIDEOPTION_H
#define DLGPRINTTAXSIDEOPTION_H

#include "baseextendeddialog.h"

#define pts_none 0
#define pts_guest 1
#define pts_company 2

namespace Ui {
class DlgPrintTaxSideOption;
}

class DlgPrintTaxSideOption : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPrintTaxSideOption(QWidget *parent = 0);
    ~DlgPrintTaxSideOption();
    static int printTaxSide();
private slots:
    void on_btnPrint_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DlgPrintTaxSideOption *ui;
};

#endif // DLGPRINTTAXSIDEOPTION_H
