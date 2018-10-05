#ifndef DLGPRINTTAXNEW_H
#define DLGPRINTTAXNEW_H

#include "baseextendeddialog.h"

namespace Ui {
class DlgPrintTaxNew;
}

class DlgPrintTaxNew : public BaseExtendedDialog
{
    Q_OBJECT

public:
    explicit DlgPrintTaxNew(QWidget *parent = 0);
    ~DlgPrintTaxNew();

private:
    Ui::DlgPrintTaxNew *ui;
};

#endif // DLGPRINTTAXNEW_H
